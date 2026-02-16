#pragma once

#include "Device.hpp"
#include <string>

namespace myvk {
    class Texture {
    public:
        //Texture(Device &device, const std::string &path);
        //~Texture();

        Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
    private:
        //void createImageTexture(const std::string &path);
        //void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        //void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        Device &device;

        VkImage image;
        VkDeviceMemory imageMemory;
        VkImageView view;
        VkSampler sampler;
    };
}