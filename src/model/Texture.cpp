#include "Texture.hpp"

#include <stb_image.h>
#include <iostream>

void Texture2D::load(std::string path) {
    pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
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
