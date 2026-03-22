#include "RenderService.hpp"
#include <iostream>

namespace myvk {

RenderService::RenderService(Device& device, DescriptorPoolManager& pool, DescriptorSetLayout& materialLayout) : device(device), pool(pool), materialLayout(materialLayout) 
{   
    createEmptyMaterial();
}

void RenderService::createEmptyMaterial() {
    std::unique_ptr<uint8_t[]> whitePixel = std::make_unique<uint8_t[]>(4);
	whitePixel[0] = 255;
	whitePixel[1] = 255;
	whitePixel[2] = 255;
	whitePixel[3] = 255;

    Material material {std::make_shared<Texture2D>(std::move(whitePixel), 1, 1, TextureChannels::RGBA)};
    defMaterialHandle = createMaterialHandle(&material);
}

void RenderService::render(Handle<RenderObject> id, const glm::mat4& transform) {
    drawList.push_back( DrawCommand { transform, id.handle });
}

Handle<RenderObject> RenderService::registerRenderObject(RenderObject object) {
    Handle<RenderObject> handle;
    handle.handle = renderables.create(object);
    return handle;
}

void RenderService::deleteObjectDeffered(Handle<RenderObject> handle) {
    if(!handle.valid()) {
        std::cerr << "RenderService: Failed to delete RenderObject by this handle. The handle is invalid" << std::endl;
        return;
    }

    RenderObject& renderObject = renderables[handle.handle];

    deleteMeshDeffered(renderObject.mesh);
    deleteMaterialDeffered(renderObject.material);

    renderables.remove(handle.handle);
}

void RenderService::deleteMeshDeffered(Handle<DrawMesh> &handle) {
    if(!handle.valid()) {
        std::cerr << "RenderService: Failed to delete Mesh by this handle. The handle is invalid" << std::endl;
        return;
    }
    DrawMesh& mesh = meshes[handle.handle];
    if(!mesh.tag.empty()) {
        auto it = tagMeshMap.find(mesh.tag);
        tagMeshMap.erase(it);
    }

    meshes.remove(handle.handle);
    handle.destroy();
}

void RenderService::deleteMaterialDeffered(Handle<DrawMaterial> &handle) {
    if(!handle.valid()) {
        std::cerr << "RenderService: Failed to delete Material by this handle. The handle is invalid" << std::endl;
        return;
    }
    else if(handle.handle == defMaterialHandle.handle) return;
    
    DrawMaterial& material = materials[handle.handle];
    if(!material.tag.empty()) {
        auto it = tagMeshMap.find(material.tag);
        tagMeshMap.erase(it);
    }

    materials.remove(handle.handle);
    handle.destroy();
}

Handle<DrawMesh> RenderService::createMeshHandle(const Mesh *mesh, std::string tag) {
    if(!mesh) return Handle<DrawMesh>();

    DrawMesh drawMesh;

    drawMesh.gpuData = std::make_unique<GPUMesh>(device, mesh);
    drawMesh.firstVertex = 0;
    drawMesh.firstIndex  = 0;
    drawMesh.indexCount  = mesh->indices.size();
    drawMesh.vertexCount = mesh->vertices.size();

    Handle<DrawMesh> handle;
    if(!tag.empty()) {
        tagMeshMap.emplace(tag, handle);
        drawMesh.tag = tag;
    }
    handle.handle = meshes.create(std::move(drawMesh));

    return handle;
};

Handle<DrawMaterial> RenderService::createMaterialHandle(const Material* material, std::string tag) {
    if(material == nullptr) return defMaterialHandle; // Return default material //
    if(!material->albedo) return Handle<DrawMaterial>();

    auto texture  = std::make_shared<GPUTexture>(device, material->albedo.get(), material->albedoFilter);

    DrawMaterial drawMaterial;
    drawMaterial.gpuData  = std::make_unique<GPUMaterial>(pool, materialLayout, texture);

    Handle<DrawMaterial> handle;
    if(!tag.empty()) {
        tagMaterialMap.emplace(tag, handle);
        drawMaterial.tag = tag;
    }
    handle.handle = materials.create(std::move(drawMaterial));
    
    return handle;
}

Handle<DrawMesh> RenderService::getMeshHandle(std::string tag) {
    auto it = tagMeshMap.find(tag);
    if(it != tagMeshMap.end()) {
        return it->second;
    }
    return Handle<DrawMesh>();
}

Handle<DrawMaterial> RenderService::getMaterialHandle(std::string tag) {
    auto it = tagMaterialMap.find(tag);
    if(it != tagMaterialMap.end()) {
        return it->second;
    }
    return Handle<DrawMaterial>();
}


};