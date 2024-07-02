#ifndef VULKAN_TUT_FIRST_APP_HPP
#define VULKAN_TUT_FIRST_APP_HPP

#include "VE_Window.hpp"
#include "VE_Pipeline.hpp"
#include "VE_Device.hpp"

namespace ve {
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void    run();

    private:
        VEWindow    veWindow{WIDTH, HEIGHT, "Hello Vulkan"};
        VEDevice    veDevice{veWindow};
        VEPipeline  vePipeline{veDevice,
                               "shaders/simple_shader.vert.spv",
                               "shaders/simple_shader.frag.spv",
                               VEPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}
#endif //VULKAN_TUT_FIRST_APP_HPP
