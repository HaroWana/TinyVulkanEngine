#include "first_app.hpp"

// std
#include <stdexcept>
#include <array>

namespace ve {
    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        createPipeline();
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
        sierpinski(vertices, 5, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
        veModel = std::make_unique<VEModel>(veDevice, vertices);
    }

    void    FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(veSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = veDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(veDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers!");

        for (int i = 0; i < commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Failed to begin recording command buffer!");

            VkRenderPassBeginInfo   renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = veSwapChain.getRenderPass();
            renderPassInfo.framebuffer = veSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = veSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues;
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vePipeline->bind(commandBuffers[i]);
            veModel->bind(commandBuffers[i]);
            veModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void    FirstApp::drawFrame()
    {
        uint32_t    imageIndex;
        auto result = veSwapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire swap chain image!");

        result = veSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
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
        auto pipelineConfig = VEPipeline::defaultPipelineConfigInfo(veSwapChain.width(), veSwapChain.height());
        pipelineConfig.renderPass = veSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vePipeline = std::make_unique<VEPipeline>(
            veDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void    FirstApp::sierpinski(std::vector<VEModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
    {
        if (depth <= 0)
        {
            vertices.push_back({ top });
            vertices.push_back({ right });
            vertices.push_back({ left });
        }
        else
        {
            auto    leftTop = 0.5f * (left + top);
            auto    rightTop = 0.5f * (right + top);
            auto    leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }
}
