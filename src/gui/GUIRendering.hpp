
#pragma once

#include "GUIContext.hpp"
#include "../RenderSystem.hpp"

#include <memory>

namespace myvk {

class GUIWindowRender {
public:
    uint32_t version = 0;

    std::unique_ptr<MeshObject> meshWindow;
    Handle<RenderObject> windowObjectId;

    inline bool isVisible() {return window->visible;};
private:
    void buildMesh();

    GUIWindow* window;
    RenderScene* scene;

    friend class GUIRenderer;
};

class GUIRenderer : public ObjectRenderer {
public:
    std::vector<std::unique_ptr<GUIWindowRender>> windowToRender;

    GUIContext* context = nullptr;
    
    GUIRenderer(GUIContext* guiContext);

    void fetchContext();
    void buildDrawList() override;
};

} 