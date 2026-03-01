#pragma once

#include <string>
#include <vector>

class Text;

namespace myvk 
{

class MeshInstance {
public:
    struct Vertex {
        float x, y, z;
        float u, v;
        float r, g, b, s;
    };

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    uint32_t indCount = 0;
    
	MeshInstance(uint32_t reservedVertices, uint32_t reservedIndices = 0) {
        vertices.reserve(reservedVertices);
        if(reservedIndices) indices.reserve(reservedIndices);
    };
    MeshInstance() {};
	~MeshInstance() {};

    MeshInstance(const MeshInstance&) = delete;
	MeshInstance& operator=(const MeshInstance&) = delete;
};

class TextMesh : public MeshInstance {
    Text* text = nullptr;
    std::u32string meshContent;

    int pen_x = 0;
    int pen_y = 0;

    void buildVertices();
public:
    TextMesh(Text* text);

    void build();
};

}