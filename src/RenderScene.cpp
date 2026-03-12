#include "RenderScene.hpp"
#include <iostream>

namespace myvk {

RenderScene::RenderScene(Device& device, DescriptorPoolManager& pool, DescriptorSetLayout& materialLayout) : device(device), pool(pool), materialLayout(materialLayout) 
{   
    createEmptyMaterial();
}

void RenderScene::createEmptyMaterial() {
    std::unique_ptr<uint8_t[]> whitePixel = std::make_unique<uint8_t[]>(4);
	whitePixel[0] = 255;
	whitePixel[1] = 255;
	whitePixel[2] = 255;
	whitePixel[3] = 255;

    Material material {std::make_shared<Texture2D>(std::move(whitePixel), 1, 1, TextureChannels::RGBA)};
    defMaterialHandle = getMaterialHandle(&material);
}

Handle<RenderObject> RenderScene::registerObject(MeshObject* meshObject) {
    RenderObject render;
    render.mesh = getMeshHandle(meshObject->getMesh());
    render.material = getMaterialHandle(meshObject->getMaterial());
    render.meshObject = meshObject;
    render.scissorWidth = meshObject->getScissorWidth();
    render.scissorHeight = meshObject->getScissorHeight();

    Handle<RenderObject> handle;
    handle.handle = renderables.create(render);

    meshObject->setRenderScene(this);
    meshObject->setRenderId(handle.handle);
    meshObject->tryFree();
    attach(handle);

    return handle;
}

void RenderScene::attach(Handle<RenderObject> objectId) {
    RenderObject& renderObject = renderables[objectId.handle];
    renderObject.drawIndex = Handle<uint32_t>(drawList.size());
    drawList.push_back(objectId);
}

void RenderScene::detach(Handle<RenderObject> objectId) {
    RenderObject& renderObject = renderables[objectId.handle];
    if(renderObject.drawIndex.valid()) {
        drawList.erase(drawList.begin() + renderObject.drawIndex.handle);
    };
}

void RenderScene::updateMeshData(const MeshObject* meshObject, Handle<RenderObject> objectId) {
    RenderObject& renderObject = renderables[objectId.handle];
    const Mesh* mesh = meshObject->getMesh();

    if(renderObject.mesh.valid()) {
        renderObject.mesh = getMeshHandle(mesh);
    }
    else {
        DrawMesh& currentMesh = meshes[renderObject.mesh.handle];
        currentMesh.gpuData->update(mesh);
    }
}

void RenderScene::deleteObjectDeffered(Handle<RenderObject> handle) {
    if(!handle.valid()) {
        std::cerr << "RenderScene: Failed to delete RenderObject by this handle. The handle is invalid" << std::endl;
        return;
    }

    RenderObject& renderObject = renderables[handle.handle];

    deleteMeshDeffered(renderObject.mesh);
    deleteMaterialDeffered(renderObject.material);

    renderables.remove(handle.handle);
}

void RenderScene::deleteMeshDeffered(Handle<DrawMesh> &handle) {
    if(!handle.valid()) {
        std::cerr << "RenderScene: Failed to delete Mesh by this handle. The handle is invalid" << std::endl;
        return;
    }

    DrawMesh& drawMesh = meshes[handle.handle];
    drawMesh.refCount--;

    if(drawMesh.refCount == 0) {
        std::cout << "RenderScene: Mesh " << drawMesh.original << " is deleted from GPU memory" << std::endl;

        auto it = meshConvert.find(drawMesh.original);
        if(it != meshConvert.end()) meshConvert.erase(it);

        meshes.remove(handle.handle);
        handle.destroy();
    }
}

void RenderScene::deleteMaterialDeffered(Handle<DrawMaterial> &handle) {
    if(!handle.valid()) {
        std::cerr << "RenderScene: Failed to delete Material by this handle. The handle is invalid" << std::endl;
        return;
    }
    else if(handle.handle == defMaterialHandle.handle) return;

    DrawMaterial& drawMaterial = materials[handle.handle];
    drawMaterial.refCount--;

    if(drawMaterial.refCount == 0) {
        auto it = materialConvert.find(drawMaterial.original);
        if(it != materialConvert.end()) materialConvert.erase(it);

        materials.remove(handle.handle);
        handle.destroy();
    }
}

Handle<DrawMesh> RenderScene::getMeshHandle(const Mesh *mesh) {
    auto it = meshConvert.find(mesh);
    if(it != meshConvert.end()) {
        DrawMesh& drawMesh = meshes[it->second.handle];
        drawMesh.refCount++;
        
        return it->second;
    }
    else {
        if(!mesh) return Handle<DrawMesh>();

        DrawMesh drawMesh;
        drawMesh.original = mesh;
        drawMesh.gpuData = std::make_unique<GPUMesh>(device, mesh);
        drawMesh.firstVertex = 0;
        drawMesh.firstIndex  = 0;
        drawMesh.indexCount  = mesh->indices.size();
        drawMesh.vertexCount = mesh->vertices.size();
        drawMesh.refCount++;

        Handle<DrawMesh> handle;
        handle.handle = meshes.create(std::move(drawMesh));

        meshConvert.emplace(mesh, handle);

        return handle;
    }
};

Handle<DrawMaterial> RenderScene::getMaterialHandle(const Material* material) {
    if(material == nullptr) return defMaterialHandle;

    auto it = materialConvert.find(material);
    if(it != materialConvert.end()) {
        return it->second;
    }
    else {
        if(!material->albedo) return Handle<DrawMaterial>();

        auto texture  = std::make_shared<GPUTexture>(device, material->albedo.get(), material->albedoFilter);

        DrawMaterial drawMaterial;
        drawMaterial.original = material;
        drawMaterial.gpuData  = std::make_unique<GPUMaterial>(pool, materialLayout, texture);
        drawMaterial.refCount++;

        Handle<DrawMaterial> handle;
        handle.handle = materials.create(std::move(drawMaterial));

        materialConvert.emplace(material, handle);

        return handle;
    }
}

};

/*

void RenderScene::updateObject(const MeshObject* meshObject, Handle<RenderObject> objectId) {
    RenderObject& renderObject = renderables[objectId.handle];

    const Mesh* mesh = meshObject->getMesh();
    DrawMesh& currentMesh = meshes[renderObject.mesh.handle];

    if(currentMesh.original != mesh) {
        Handle<DrawMesh> newHandle = getMeshHandle(mesh);
        deleteMeshDeffered(renderObject.mesh);
        renderObject.mesh = newHandle;
    }

    const Material* material = meshObject->getMaterial();
    DrawMaterial& currentMaterial = materials[renderObject.material.handle];

    if(currentMaterial.original != material) {
        Handle<DrawMaterial> newHandle = getMaterialHandle(material);
        deleteMaterialDeffered(renderObject.material);
        renderObject.material = newHandle;
    }
}

*/