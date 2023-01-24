#pragma once

#include "Geometry/ScreenQuad.h"
#include "GPU/GL/GLVertexArray.h"
#include "GPU/Shader.h"
#include "pch.h"

class Scene;

enum class GLRenderMode
{
    DEFAULT,
    RAYTRACER
};

class GLRenderer
{
    public:
        GLRenderer();
        ~GLRenderer();
        void Clear();
        void OnResize(int width, int height);
        void Render(const Scene& scene);
        inline void SetMode(GLRenderMode mode)
        {
            bool changed = _mode != mode;
            _mode = mode;
            if(changed)
            {
                OnRenderModeChange();
            }
        }
        inline GLRenderMode GetMode() const
        {
            return _mode;
        }
    private:
        void RenderInternal(const Scene& scene);        
        void RenderInternalQuad();
        void OnRenderModeChange();
        std::unique_ptr<GLVertexArray> _vao; // single VAO for now
        std::unique_ptr<ScreenQuad> _quad;
        std::unique_ptr<Shader> _quadShader;
        GLRenderMode _mode;
};
