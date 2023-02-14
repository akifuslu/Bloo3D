#pragma once

#include "Geometry/Mesh.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "pch.h"

namespace Importer
{
    void ImportSingle(aiMesh* aimesh, Mesh* mesh)
    {
        for (size_t i = 0; i < aimesh->mNumVertices; i++)
        {
            auto vert = aimesh->mVertices[i];
            if(aimesh->HasNormals())
            {
                auto norm = aimesh->mNormals[i];
                mesh->AddVertex({vert.x, vert.y, vert.z}, {norm.x, norm.y, norm.z});
            }
            else
            {
                mesh->AddVertex({vert.x, vert.y, vert.z});
            }
        }
        
        for (size_t i = 0; i < aimesh->mNumFaces; i++)
        {
            auto tri = aimesh->mFaces[i];
            mesh->AddTriangle(tri.mIndices[0], tri.mIndices[1], tri.mIndices[2]);
        }        

        if(!aimesh->HasNormals())
        {
            mesh->RecalculateNormals();
        }
    }

    bool Import(const std::string& filepath, Mesh* mesh)
    {
        // Create an instance of the Importer class
        Assimp::Importer importer;

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* scene = importer.ReadFile( filepath,
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

        // If the import failed, report it
        if (nullptr == scene) {
            std::cout << importer.GetErrorString() << std::endl;
            return false;
        }
        
        //auto msh = scene->mMeshes[0];
        ImportSingle(scene->mMeshes[0], mesh);


        // We're done. Everything will be cleaned up by the importer destructor
        return true;        
    }
}
