#pragma once

#include "RenderSystem.hpp"

// vulkan dep.
#include "../vk/Descriptors.hpp"
#include "../vk/FrameInfo.hpp"

//#include "gui/GUIRendering.hpp"
//#include "Fonts.hpp"

namespace myvk {

class UIRenderSystem : public RenderSystem {
private:
    void createUILayouts();
public:
    UIRenderSystem(Device &device, VkRenderPass renderPass, DescriptorPoolManager* descriptorPool, FrameInfo& frame);
};

}