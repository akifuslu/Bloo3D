#pragma once

#include "glm/glm.hpp"
#include "BS_thread_pool.hpp"
#include "pch.h"

struct Ray;
struct RayHit;
class Mesh;
class Camera;
class PointLight;
class MaterialBase;
class Texture;

class Raytracer
{
    public:
        Raytracer(Camera* camera, Texture* target);
        ~Raytracer();
        int AddMesh(Mesh* mesh);
        int AddLight(PointLight* light);
        int AddMaterial(MaterialBase* mat);
        void Render();
        bool RayCast(const Ray& ray, RayHit* hit);
        void Refresh();
        void OnResize(int width, int height);
    private:
        glm::vec3 GetFragColor(const Ray& ray, const RayHit& frag);
        void RenderInternal();
        std::vector<unsigned char> _buffer;
        Camera* _camera;
        std::vector<Mesh*> _meshes;
        std::vector<PointLight*> _lights;
        std::vector<MaterialBase*> _mats;

        volatile std::atomic_bool _onRender;
        volatile std::atomic_uint32_t _pixelIndex;
        std::vector<std::future<void>> _futures;
        Texture* _target;

        BS::thread_pool _pool;
};
