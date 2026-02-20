#include "Model.hpp"
#include "Device.hpp"

#include <cassert>
#include <cstring>

namespace myvk {
	void Mesh::createBuffers(Device& device) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		if(vertexCount >= 3) {
			VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
			Buffer stagingBuffer(
				device,
				bufferSize,
				1,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY
			);
			stagingBuffer.map();
			stagingBuffer.writeToBuffer(vertices.data(), bufferSize);
			stagingBuffer.unmap();

			vertexBuffer = std::make_unique<Buffer>(
				device,
				bufferSize,
				1,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				0,
				VMA_MEMORY_USAGE_GPU_ONLY
			);
			device.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
			if(freeDataOnUpload) vertices.clear();
		}
		indexCount = static_cast<uint32_t>(indices.size());
		if(vertexCount >= 3 && indexCount) {
				hasIndexBuffer = indexCount > 0;

			if (!hasIndexBuffer) {
				return;
			}

			VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
			Buffer stagingBuffer(
				device,
				bufferSize,
				1,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY
			);
			stagingBuffer.map();
			stagingBuffer.writeToBuffer(indices.data(), bufferSize);
			stagingBuffer.unmap();

			indexBuffer = std::make_unique<Buffer>(
				device,
				bufferSize,
				1,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				0,
				VMA_MEMORY_USAGE_GPU_ONLY
			);
			device.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
			if(freeDataOnUpload) indices.clear();
		}
		uploaded = true;
	}

	void Mesh::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = 0; 

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, u);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, r);

		return attributeDescriptions;
	}
}