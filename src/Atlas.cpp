#include "Atlas.hpp"
#include "model/Texture.hpp"
#include <iostream>

AtlasDescriptor::AtlasDescriptor(int atlasWidth, int atlasHeight, int padding_x, int padding_y) :
    width(atlasWidth), 
    height(atlasHeight),
    padding_x(padding_x),
    padding_y(padding_y)
{
    nodes.resize(atlasWidth);
    stbrp_init_target(&context, atlasWidth, atlasHeight, nodes.data(), atlasWidth);
}

void AtlasDescriptor::insert(Texture2D* texture) {
    rects.push_back({stbrp_rect{index, texture->width + padding_x, texture->height + padding_y}});
    images.push_back(texture);

    index++;
}

void AtlasDescriptor::packAll() {
    stbrp_pack_rects(&context, rects.data(), rects.size());
}

stbrp_rect AtlasDescriptor::pack(Texture2D* texture) {
    stbrp_rect rect{index, texture->width + padding_x, texture->height + padding_y*2};
    stbrp_pack_rects(&context, &rect, 1);
    index++;
    return rect;
}

void AtlasBitmap::create(AtlasDescriptor& atlasDesc) {
    if(!pixels) {
        tiles.resize(atlasDesc.width);
        width  = atlasDesc.width;
        height = atlasDesc.height;
        pixels = new uint8_t[atlasDesc.width*atlasDesc.height*4];
        channels = 4;
    }
    padding_x = atlasDesc.padding_x;
    padding_y = atlasDesc.padding_y;

    pasteImages(atlasDesc.rects, atlasDesc.images);
}

void AtlasBitmap::addPadding(const stbrp_rect& rect) {
    uint32_t srcW = rect.w - padding_x;
    uint32_t srcH = rect.h - padding_y*2;

    // VERTICAL BOTTOM PADDING //
    const uint8_t* bottomToCopy = pixelPtr(rect.x, rect.y + srcH-1 + padding_y);
    for (int i = 0; i < padding_y; i++)
    {
        memcpy(
            pixelPtr(rect.x, rect.y + srcH + padding_y + i),
            bottomToCopy,
            srcW * channels
        );
    }

    // VERTICAL UPPER PADDING //
    const uint8_t* upToCopy = pixelPtr(rect.x, rect.y + padding_y);
    for (int i = 0; i < padding_y; i++)
    {
        memcpy(
            pixelPtr(rect.x, rect.y + i),
            upToCopy,
            srcW * channels
        );
    }

    // HORIZONTAL RIGHT PADDING //
    for (int i = 0; i < srcH; i++) {
        uint32_t* dst =
            reinterpret_cast<uint32_t*>(
                pixelPtr(rect.x + srcW,
                        rect.y + i));

        uint32_t* src =
            reinterpret_cast<uint32_t*>(
                pixelPtr(rect.x + srcW - 1,
                        rect.y + i));

        for (uint32_t p = 0; p < padding_x; p++) {
            dst[p] = *src;
    }
}
}

void AtlasBitmap::pasteImages(std::vector<stbrp_rect> &rects, std::vector<Texture2D*> &images) {
    for(int i = 0; i < rects.size(); i++) extend(rects[i], images[i]);
}

void AtlasBitmap::extend(const stbrp_rect& rect, Texture2D* texture) {
    if (!rect.was_packed) return;

    pasteGrayscale(texture, rect.x, rect.y + padding_y);
    addPadding(rect);

    Tile& t = tiles[rect.id];

    t.width  = texture->width;
    t.height = texture->height;
    t.u1 = (float)rect.x / width;
    t.v1 = (float)(rect.y + padding_y) / height;
    t.u2 = (float)(rect.x + (rect.w - padding_x)) / width;
    t.v2 = (float)(rect.y + (rect.h - padding_y)) / height;
}

const Tile& AtlasBitmap::getTile(int index) const {
    return tiles[index];
}