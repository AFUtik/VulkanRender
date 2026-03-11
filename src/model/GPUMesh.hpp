#pragma once

#include "../Buffer.hpp"
#include "../Device.hpp"

#include "Mesh.hpp"

namespace myvk {

enum GPUMeshBufferFlags : uint32_t {
	CreateWithReserve,
	CreateOnGPUMemory
};

class GPUMesh {
private:
	Device& device;
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indexBuffer;
	bool uploaded = false;
	bool hasIndexBuffer = false;
	uint32_t reservedVertexBufferSize = 0;
	uint32_t reservedIndexBufferSize  = 0;
	uint32_t vertexCount;
	uint32_t indexCount;
	uint32_t flags;
	
	void createBuffers(const Mesh* mesh);
	void updateBuffers(const Mesh* mesh);

	friend class RenderSystem;
public:
	GPUMesh(Device& device, const Mesh* mesh, uint32_t flags = (CreateWithReserve | CreateOnGPUMemory));
	~GPUMesh() {};

	GPUMesh(const GPUMesh&) = delete;
	GPUMesh& operator=(const GPUMesh&) = delete;

	void update(const Mesh* mesh);
	
	void draw(VkCommandBuffer commandBuffer) const;
	void bind(VkCommandBuffer commandBuffer) const;

	static std::vector<VkVertexInputBindingDescription>   getBindingDescriptions();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

}