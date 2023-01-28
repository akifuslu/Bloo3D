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
    // init unlit shader(for now)
    _unlitShader.reset(Shader::Create("res/shaders/Unlit.shader"));
}

GLRenderer::~GLRenderer() 
{

}

void GLRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
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
    auto viewProj = scene.mainCam->GetVP();
    //glm::mat4 proj = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.01f, 100.0f);
    //glm::mat4 view = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0), glm::vec3(0, 1, 0));

    //auto viewProj = proj * view;

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(err);
    }
    for(auto& mesh: scene.meshes)
    {
        mesh->UpdateRenderInfo();
        auto mvp = viewProj * glm::identity<glm::mat4>(); //viewProj * mesh->transform.LocalToWorld();
        _unlitShader->Bind();
        _unlitShader->SetMatrix4("MVP", mvp);
        _vao->AddBuffer(mesh->renderInfo.vb.get(), *mesh->renderInfo.layout.get());
        mesh->renderInfo.ib->Bind();
        glDrawElements(GL_TRIANGLES, mesh->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    // auto mvp = viewProj * glm::identity<glm::mat4>(); //viewProj * mesh->transform.LocalToWorld();
    // _unlitShader->Bind();
    // _unlitShader->SetMatrix4("MVP", mvp);
    
    // _vao->AddBuffer(_quad->renderInfo.vb.get(), *_quad->renderInfo.layout.get());
    // _quad->renderInfo.ib->Bind();
    glDrawElements(GL_TRIANGLES, _quad->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GLRenderer::RenderInternalQuad()
{
    // TODO: avoid redundant binds in buffers, shaders, etc.
    _vao->AddBuffer(_quad->renderInfo.vb.get(), *_quad->renderInfo.layout.get());
    _quad->renderInfo.ib->Bind();
    _quadShader->Bind();
    glDrawElements(GL_TRIANGLES, _quad->renderInfo.ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}
