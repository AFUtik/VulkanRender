#pragma once

#include "model/Texture.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

#include <stb_rect_pack.h>

// Describes texture in atlas, stores index and uv coord. //
struct Tile {
    float u1 = 0.0f, v1 = 0.0f;
    float u2 = 0.0f, v2 = 0.0f;
    uint32_t width = 0, height = 0;
    int padding = 0;
};

// Packs all textures and can be transfered to AtlasBitmap for pasting all textures;
class AtlasDescriptor {
    stbrp_context context;
    std::vector<stbrp_node> nodes;
    std::vector<stbrp_rect> rects;
    std::vector<Texture2D*> images;

    int padding_x = 0;
    int padding_y = 0;
    int index = 0;

    friend class AtlasBitmap;
public:
    bool packed = false;
    bool packedFull = false;

    uint32_t width, height;
    AtlasDescriptor(int atlasWidth, int atlasHeight, int padding_x = 0, int padding_y = 0);

    void insert(Texture2D* texture);
    void packAll();
    
    stbrp_rect pack(Texture2D* texture);
};

class AtlasBitmap : public Texture2D {
private:
    int padding_x = 0;
    int padding_y = 0;
    std::vector<Tile> tiles;

    void addPadding(const stbrp_rect& rect);
    void pasteImages(std::vector<stbrp_rect> &rects, std::vector<Texture2D*> &images);
public:
    void create(AtlasDescriptor& desc);
    void extend(const stbrp_rect& rect, Texture2D* texture);

    const Tile& getTile(int index) const;
};