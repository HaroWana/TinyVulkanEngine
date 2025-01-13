#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

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
        void    freeCommandBuffers();
        void    drawFrame();
        void    recreateSwapChain();
        void    recordCommandBuffer(int imageIndex);

        void    sierpinski(std::vector<VEModel::Vertex>& vertices, int depth, VEModel::Vertex left, VEModel::Vertex right, VEModel::Vertex top);

        VEWindow    veWindow{WIDTH, HEIGHT, "Hello Vulkan"};
        VEDevice    veDevice{veWindow};
        std::unique_ptr<VESwapChain> veSwapChain;
        std::unique_ptr<VEPipeline> vePipeline;
        VkPipelineLayout            pipelineLayout;
        std::vector<VkCommandBuffer>    commandBuffers;
        std::unique_ptr<VEModel>        veModel;
    };
} // namespace ve

#endif // FIRST_APP_HPP