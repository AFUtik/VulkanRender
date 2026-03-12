#include "MeshObject.hpp"
#include "../RenderScene.hpp"

namespace myvk {

MeshObject::~MeshObject() {
    if(renderScene) renderScene->deleteObjectDeffered(Handle<RenderObject>(renderId));
}

void MeshObject::attachToScene() {
    if(Handle<RenderObject>::valid(renderId)) {
        renderScene->attach(Handle<RenderObject>(renderId));
    }
}

void MeshObject::setMesh(std::shared_ptr<Mesh> mesh) {
    this->mesh = mesh;
    if(Handle<RenderObject>::valid(renderId)) {
        renderScene->updateMeshData(this, Handle<RenderObject>(renderId));
    }
}

void MeshObject::setMaterial(std::shared_ptr<Material> material) {
    this->material = material;
    // renderScene->updateMeshData(this, Handle<RenderObject>(renderId));
}

void MeshObject::updateMesh() {
    renderScene->updateMeshData(this, Handle<RenderObject>(renderId));
}

void MeshObject::updateMaterial() {
    // renderScene->updateMeshData(this, Handle<RenderObject>(renderId));
}

}