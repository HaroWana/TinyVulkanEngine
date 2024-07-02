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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, winName.c_str(), nullptr, nullptr);
    }

    void VEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface");
    }
}