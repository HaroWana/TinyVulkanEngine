#ifndef VULKAN_TUT_FIRST_APP_HPP
#define VULKAN_TUT_FIRST_APP_HPP

#include "VE_Window.hpp"
#include "VE_Pipeline.hpp"
#include "VE_Device.hpp"
#include "VE_Swap_Chain.hpp"
#include "VE_Model.hpp"

#include <memory>
#include <vector>

namespace ve {
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;

        void    run();

    private:
        void    loadModels();
        void    createPipelineLayout();
        void    createPipeline();
        void    createCommandBuffers();
        void    drawFrame();

        void    sierpinski(std::vector<VEModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);

        VEWindow    veWindow{WIDTH, HEIGHT, "Hello Vulkan"};
        VEDevice    veDevice{veWindow};
        VESwapChain veSwapChain{veDevice, veWindow.getExtent()};
        std::unique_ptr<VEPipeline> vePipeline;
        VkPipelineLayout            pipelineLayout;
        std::vector<VkCommandBuffer>    commandBuffers;
        std::unique_ptr<VEModel>        veModel;
    };
} // namespace ve

#endif //VULKAN_TUT_FIRST_APP_HPP