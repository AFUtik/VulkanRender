#pragma once

#include "Device.hpp"

#include <vma/vk_mem_alloc.h>
#include <string>

namespace myvk {
    class Texture {
    public:
        Texture(Device& device, const char* pFilename);
        ~Texture();

        Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

        VkSampler getSampler() {return sampler;}
        VkImageView getView() {return view;}
    private:
        void imageMemBarrier(VkCommandBuffer CmdBuf, VkImageLayout OldLayout, VkImageLayout NewLayout, int layerCount);
        
        void transitionImageLayout(VkImageLayout OldLayout, VkImageLayout NewLayout, int LayerCount);

		void updateTextureImage(int layerCount, const void* pPixels);

		void createImageView(VkImageAspectFlags AspectFlags);

		void createTextureSampler(VkFilter MinFilter, VkFilter MaxFilter, VkSamplerAddressMode AddressMode);

		void createImage();

        void createTextureFromData(const void* pPixels);

		void createTexture(const char *pFilename);

        bool isCubemap = false;
        int imageWidth, imageHeight, imageChannels;

        VkFormat format;
        VkImage image;
        VmaAllocation vmaAllocation;
        VkImageView view;
        VkSampler sampler;
        VkImageLayout imageLayout;
        Device& device;
    };
}