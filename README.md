# Tiny Vulkan Engine
=======================

**Work in Progress**
--------------------

This is a basic, tiny, and highly experimental Vulkan engine. It is very far from ready for production use and lacks many features.
For now, I am following the tutorial "Vulkan (c++) Game Engine Tutorials" by Brendan Galea (github.com/blurrypiano). The plan is to greatly expand the features and optimizations after completion of the tutorial.
I don't have much time to dedicate to this project as of now, so this might take a little while.

**Features**
------------

* Basic window creation using GLFW
* Swapchain creation and management
* Model loading and rendering
* Simple pipeline setup
* Vertex buffer and index buffer management

**Requirements**
---------------

* C++17 compiler
* GLFW 3.3 or later
* Vulkan SDK
* pkg-config

**Building**
------------

To build the engine, use the following commands:

```bash
make test
```

**Running**
------------

Run the engine using the following command:

```bash
./VulkanTest
```

## Roadmap
------------

* Improve pipeline setup and management
* Add support for more complex models
* Implement lighting and shading
* Add support for physics and animation

**License**
------------

This project is licensed under the MIT License.

Note: This is a basic README file for a work-in-progress project. As the project evolves, this file will be updated to reflect the current state of the project.