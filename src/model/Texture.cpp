#include "Texture.hpp"

#include <stb_image.h>
#include "stb_image_write.h"

#include <iostream>

Texture2D::Texture2D(std::string path) : path(path) {
    pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    channels = 4;
    if (!pixels) {
    #ifndef _WIN64
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working dir: %s\n", cwd);
        } else {
            printf("getcwd() error\n");
        }
    #endif
        printf("Error loading Texture from '%s'\n", path.c_str());
        exit(1);
    }
}

Texture2D::Texture2D(FT_GlyphSlot g)
{
    width = g->bitmap.width;
    height = g->bitmap.rows;
    channels = 1;
    pixels = new uint8_t[width * height];

    for (int y = 0; y < height; ++y)
        memcpy(pixels  + y * width, g->bitmap.buffer + y * g->bitmap.pitch, width);
}

Texture2D::Texture2D(uint8_t* ptr, int width, int height, int channels) :
    pixels(ptr), width(width), height(height), channels(channels)
{
    
}

Texture2D::~Texture2D() {
    if(pixels) stbi_image_free(pixels);
}

void Texture2D::paste(Texture2D *src, uint32_t offsetX, uint32_t offsetY) {
    if(!src || src->height > this->height || src->width > this->width || src->channels != this->channels) {
        std::cerr << "Failed to paste an image: src is bigger or channels are different" << std::endl;
        return;
    }
    if(offsetX + src->width > this->width || offsetY + src->height > this->height) {
        std::cerr << "Failed to paste an image: src is out of destination image" << std::endl;
        return;
    }

    std::cout << (src == nullptr) << " " << offsetX << " " << offsetY << std::endl;

    const uint32_t bytesPerPixel = channels;
    for (uint32_t y = 0; y < src->height; y++)
    {
        uint8_t* dstRow = this->pixels +
            ((offsetY + y) * this->width + offsetX) * bytesPerPixel;
        uint8_t* srcRow = src->pixels +
            (y * src->width) * bytesPerPixel;
        memcpy(dstRow, srcRow, src->width * bytesPerPixel);
    }
}

void Texture2D::pasteGrayscale(Texture2D* src,
                               uint32_t offsetX,
                               uint32_t offsetY)
{
    if (!src) return;

    if (src->channels != 1 || this->channels != 4)
    {
        std::cerr << "Invalid format: need R8 -> RGBA\n";
        return;
    }

    if (offsetX + src->width  > this->width ||
        offsetY + src->height > this->height)
    {
        std::cerr << "Out of bounds\n";
        return;
    }

    for (int y = 0; y < src->height; ++y)
    {
        for (int x = 0; x < src->width; ++x)
        {
            uint8_t gray = *src->pixelPtr(x, y);

            uint32_t dstIndex =
                ((offsetY + y) * this->width + (offsetX + x)) * 4;

            // RGB
            this->pixels[dstIndex + 0] = 255;
            this->pixels[dstIndex + 1] = 255;
            this->pixels[dstIndex + 2] = 255;
            // Alpha
            this->pixels[dstIndex + 3] = gray;
        }
    }
}

void Texture2D::save(const std::string& path) const {
    if(width == 0 || height == 0 || channels == 0 || !pixels) {
        std::cerr << " Can't save image: data or parameters is empty" << std::endl;
        return;
    } 

    stbi_write_png(
        path.c_str(),
        width,
        height,
        channels,
        pixels,
        width * channels  // stride (bytes per row)
    );
}