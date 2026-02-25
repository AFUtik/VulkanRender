#include "Atlas.hpp"
#include "model/Texture.hpp"
#include <iostream>

AtlasDescriptor::AtlasDescriptor(int atlasWidth, int atlasHeight) :
    width(atlasWidth), 
    height(atlasHeight) 
{
    nodes.resize(atlasWidth);
    stbrp_init_target(&context, atlasWidth, atlasHeight, nodes.data(), atlasWidth);
}

void AtlasDescriptor::insert(Texture2D* texture) {
    rects.push_back({stbrp_rect{index, texture->width, texture->height}});
    images.push_back(texture);

    index++;
}

void AtlasDescriptor::packAll() {
    packedFull = stbrp_pack_rects(&context, rects.data(), rects.size());
    packed = true;
}

AtlasBitmap::AtlasBitmap(AtlasDescriptor& atlasDesc) : Texture2D() {
    if(!atlasDesc.packed) atlasDesc.packAll();
    if(!atlasDesc.packedFull) {std::cerr << "Failed to pack rect (try to resize atlas)" << std::endl;}

    pasteImages(atlasDesc.rects, atlasDesc.images);
}

void AtlasBitmap::pasteImages(std::vector<stbrp_rect> &rects, std::vector<Texture2D*> &images) {
    for(auto &rect : rects) {
        if(rect.was_packed) this->paste(images[rect.id], rect.x, rect.y);
    }
}