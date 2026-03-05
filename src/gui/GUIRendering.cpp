#include "GUIRendering.hpp"

namespace myvk {

void GUIMeshMaker::makeMesh(GUIWindowRender* windowRender, GUIWindow* contextWindow) {
    const uint32_t windowContentsSize = contextWindow->contents.size();
    if(windowRender->models.size() != windowContentsSize) {

        windowRender->meshInstances.resize(windowContentsSize);
        windowRender->models.resize(windowContentsSize);
        
    }
}

GUIRender::GUIRender(GUIContext* context) : context(context) {

}

void GUIRender::update() {
    const uint32_t contextWindowsSize = context->windows_.size();
    if(contextWindowsSize != windowToRender.size()) windowToRender.resize(contextWindowsSize);

    for(int i = 0; i < contextWindowsSize; i++) {
        GUIWindowRender& windowRender = windowToRender[i];
        GUIWindow* window = context->windows_[i].get();

        if(windowRender.version != window->version) {
            meshMaker.makeMesh(&windowRender, window);
        }
    }
}

}