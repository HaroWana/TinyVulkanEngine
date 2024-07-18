#ifndef VE_MODEL_HPP
# define VE_MODEL_HPP

#include "VE_Device.hpp"

// LIBS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// STD
#include <vector>

namespace ve
{
	class VEModel
	{
	public:
		struct Vertex
		{
			glm::vec2	position;
			glm::vec3	color;

			static std::vector<VkVertexInputBindingDescription>	getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription>	getAttributeDescriptions();
		};

		VEModel(VEDevice& device, const std::vector<Vertex>& vertices);
		~VEModel();

		VEModel(const VEModel&) = delete;
		VEModel &operator=(const VEModel&) = delete;

		void	bind(VkCommandBuffer commandBuffer);
		void	draw(VkCommandBuffer commandBuffer);


	private:
		void	createVertexBuffer(const std::vector<Vertex>& vertices);

		VEDevice&		veDevice;
		VkBuffer		vertexBuffer;
		VkDeviceMemory	vertexBufferMemory;
		uint32_t		vertexCount;
	};
}

#endif