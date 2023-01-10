#pragma once

#include <vector>
#include <atomic>

class Mesh;
class Ray;
class Camera;

class Renderer{

    public:
        Renderer(Camera* camera, unsigned char* buffer, int width, int height);
        ~Renderer();
        void AddMesh(const Mesh& mesh);
        void Render();
        bool RayCast(const Ray& ray);
        void Refresh();
    private:
        void RenderInternal(unsigned char* buffer, int width, int height);
        unsigned char* _buffer;
        int _width;
        int _height;
        std::vector<Mesh> _meshes;
        Camera* _camera;
        std::atomic_bool _refresh;
        bool _onRender;
};
