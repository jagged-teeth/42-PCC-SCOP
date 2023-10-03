# SCOP : GPU Rendering Project
This project explores basic GPU rendering through Vulkan, rendering a textured model within a GLFW window using C++. The model, sourced from an OBJ file and textured from a BMP file, rotates continuously around its central axis. Users can zoom in/out and maneuver the camera.

#### Technologies Used

- **Rendering API**: Vulkan
- **Window and Event Management**: GLFW
- **Language**: C++

## Getting Started

### Prerequisites
Ensure the following packages are installed on your Linux machine:

- Vulkan SDK
- GLFW Library
- GLSL Shader Compiler (if you want to mess with the shaders)

### Building the Project

A Makefile is provided for building the project. Navigate to the project directory and run

```fish
make
```
or 
```fish
make debug
```
to get the validation layers. This will compile the source files and generate the executable.

## Vulkan Concepts
