#pragma once

#include <vector>
#include <atomic>

class Mesh;
class Ray;
class RayHit;
class Camera;
class PointLight;

class Renderer{

    public:
        Renderer(Camera* camera, unsigned char* buffer, int width, int height);
        ~Renderer();
        void AddMesh(const Mesh& mesh);
        void AddLight(const PointLight& light);
        void Render();
        bool RayCast(const Ray& ray, RayHit& hit);
        void Refresh();
    private:
        void RenderInternal(unsigned char* buffer, int width, int height);
        unsigned char* _buffer;
        int _width;
        int _height;
        std::vector<Mesh> _meshes;
        std::vector<PointLight> _lights;
        Camera* _camera;
        std::atomic_bool _refresh;
        std::atomic_bool _onRender;
};
