#pragma once

#include <vector>
#include <atomic>
#include <future>
#include <chrono>
#include <memory>

class Mesh;
class Ray;
class RayHit;
class Camera;
class PointLight;

class Renderer{

    public:
        Renderer(std::shared_ptr<Camera> camera, unsigned char* buffer, int width, int height);
        ~Renderer();
        void AddMesh(std::shared_ptr<Mesh> mesh);
        void AddLight(std::shared_ptr<PointLight> light);
        void Render();
        bool RayCast(const Ray& ray, RayHit* hit);
        void Refresh();
    private:
        void RenderInternal();
        unsigned char* _buffer;
        int _width;
        int _height;
        int _size;
        std::shared_ptr<Camera> _camera;
        std::vector<std::shared_ptr<Mesh>> _meshes;
        std::vector<std::shared_ptr<PointLight>> _lights;
        volatile std::atomic_bool _refresh;
        volatile std::atomic_bool _onRender;
        volatile std::atomic_int _pixelIndex;
        volatile std::atomic_int _finishedThreads;
        std::future<void> _futureCompletion;
        std::vector<std::future<void>> _futures;
};
