#pragma once

#include "GPUTexture.hpp"
#include "GPUMaterial.hpp"
#include "GPUMesh.hpp"

#include <memory>
#include <unordered_map>

namespace myvk {

using ResourceHandle = uint32_t;

class GPUResouces {
private:
    Device &device;
    std::vector<std::unique_ptr<GPUMesh>> meshes;
    std::vector<std::unique_ptr<GPUTexture>> textures;
    std::vector<std::unique_ptr<GPUMaterial>> materials;
public:
    GPUResouces();
    ~GPUResouces();

    void createMesh(std::shared_ptr<MeshInstance> meshInstance);
    void createTexture();
    void createMaterial();
};



}