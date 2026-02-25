#pragma once

#include <string>
#include <memory>

class Texture2D {
public:
    std::string path;

    Texture2D() {};
    ~Texture2D() ;

    void load(std::string path);
    const uint8_t* raw() {return pixels;};
    
    int width;
    int height;
    int channels;

    void paste(Texture2D* src, uint32_t offsetX, uint32_t offsetY);
private:
    uint8_t* pixels = nullptr;
};