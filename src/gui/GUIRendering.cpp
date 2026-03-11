#include "GUIRendering.hpp"
#include "../model/Mesh.hpp"
#include "../model/GPUMesh.hpp"

namespace myvk {

void GUIWindowRender::buildMesh() {
    auto mesh = std::make_shared<Mesh>();
    MeshTools::Quad quad;

    quad.r = window->windowColor.r; 
    quad.g = window->windowColor.g; 
    quad.b = window->windowColor.b; 
    quad.a = window->windowColor.a;
    quad.x2 = (float)window->window.width;
    quad.y2 = (float)window->window.height;

    MeshTools::createQuad(mesh.get(), quad);

    // Footer  //
    quad = MeshTools::Quad();

    quad.r = window->footerColor.r; 
    quad.g = window->footerColor.g; 
    quad.b = window->footerColor.b; 
    quad.a = window->footerColor.a;
    quad.y1 = (float)(window->window.height - window->footer.height);
    quad.x2 = (float)window->window.width;
    quad.y2 = (float)window->window.height;
    MeshTools::createQuad(mesh.get(), quad);

    meshWindow = std::make_unique<MeshObject>(mesh, nullptr);
    windowObjectId = scene->registerObject(meshWindow.get());
    
    meshWindow->transform.translate({window->pos.x, window->pos.y, 0.0f});
    scene->updateTransform(windowObjectId, meshWindow->transform.matrix());
}

GUIRenderer::GUIRenderer(GUIContext* context) : context(context) {

}

void GUIRenderer::buildDrawList() {
    for(auto& window : windowToRender) renderSystem->addToDrawList(window->windowObjectId);
}

void GUIRenderer::fetchContext() {
    for(std::shared_ptr<GUIWindow>& window : context->windows()) {
        auto windowRender = std::make_unique<GUIWindowRender>();
        windowRender->window = window.get();
        windowRender->scene = renderScene;
        //windowRender->version = window->version;
        windowRender->buildMesh();

        windowToRender.push_back(std::move(windowRender));
    }
}

}
