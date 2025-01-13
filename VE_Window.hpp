#ifndef VE_WINDOW_HPP
# define VE_WINDOW_HPP

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
        bool    wasWindowResized() { return (framebufferResized); }
        void    resetWindowResizedFlag() { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void    initWindow();

        int     width;
        int     height;
        bool    framebufferResized = false;

        std::string winName;
        GLFWwindow  *window;
    };

}

#endif // VE_WINDOW_HPP
