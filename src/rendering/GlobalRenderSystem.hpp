#pragma once

#include "RenderSystem.hpp"

class Camera;

namespace myvk {

class GlobalRenderSystem : public RenderSystem {
private:
    GlobalUbo ubo{};

    void createGlobalLayouts();
public:
    GlobalRenderSystem(Device &device, VkRenderPass renderPass, DescriptorPoolManager* descriptorPool, FrameInfo& frame);

    void renderGlobal(Camera* camera);
};

}