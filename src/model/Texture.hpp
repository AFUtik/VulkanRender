#pragma once

#include <string>
#include <memory>

namespace myvk {

class Texture2D {
public:
    std::string path;

    Texture2D();
    ~Texture2D();

    void load(std::string path);
    const uint8_t* raw() {return pixels;};
private:
    uint8_t* pixels = nullptr;
    int width;
    int height;
    int channels;
};

}