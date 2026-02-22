#pragma once

#include <vector>

namespace myvk 
{

struct MeshInstance {
public:
    uint32_t meshHadler = UINT32_MAX;
    bool handlerAssigned = false;

    struct Vertex {
        float x, y, z;
        float u, v;
        float r, g, b, s;
    };

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
    bool freeAfterUpload = true;
    
	MeshInstance();
	~MeshInstance();
};

}