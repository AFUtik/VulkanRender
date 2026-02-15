#pragma once

#include "Device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>

namespace myvk {
	struct Transform {
		glm::dvec3 pos {0.0f};
		glm::quat  rot {1, 0, 0, 0};
		glm::dvec3 scl {1.0f};
		bool dirty = true;
		glm::mat4 mat{ 1.0f };

		inline void translate(const glm::dvec3& delta) {
			pos += delta; 
			dirty = true;
		}

		inline void setPosition(const glm::dvec3& npos) {
			this->pos = npos; 
			dirty = true;
		}

		inline void rotateLocal (double radians, const glm::dvec3& axis) {
			glm::quat q = glm::angleAxis((float)radians, glm::normalize(glm::vec3(axis)));
			rot = glm::normalize(q * rot);
			dirty = true;
		}

		inline void rotateGlobal(double radians, const glm::dvec3& axis) {
			glm::quat q = glm::angleAxis((float)radians, glm::normalize(glm::vec3(axis)));
			rot = glm::normalize(rot * q);
			dirty = true;
		}

		inline void setScale(const glm::dvec3& s)
		{
			scl = s;
			dirty = true;
		}

		inline glm::mat4& model()
		{
			if (!dirty) return mat;

			const glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(pos));
			const glm::mat4 R = glm::mat4_cast(rot);
			const glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(scl));

			mat = T * R * S;
			dirty = false;

			return mat;
		}
	};

	class Model {
	public:
		Transform transform;

		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		Model(Device& device, const std::vector<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);

		Device& device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}