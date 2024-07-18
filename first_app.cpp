#include "first_app.hpp"

// std
#include <stdexcept>
#include <array>
#include <cassert>

namespace ve {
    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(veDevice.device(), pipelineLayout, nullptr);
    }

    void    FirstApp::run() {
        while (!veWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(veDevice.device());
    }

    void    FirstApp::loadModels()
    {
        std::vector<VEModel::Vertex> vertices{};
    
        sierpinski(vertices, 5, 
            { { -0.5f, 0.5f }, {1.0f, 0.0, 0.0} },
            { { 0.5f, 0.5f }, {0.0f, 1.0f, 0.0f} },
            { { 0.0f, -0.5f }, {0.0f, 0.0f, 1.0f} });
        veModel = std::make_unique<VEModel>(veDevice, vertices);
    }

    void    FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(veSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = veDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(veDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers!");


    }

    void    FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            veDevice.device(),
            veDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void    FirstApp::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording command buffer!");

        VkRenderPassBeginInfo   renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = veSwapChain->getRenderPass();
        renderPassInfo.framebuffer = veSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = veSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues;
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(veSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(veSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, veSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        vePipeline->bind(commandBuffers[imageIndex]);
        veModel->bind(commandBuffers[imageIndex]);
        veModel->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
            throw std::runtime_error("Failed to record command buffer!");
    }

    void    FirstApp::drawFrame()
    {
        uint32_t    imageIndex;
        auto result = veSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swap chain image!");

        recordCommandBuffer(imageIndex);
        result = veSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || veWindow.wasWindowResized())
        {
            veWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to present swap chain image!");
    }

    void    FirstApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo  pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(veDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout!");
    }

    void    FirstApp::createPipeline()
    {
        assert(veSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VEPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = veSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vePipeline = std::make_unique<VEPipeline>(
            veDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void    FirstApp::recreateSwapChain()
    {
        auto extent = veWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = veWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(veDevice.device());
        if (veSwapChain == nullptr)
        {
            veSwapChain = std::make_unique<VESwapChain>(veDevice, extent);
        }
        else
        {
            veSwapChain = std::make_unique<VESwapChain>(veDevice, extent, std::move(veSwapChain));
            if (veSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        // TO BE PERFECTED (if render pass compatible do nothing else)
        createPipeline();
    }

    void    FirstApp::sierpinski(std::vector<VEModel::Vertex>& vertices, int depth, VEModel::Vertex left, VEModel::Vertex right, VEModel::Vertex top)
    {
        if (depth <= 0)
        {
            vertices.push_back({ top });
            vertices.push_back({ right });
            vertices.push_back({ left });
        }
        else
        {
            VEModel::Vertex    leftTop = { {0.5f * (left.position + top.position)}, {left.color} };
            VEModel::Vertex    rightTop = { {0.5f * (right.position + top.position)}, {top.color} };
            VEModel::Vertex    leftRight = { {0.5f * (left.position + right.position)}, {right.color} };
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }
}
