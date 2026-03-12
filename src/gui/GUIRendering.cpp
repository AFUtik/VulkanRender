#include "GUIRendering.hpp"
#include "../model/Mesh.hpp"
#include "../model/GPUMesh.hpp"
#include <memory>


namespace myvk {

void GUIWindowRender::prepareMeshes() {
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

    meshObject = std::make_shared<MeshObject>();
    meshObject->setMesh(mesh);

    auto titleMesh  = std::make_shared<TextMesh>(&windowName, font.get());
    titleMeshObject = std::make_shared<MeshObject>();
    titleMeshObject->setMesh(titleMesh);
    titleMeshObject->setMaterial(fontMaterial);
    titleMeshObject->setScissor(50, 50);

    scene->registerObject(meshObject.get());
    scene->registerObject(titleMeshObject.get());
}

void GUIWindowRender::prepareMaterials() {
    FontSample* sample = fontHandler->getSample("Minecraft");
    FontData fontData;
    fontData.pxHeight = 25;
    
    font = std::make_unique<Font>(sample, fontData); 

    fontMaterial = std::make_shared<Material>(font->fontData.bitmap);
    fontMaterial->albedoFilter = TextureFilter::Nearest;
    
    windowName = Text(U"Simple window");
}

void GUIWindowRender::sync() {
    if(window->pos.x != posX || window->pos.y != posY) {
        meshObject->transform.setPosition({window->pos.x, window->pos.y, 0.0f});
        titleMeshObject->transform.setPosition({window->pos.x + 20, window->pos.y + (window->window.height - window->footer.height) + 7, 0.0f});

        posX = window->pos.x;
        posY = window->pos.y;
    }
}

GUIWindowRender::~GUIWindowRender() {
    
}

GUIRenderer::GUIRenderer(GUIContext* context, FontHandler* fontHandler) : context(context), fontHandler(fontHandler) {

}

void GUIRenderer::syncAll() {
    for(auto& window : windowToRender) window->sync();
}

void GUIRenderer::buildDrawList() {
    for(auto& window : windowToRender) renderSystem->addToDrawList(Handle<RenderObject>(window->meshObject->getRenderId()));
}

void GUIRenderer::fetchContext() {
    for(std::shared_ptr<GUIWindow>& window : context->windows()) {
        auto windowRender = std::make_unique<GUIWindowRender>();
        windowRender->window = window.get();
        windowRender->scene = renderScene;
        windowRender->fontHandler = fontHandler;

        //windowRender->version = window->version;

        windowRender->prepareMaterials();
        windowRender->prepareMeshes();

        windowToRender.push_back(std::move(windowRender));
    }
}

}
