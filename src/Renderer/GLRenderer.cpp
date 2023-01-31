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
    _quadShader.reset(Shader::Create("res/shaders/TexturedScreenQuad.shader"));
    // init grid
    _grid.reset(new ScreenQuad());
    _gridShader.reset(Shader::Create("res/shaders/InfiniteGrid.shader"));
    // init unlit shader(for now)
    _unlitShader.reset(Shader::Create("res/shaders/Unlit.shader"));
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

void GLRenderer::Render(const Scene& scene) 
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

void GLRenderer::RenderInternal(const Scene& scene)
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(err);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    auto viewProj = scene.mainCam->GetViewProj();
    // render others
    for(auto& mesh: scene.meshes)
    {
        mesh->UpdateRenderInfo();
        auto mvp = viewProj * mesh->transform.LocalToWorld();
        _unlitShader->Bind();
        _unlitShader->SetMatrix4("MVP", mvp);
        _vao->AddBuffer(mesh->renderInfo.vb.get(), *mesh->renderInfo.layout.get());
        mesh->renderInfo.ib->Bind();
        glDrawElements(GL_TRIANGLES, mesh->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    // transparent objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    // render grid last
    _gridShader->Bind();
    _gridShader->SetFloat("CameraY", scene.mainCam->transform.GetLocation().y);
    _gridShader->SetFloat("Near", scene.mainCam->GetNear());
    _gridShader->SetFloat("Far", scene.mainCam->GetFar());
    _gridShader->SetMatrix4("ViewProj", viewProj);
    _gridShader->SetMatrix4("InvView", scene.mainCam->GetInvView());
    _gridShader->SetMatrix4("InvProj", scene.mainCam->GetInvProj());
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
