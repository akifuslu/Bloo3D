#include "GLRenderer.h"

#include "Scene/Scene.h"
#include "Logger.h"
#include "pch.h"
#include <GL/glew.h>

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

    // init matrix buffer, we pass no data since we dont have the camera info yet
    // but we pass the size so GPU can reserve the space for later use
    _matrixBuffer.reset(UniformBuffer::Create(nullptr, sizeof(glm::mat4)));
    _matrixBuffer->BindIndex(0);
     // TODO: we may need to keep track of block bindings for different UBOs
    _editorLitShader->SetUniformBlockBinding("Matrices", 0);
    _editorOutlineShader->SetUniformBlockBinding("Matrices", 0);
    _unlitShader->SetUniformBlockBinding("Matrices", 0);
    _gridShader->SetUniformBlockBinding("Matrices", 0);    
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
        _editorLitShader->SetVec3("Light.color", scene.editorLight->GetColorPower());
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
                _editorOutlineShader->SetVec3("OutlineColor", {0.0, 0.3686, 1.0});
            }
            else
            {
                _editorOutlineShader->SetVec3("OutlineColor", {0.39608,  0.58431,  0.91373});
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
    // render grid last
    _gridShader->Bind();
    _gridShader->SetFloat("CameraY", scene.editorCamera->transform.GetLocation().y);
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
