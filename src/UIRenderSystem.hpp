#pragma once

#include "RenderSystem.hpp"
#include "gui/GUIRendering.hpp"

namespace myvk {

class UIRenderSystem : public RenderSystem {
private:
    std::unique_ptr<GUIRender> guiRender;
public:
    UIRenderSystem(
        Device &device, 
        VkRenderPass renderPass, 
        std::vector<VkDescriptorSetLayout> layouts, 
        GUIContext* guiContext);

    void renderGUI(FrameInfo& frameInfo);
};

}