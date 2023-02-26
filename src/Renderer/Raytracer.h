#pragma once

#include "glm/glm.hpp"
#include "BS_thread_pool.hpp"
#include "pch.h"

struct Ray;
struct RayHit;
class Camera;
class MaterialBase;
class Texture;
class Scene;

class Raytracer
{
    public:
        Raytracer(Camera* camera, Texture* target);
        ~Raytracer();
        int AddMaterial(MaterialBase* mat);
        void Render(Scene* scene);
        void Refresh();
        void OnResize(int width, int height);
    private:
        glm::vec3 GetFragColor(Scene* scene, const Ray& ray, const RayHit& frag);
        void RenderInternal(Scene* scene);
        std::vector<unsigned char> _buffer;
        Camera* _camera;
        std::vector<MaterialBase*> _mats;

        volatile std::atomic_bool _onRender;
        volatile std::atomic_uint32_t _pixelIndex;
        std::vector<std::future<void>> _futures;
        Texture* _target;

        BS::thread_pool _pool;
};
