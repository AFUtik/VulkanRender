#include "Texture.hpp"

#include <stb_image.h>

namespace myvk {
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
}