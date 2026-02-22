#pragma once

#include "../Buffer.hpp"
#include "../Device.hpp"

#include "Mesh.hpp"

namespace myvk {

class GPUMesh {
private:
	Device& device;
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indexBuffer;
	bool uploaded = false;
	bool hasIndexBuffer = false;
	uint32_t vertexCount;
	uint32_t indexCount;

	friend class RenderSystem;
public:
	std::shared_ptr<MeshInstance> instance;

	GPUMesh(Device& device, std::shared_ptr<MeshInstance> instance);
	~GPUMesh();

	GPUMesh(const GPUMesh&) = delete;
	GPUMesh& operator=(const GPUMesh&) = delete;
	
	void createBuffers(Device& device);
	void bind(VkCommandBuffer commandBuffer) const;
	static std::vector<VkVertexInputBindingDescription>   getBindingDescriptions();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

}