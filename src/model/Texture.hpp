#pragma once

#include <string>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

class Texture2D {
public:
    std::string path;
    
    Texture2D(uint8_t* ptr, int width, int height, int channels);
    Texture2D(std::string path);
    Texture2D(FT_GlyphSlot g);

    Texture2D() {};
    ~Texture2D() ;
    
    const uint8_t* raw() {return pixels;};
    
    int width = 0;
    int height = 0;
    int channels = 0;
    uint8_t* pixels = nullptr;

    inline uint8_t* pixelPtr(uint32_t x, uint32_t y)
    {
        return pixels + (y * width + x) * channels;
    }

    void paste(Texture2D* src, uint32_t offsetX, uint32_t offsetY);
    void pasteGrayscale(Texture2D* src, uint32_t offsetX, uint32_t offsetY);

    void save(const std::string& path) const;
protected:
    
};