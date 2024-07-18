#include "VE_Window.hpp"

#include <stdexcept>

namespace ve {

    VEWindow::VEWindow(int w, int h, std::string name): width(w), height(h), winName(name) {
        initWindow();
    }

    VEWindow::~VEWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void    VEWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, winName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void VEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface");
    }

    void VEWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto veWindow = reinterpret_cast<VEWindow*>(glfwGetWindowUserPointer(window));
        veWindow->framebufferResized = true;
        veWindow->width = width;
        veWindow->height = height;
    }



} // namespace ve