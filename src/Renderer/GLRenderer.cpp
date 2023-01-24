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
    //
}

void GLRenderer::RenderInternalQuad()
{
    glDrawElements(GL_TRIANGLES, _quad->GetIB()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GLRenderer::OnRenderModeChange()
{
    if(_mode == GLRenderMode::RAYTRACER)
    {
        _vao->AddBuffer(_quad->GetVB(), *_quad->GetLayout());
        _quad->GetIB()->Bind();
        _quadShader->Bind();
    }
}
