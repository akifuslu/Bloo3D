#pragma once

#include<vector>

class Mesh;
class Ray;
class Camera;

class Renderer{

    public:
        Renderer(int width, int height);
        ~Renderer();
        void AddMesh(const Mesh& mesh);
        void Render(unsigned char* buffer, int width, int height, int frame);
        bool RayCast(const Ray& ray);
    private:
        std::vector<Mesh> _meshes;
        Camera* _camera;

};
