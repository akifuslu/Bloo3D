#pragma once

#include "glm/glm.hpp"
#include "BS_thread_pool.hpp"
#include "pch.h"

class Mesh;
class Ray;
class RayHit;
class Camera;
class PointLight;
class MaterialBase;
class Texture;

class Renderer{

    public:
        Renderer(Camera* camera, unsigned char* buffer, int width, int height, Texture* target);
        ~Renderer();
        int AddMesh(Mesh* mesh);
        int AddLight(PointLight* light);
        int AddMaterial(MaterialBase* mat);
        void Render();
        bool RayCast(const Ray& ray, RayHit* hit);
        void Refresh();
    private:
        glm::vec3 GetFragColor(const Ray& ray, const RayHit& frag);
        void RenderInternal();
        unsigned char* _buffer;
        int _width;
        int _height;
        int _size;
        Camera* _camera;
        std::vector<Mesh*> _meshes;
        std::vector<PointLight*> _lights;
        std::vector<MaterialBase*> _mats;

        volatile std::atomic_bool _refresh;
        volatile std::atomic_bool _onRender;
        volatile std::atomic_int _pixelIndex;
        volatile std::atomic_int _finishedThreads;
        std::future<void> _futureCompletion;
        std::vector<std::future<void>> _futures;
        Texture* _target;

        BS::thread_pool _pool;
};
