#include "GUIContext.hpp"

GUIContext::GUIContext(Rect windowRect) : screen(windowRect)
{

}

void GUIContext::createWindow(std::shared_ptr<GUIWindow> window) {
    window->id = windows_.size();
    window->zOrder = windowOrder.size();

    windows_.push_back(window);
    windowOrder.push_back(window.get());
}

void GUIContext::closeWindow(std::shared_ptr<GUIWindow> window) {
    windows_.erase(windows_.begin()+window->id);
    windowOrder.erase(windowOrder.begin()+window->zOrder);
    reorderWindows();
}

void GUIContext::keyEvent(KeyEvent& keyEvent) {

}

void GUIContext::mouseMoveEvent(MouseMoveEvent& mouseMoveEvent) {
    GUIWindow* window = getWindowByCursor(mouseMoveEvent.pos);

    if(window && mouseMoveEvent.button >= 0) {
        window->pos.x += (int)mouseMoveEvent.delta.x;
        window->pos.y += (int)mouseMoveEvent.delta.y;
    }
}

void GUIContext::mouseClickEvent(MouseClickEvent& mouseClickEvent) {
    
}

void GUIContext::reorderWindows() {
    for(int i = 0; i < windowOrder.size(); i++) windowOrder[i]->zOrder=i;
}

void GUIContext::reorderWindow(GUIWindow* window) {
    windowOrder.erase(windowOrder.begin()+window->zOrder);
    windowOrder.push_back(window);
    reorderWindows();
}

GUIWindow* GUIContext::getWindowByCursor(Vec2 pos) {

    for(auto it = windows_.rbegin(); it != windows_.rend(); ++it) {

        auto& window = *it;

        if(pos.x >= window->pos.x &&
           pos.x <= window->pos.x + window->window.width &&
           pos.y >= window->pos.y &&
           pos.y <= window->pos.y + window->window.height)
        {
            return window.get();
        }
    }

    return nullptr;
}

void GUIEventListener::listen() {
    // MOUSE CLICK EVENT //
    bool mouseButtonPressed = false;
    if(Events::jclicked(GLFW_MOUSE_BUTTON_LEFT)) {
        mouseClickEvent.button = MouseButtons::Left;
        mouseClickEvent.mouseX = Events::x;
        mouseClickEvent.mouseY = Events::y;
        mouseButtonPressed = true;
    }
    else if(Events::jclicked(GLFW_MOUSE_BUTTON_RIGHT)) {
        mouseClickEvent.button = MouseButtons::Right;
        mouseClickEvent.mouseX = Events::x;
        mouseClickEvent.mouseY = Events::y;
        mouseButtonPressed = true;
    }
    else if(Events::jclicked(GLFW_MOUSE_BUTTON_MIDDLE)) {
        mouseClickEvent.button = MouseButtons::Middle;
        mouseClickEvent.mouseX = Events::x;
        mouseClickEvent.mouseY = Events::y;
        mouseButtonPressed = true;
    }
    if(mouseButtonPressed) context->mouseClickEvent(mouseClickEvent);

    // MOUSE MOVE EVENT //
    if(Events::clicked(GLFW_MOUSE_BUTTON_LEFT)) {
        mouseMoveEvent.button = MouseButtons::Left;
    }
    else if(Events::clicked(GLFW_MOUSE_BUTTON_RIGHT)) {
        mouseMoveEvent.button = MouseButtons::Right;
    }
    else if(Events::clicked(GLFW_MOUSE_BUTTON_MIDDLE)) {
        mouseMoveEvent.button = MouseButtons::Middle;
    } else {
        mouseMoveEvent.button = -1;
    }
    mouseMoveEvent.pos.x = Events::x;
    mouseMoveEvent.pos.y = Events::y;
    mouseMoveEvent.delta.x = (int)Events::x - lastMousePos.x;
    mouseMoveEvent.delta.y = (int)Events::y - lastMousePos.y;
    context->mouseMoveEvent(mouseMoveEvent);

    lastMousePos.x = mouseMoveEvent.pos.x;
    lastMousePos.y = mouseMoveEvent.pos.y;

    // KEY EVENT //
}