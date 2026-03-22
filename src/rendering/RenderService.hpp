#pragma once

#include "../model/MeshObject.hpp"
#include "../vk/GPUTexture.hpp"
#include "../vk/GPUMesh.hpp"
#include "../vk/GPUMaterial.hpp"
#include "../vk/Descriptors.hpp"

#include "../FreeList.hpp"

#include <limits>
#include <memory>
#include <deque>
#include <unordered_map>

namespace myvk {

template <typename T>
struct Handle {
    static constexpr uint32_t INVALID_HANDLE   = std::numeric_limits<uint32_t>().max();
    static constexpr uint32_t DESTROYED_HANDLE = INVALID_HANDLE-1;

    uint32_t handle = std::numeric_limits<uint32_t>().max(); // DEFAULT INVALID HANDLE //

    inline bool valid() {return handle < DESTROYED_HANDLE;}
    static inline bool valid(uint32_t value) {return value < DESTROYED_HANDLE;}
    
    inline void destroy() {handle = DESTROYED_HANDLE;}
};

struct DrawMesh {
	uint32_t firstVertex;
	uint32_t firstIndex;
	uint32_t indexCount;
	uint32_t vertexCount;
	bool isMerged;

	std::unique_ptr<GPUMesh> gpuData;
    std::string tag;
};

struct DrawMaterial {
    std::unique_ptr<GPUMaterial> gpuData;
    std::string tag;
};

struct RenderObject {
    Handle<DrawMesh> mesh;
    Handle<DrawMaterial> material;
    std::string tag;
};

struct DrawCommand {
    glm::mat4 transform;
    uint32_t  object_id;
};

class RenderSystem;

class RenderService {
private:
    Device& device;
    DescriptorPoolManager& pool;
    DescriptorSetLayout& materialLayout;
    
    FreeList<RenderObject> renderables;
    FreeList<DrawMesh> meshes;
    FreeList<DrawMaterial> materials;

    std::unordered_map<std::string, Handle<RenderObject>> tagObject;
    std::unordered_map<std::string, Handle<DrawMesh>> tagMeshMap;
    std::unordered_map<std::string, Handle<DrawMaterial>> tagMaterialMap;

    std::vector<DrawCommand> drawList;

    void deleteMeshDeffered(Handle<DrawMesh> &handle);
    void deleteMaterialDeffered(Handle<DrawMaterial> &handle);

    // Creates material where albedo is 1x1 white texture, sets by default if there's no material in meshObject //
    void createEmptyMaterial(); 

    Handle<DrawMaterial> defMaterialHandle;

    friend class RenderSystem;
public:
    RenderService(Device& device, DescriptorPoolManager& pool, DescriptorSetLayout& materialLayout);

    void render(Handle<RenderObject> id, const glm::mat4& transform);

    Handle<RenderObject> registerRenderObject(RenderObject object);
    
    Handle<DrawMesh> createMeshHandle(const Mesh* mesh, std::string tag = "");
    Handle<DrawMaterial> createMaterialHandle(const Material* material, std::string tag = "");

    Handle<DrawMesh> getMeshHandle(std::string tag = "");
    Handle<DrawMaterial> getMaterialHandle(std::string tag = "");

    void deleteObjectDeffered(Handle<RenderObject> objectId);
};

}
