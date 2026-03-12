
#pragma once

#include "GUIContext.hpp"
#include "../RenderSystem.hpp"
#include "../Fonts.hpp"

#include <memory>

namespace myvk {

class GUIWindowRender {
public:
    uint32_t posX, posY;
    uint32_t version = 0;

    std::shared_ptr<MeshObject> meshObject;
    std::shared_ptr<MeshObject> titleMeshObject;

    std::shared_ptr<Material> fontMaterial;
    std::unique_ptr<Font> font;
    Text windowName;

    inline bool isVisible() {return window->visible;};

    ~GUIWindowRender();
private:
    void prepareMaterials();
    void prepareMeshes();
    void sync();

    GUIWindow* window;
    RenderScene* scene;
    FontHandler* fontHandler;

    friend class GUIRenderer;
};

class GUIRenderer : public ObjectRenderer {
public:
    std::vector<std::unique_ptr<GUIWindowRender>> windowToRender;

    GUIContext* context = nullptr;
    FontHandler* fontHandler = nullptr;
    
    GUIRenderer(GUIContext* guiContext, FontHandler* fontHandler);

    void syncAll();
    void fetchContext();
    void buildDrawList() override;
};

} 