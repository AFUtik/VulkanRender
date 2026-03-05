#pragma once

#include "GUIContext.hpp"

#include "../model/GPUMesh.hpp"
#include "../model/Model.hpp"

#include <memory>

namespace myvk {

struct GUIContentModel {
    uint32_t version = 0;
    std::shared_ptr<Model> model;
};

struct GUIWindowRender {
    uint32_t windowId = 0;
    uint32_t version = 0;
    bool visible = true;
    int zOrder = 0;
    
    std::vector<MeshInstance> meshInstances;
    std::vector<GUIContentModel> models;
};

struct GUIMeshMaker {
    void makeMesh(GUIWindowRender* windowRender, GUIWindow* contextWindow);
};

struct GUIRender {
    std::vector<GUIWindowRender> windowToRender;

    GUIMeshMaker meshMaker;
    GUIContext* context;
    
    GUIRender(GUIContext* guiContext);

    void update();
    
};

}