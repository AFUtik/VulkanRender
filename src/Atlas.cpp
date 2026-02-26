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
    packedFull = stbrp_pack_rects(&context, rects.data(), rects.size());
    packed = true;
}

AtlasBitmap::AtlasBitmap(AtlasDescriptor& atlasDesc) : Texture2D() {
    width  = atlasDesc.width;
    height = atlasDesc.height;
    pixels = new uint8_t[atlasDesc.width*atlasDesc.height*4];
    channels = 4;

    padding_x = atlasDesc.padding_x;
    padding_y = atlasDesc.padding_y;

    if(!atlasDesc.packed) atlasDesc.packAll();
    if(!atlasDesc.packedFull) {std::cerr << "Failed to pack rect (try to resize atlas)" << std::endl;}

    tiles.resize(atlasDesc.rects.size());
    pasteImages(atlasDesc.rects, atlasDesc.images);
}

void AtlasBitmap::addPadding(stbrp_rect& rect, Texture2D* texture) {
    // VERTICAL BOTTOM PADDING //
    const uint8_t* bottomToCopy = texture->pixelPtr(0, texture->height-1);
    for (int i = 0; i < padding_y; i++)
    {
        memcpy(
            pixelPtr(rect.x, rect.y + (rect.h-padding_y) + i),
            bottomToCopy,
            (texture->width) * channels
        );
    }

    // HORIZONTAL RIGHT PADDING //
    for (int i = 0; i < rect.h; i++) {
        uint32_t* dst =
            reinterpret_cast<uint32_t*>(
                pixelPtr(rect.x + texture->width, i));

        uint32_t* src =
            reinterpret_cast<uint32_t*>(
                pixelPtr(rect.x + texture->width - 1, i));

        for (uint32_t p = 0; p < padding_x; p++)
        {
            dst[p] = *src;
        }
    }
}

void AtlasBitmap::pasteImages(std::vector<stbrp_rect> &rects, std::vector<Texture2D*> &images) {
    for(int i = 0; i < rects.size(); i++) {
        if (!rects[i].was_packed)
            continue;

        paste(images[i], rects[i].x, rects[i].y);
        addPadding(rects[i], images[i]);

        Tile& t = tiles[i];

        t.u1 = (float)rects[i].x / width;
        t.v1 = (float)rects[i].y / height;
        t.u2 = (float)(rects[i].x + (rects[i].w - padding_x)) / width;
        t.v2 = (float)(rects[i].y + (rects[i].h - padding_y)) / height;
    }
}

const Tile& AtlasBitmap::getTile(int index) const {
    return tiles[index];
}