#pragma once

#include <vector>
#include <atomic>
#include <future>
#include <chrono>

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
        void RenderInternal();
        unsigned char* _buffer;
        int _width;
        int _height;
        int _size;
        std::vector<Mesh> _meshes;
        std::vector<PointLight> _lights;
        Camera* _camera;
        volatile std::atomic_bool _refresh;
        volatile std::atomic_bool _onRender;
        volatile std::atomic_int _pixelIndex;
        volatile std::atomic_int _finishedThreads;
        std::future<void> _futureCompletion;
        std::vector<std::future<void>> _futures;
};
