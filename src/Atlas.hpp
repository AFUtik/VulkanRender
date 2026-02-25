#pragma once

#include "model/Texture.hpp"

#include <vector>
#include <memory>

#include <stb_rect_pack.h>

class AtlasDescriptor {
    stbrp_context context;
    std::vector<stbrp_node> nodes;
    std::vector<stbrp_rect> rects;
    std::vector<Texture2D*> images;

    int index = 0;

    friend class AtlasBitmap;
public:
    bool packed = false;
    bool packedFull = false;

    uint32_t width, height;
    AtlasDescriptor(int atlasWidth, int atlasHeight);

    void insert(Texture2D* texture);
    void packAll();
};

class AtlasBitmap : public Texture2D {
private:
    void pasteImages(std::vector<stbrp_rect> &rects, std::vector<Texture2D*> &images);
public:
    AtlasBitmap(AtlasDescriptor& atlasDesc);
};