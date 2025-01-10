#ifndef VE_PIPELINE_HPP
# define VE_PIPELINE_HPP

#include "VE_Device.hpp"

//  STD
#include <string>
#include <vector>

namespace ve {
    struct  PipelineConfigInfo {
        PipelineConfigInfo() = default;

        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo       viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo  rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo    multisampleInfo;
        VkPipelineColorBlendAttachmentState     colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo     colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo   depthStencilInfo;
        std::vector<VkDynamicState>             dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo        dynamicStateInfo;
        VkPipelineLayout                        pipelineLayout = nullptr;
        VkRenderPass                            renderPass = nullptr;
        uint32_t                                subpass = 0;
    };

    class   VEPipeline
    {
    public:
        VEPipeline() = default;
        VEPipeline(
                VEDevice                    &device,
                const std::string           &vertFilepath,
                const std::string           &fragFilepath,
                const PipelineConfigInfo    &configInfo);

        ~VEPipeline();

        VEPipeline(const VEPipeline &) = delete;
        VEPipeline &operator=(const VEPipeline &) = delete;

        void    bind(VkCommandBuffer commandBuffer);
        static void   defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char>    readFile(const std::string &filepath);
        void    createGraphicsPipeline(const std::string &vertFilepath,
                                       const std::string &fragFilepath,
                                       const PipelineConfigInfo &configInfo);

        void    createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        VEDevice        &veDevice;
        VkPipeline      graphicsPipeline;
        VkShaderModule  vertShaderModule;
        VkShaderModule  fragShaderModule;
    };
}

#endif // VE_PIPELINE_HPP
