#include "GLRenderer.h"

#include "Scene/Scene.h"
#include "Logger.h"
#include "pch.h"
#include "ResourceManager.h"
#include "Styles.h"
#include <GL/glew.h>
#include "glm/gtx/string_cast.hpp"

GLRenderer::GLRenderer() 
{
    glewInit();    
    // init vao
    _vao.reset(new GLVertexArray());
    _vao->Bind();
    // init screen quad
    _quad.reset(new ScreenQuad());
    _quadShader.reset(Shader::Create(s_BasePath + "/res/shaders/TexturedScreenQuad.shader"));
    // init grid
    _grid.reset(new ScreenQuad());
    _gridShader.reset(Shader::Create(s_BasePath + "/res/shaders/InfiniteGrid.shader"));
    // init editor lit shader
    _editorLitShader.reset(Shader::Create(s_BasePath + "/res/shaders/EditorLit.shader"));
    // init editor outline shader
    _editorOutlineShader.reset(Shader::Create(s_BasePath + "/res/shaders/EditorOutline.shader"));
    // init unlit shader
    _unlitShader.reset(Shader::Create(s_BasePath + "/res/shaders/Unlit.shader"));
    // init point shader
    _pointShader.reset(Shader::Create(s_BasePath + "/res/shaders/PointSprite.shader"));

    _pointBuffer.reset(VertexBuffer::Create(&glm::vec3(0)[0], sizeof(float) * 3));
    _pbLayout.Push<float>(3);    
    // init matrix buffer, we pass no data since we dont have the camera info yet
    // but we pass the size so GPU can reserve the space for later use
    _matrixBuffer.reset(UniformBuffer::Create(nullptr, sizeof(glm::mat4)));
    _matrixBuffer->BindIndex(0);
     // TODO: we may need to keep track of block bindings for different UBOs
    _editorLitShader->SetUniformBlockBinding("Matrices", 0);
    _editorOutlineShader->SetUniformBlockBinding("Matrices", 0);
    _unlitShader->SetUniformBlockBinding("Matrices", 0);
    _gridShader->SetUniformBlockBinding("Matrices", 0);    
    _pointShader->SetUniformBlockBinding("Matrices", 0);

    glPointSize(Styles::PointSize);
}

GLRenderer::~GLRenderer() 
{

}

void GLRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLRenderer::Render(Scene& scene) 
{
    if(_mode == GLRenderMode::DEFAULT)
    {
        RenderInternal(scene);
    }
    else if(_mode == GLRenderMode::RAYTRACER)
    {
        RenderInternalQuad();
    }
    else
    {
        LOGERROR("GLRenderer: Invalid render mode.");
    }
}

void GLRenderer::RenderInternal(Scene& scene)
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(err);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    auto viewProj = scene.editorCamera->GetViewProj();
    _matrixBuffer->Update(&viewProj[0][0], sizeof(glm::mat4));
    // render others
    for(auto& mesh: scene.GetMeshes())
    {
        mesh->UpdateRenderInfo();
        auto m = mesh->transform.LocalToWorld();
        auto tim = mesh->transform.T_WorldToLocal();
        _editorLitShader->Bind();
        _editorLitShader->SetMatrix4("M", m);
        _editorLitShader->SetMatrix4("T_IM", tim);
        _editorLitShader->SetVec3("Light.dir", scene.editorLight->transform.Forward());
        _editorLitShader->SetVec3("Light.color", scene.editorLight->power * scene.editorLight->color);
        _editorLitShader->SetVec3("CameraPos", scene.editorCamera->transform.GetLocation());
        _vao->AddBuffer(mesh->renderInfo.vb.get(), *mesh->renderInfo.layout.get());
        mesh->renderInfo.ib->Bind();
        glDrawElements(GL_TRIANGLES, mesh->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    // render outlines
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    for(auto& mesh: scene.GetMeshes())
    {
        if(mesh->selected) // draw outline
        {
            auto m = mesh->transform.LocalToWorld();
            _editorOutlineShader->Bind();
            _editorOutlineShader->SetMatrix4("M", m);
            _editorOutlineShader->SetVec2("ScreenSize", scene.editorCamera->GetSize());
            if(scene.activeObject.Get() == mesh)
            {
                _editorOutlineShader->SetVec3("OutlineColor", Styles::ActiveObjectColor);
            }
            else
            {
                _editorOutlineShader->SetVec3("OutlineColor", Styles::SelectedObjectColor);
            }
            _vao->AddBuffer(mesh->renderInfo.vb.get(), *mesh->renderInfo.layout.get());
            mesh->renderInfo.ib->Bind();
            glDrawElements(GL_TRIANGLES, mesh->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
    glDisable(GL_CULL_FACE);

    // transparent objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    // render light gizmos
    for(auto& light: scene.GetLights())
    {
        auto m = light->transform.LocalToWorld();
        _pointShader->Bind();
        _pointShader->SetMatrix4("M", m);
        _pointShader->SetInt("Tex", 0);
        if(scene.activeObject.Get() == light)
        {
            _pointShader->SetVec3("BaseColor", Styles::ActiveObjectColor);
        }
        else if(light->selected)
        {
            _pointShader->SetVec3("BaseColor", Styles::SelectedObjectColor);
        }
        else
        {
            _pointShader->SetVec3("BaseColor", Styles::LightGizmoColor);
        }
        ResourceManager::Instance().GetInternalTexture(InternalTextureTag::POINT_LIGHT_GIZMO)->Bind(0);
        _vao->AddBuffer(_pointBuffer.get(), _pbLayout);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    // render grid last
    _gridShader->Bind();
    auto gp = scene.editorCamera->GetGridPlane();
    _gridShader->SetInt("Plane", gp.first);
    _gridShader->SetBool("ForceFar", gp.second);
    _gridShader->SetVec3("CameraPos", scene.editorCamera->transform.GetLocation());
    _gridShader->SetFloat("Near", scene.editorCamera->GetNear());
    _gridShader->SetFloat("Far", scene.editorCamera->GetFar());
    _gridShader->SetMatrix4("InvView", scene.editorCamera->GetInvView());
    _gridShader->SetMatrix4("InvProj", scene.editorCamera->GetInvProj());
    _vao->AddBuffer(_grid->renderInfo.vb.get(), *_grid->renderInfo.layout.get());
    _grid->renderInfo.ib->Bind();
    glDrawElements(GL_TRIANGLES, _grid->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
}

void GLRenderer::RenderInternalQuad()
{
    // TODO: avoid redundant binds in buffers, shaders, etc.
    _vao->AddBuffer(_quad->renderInfo.vb.get(), *_quad->renderInfo.layout.get());
    _quad->renderInfo.ib->Bind();
    _quadShader->Bind();
    glDrawElements(GL_TRIANGLES, _quad->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}
