#ifndef VULKAN_TUT_VE_PIPELINE_HPP
#define VULKAN_TUT_VE_PIPELINE_HPP

#include "VE_Device.hpp"

//  STD
#include <string>
#include <vector>

namespace ve {
    struct  PipelineConfigInfo {};

    class   VEPipeline
    {
    public:
        VEPipeline(
                VEDevice &device,
                const std::string &vertFilepath,
                const std::string &fragFilepath,
                const PipelineConfigInfo &configInfo);

        ~VEPipeline() {};

        VEPipeline(const VEPipeline &) = delete;
        void operator=(const VEPipeline &) = delete;

        static PipelineConfigInfo   defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char>    readFile(const std::string &filepath);
        void    createGraphicsPipeline(const std::string &vertFilepath,
                                       const std::string &fragFilepath,
                                       const PipelineConfigInfo &configInfo);

        void    createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        VEDevice    &veDevice;
        VkPipeline  graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}

#endif //VULKAN_TUT_VE_PIPELINE_HPP
