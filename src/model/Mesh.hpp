#pragma once

#include <vector>

namespace myvk 
{

struct MeshInstance {
public:
    struct Vertex {
        float x, y, z;
        float u, v;
        float r, g, b, s;
    };

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    
	MeshInstance(uint32_t reservedVertices, uint32_t reservedIndices) {
        vertices.reserve(reservedVertices);
        indices.reserve(reservedIndices);
    };
    MeshInstance() {};
	~MeshInstance() {};

    MeshInstance(const MeshInstance&) = delete;
	MeshInstance& operator=(const MeshInstance&) = delete;
};

}