#pragma once

#include "model/Model.hpp"
#include "model/GPUMesh.hpp"
#include "model/GPUMaterial.hpp"

#include "Descriptors.hpp"
#include "FreeList.hpp"

#include <limits>
#include <memory>
#include <deque>
#include <unordered_map>

namespace myvk {

template <typename T>
struct Handle {
    uint32_t handle = std::numeric_limits<uint32_t>().max(); // DEFAULT INVALID HANDLE //
};

struct DrawMesh {
	uint32_t firstVertex;
	uint32_t firstIndex;
	uint32_t indexCount;
	uint32_t vertexCount;
	bool isMerged;

	std::unique_ptr<GPUMesh> gpuData;
    const Mesh* original;
    uint32_t refCount = 0;
};

struct DrawMaterial {
    std::unique_ptr<GPUMaterial> gpuData;
    const Material* original;
    uint32_t refCount = 0;
};

struct RenderObject {
    Handle<DrawMesh> mesh;
    Handle<DrawMaterial> material;
    glm::mat4 transform;
};

class RenderSystem;

class RenderScene {
private:
    Device& device;
    DescriptorPoolManager& pool;
    DescriptorSetLayout& materialLayout;
    
    FreeList<RenderObject> renderables;
    FreeList<DrawMesh> meshes;
    FreeList<DrawMaterial> materials;

	std::unordered_map<const Mesh*, Handle<DrawMesh>> meshConvert;
    std::unordered_map<const Material*, Handle<DrawMaterial>> materialConvert;
    
    Handle<DrawMesh> getMeshHandle(const Mesh* mesh);
    Handle<DrawMaterial> getMaterialHandle(const Material* material);

    void deleteMeshDeffered(Handle<DrawMesh> &handle);
    void deleteMaterialDeffered(Handle<DrawMaterial> &handle);

    // Creates material where albedo is 1x1 white texture, sets by default if there's no material in meshObject //
    void createEmptyMaterial(); 

    static constexpr uint32_t INVALID_HANDLE   = std::numeric_limits<uint32_t>().max();
    static constexpr uint32_t DESTROYED_HANDLE = INVALID_HANDLE-1;

    Handle<DrawMaterial> defMaterialHandle;

    friend class RenderSystem;
public:
    RenderScene(Device& device, DescriptorPoolManager& pool, DescriptorSetLayout& materialLayout);

    Handle<RenderObject> registerObject(const MeshObject* meshObject);
    void updateTransform(Handle<RenderObject> objectId, const glm::mat4& localToWorld);
    void updateMeshData(const MeshObject* meshObject, Handle<RenderObject> objectId);
    //void updateMaterialData(const MeshObject* meshObject, Handle<RenderObject> objectId);

    void deleteObjectDeffered(Handle<RenderObject> objectId);
};

}
