#ifndef VULKAN_TUT_VE_WINDOW_HPP
#define VULKAN_TUT_VE_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace ve {

    class VEWindow
    {
    public:
        VEWindow(int w, int h, std::string name);
        ~VEWindow();

        VEWindow(const VEWindow &) = delete;
        VEWindow &operator=(const VEWindow &) = delete;

        bool shouldClose() { return (glfwWindowShouldClose(window)); }
        VkExtent2D  getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        void    initWindow();

        const int   width;
        const int   height;

        std::string winName;
        GLFWwindow  *window;
    };

}

#endif //VULKAN_TUT_VE_WINDOW_HPP
