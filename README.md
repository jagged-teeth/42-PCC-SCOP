# SCOP : GPU Rendering Project

<div id="header" align="center">
<img src="https://s6.gifyu.com/images/S6MMV.gif" width="650" height="320" />
</div>

This project explores basic GPU rendering through Vulkan, rendering a textured model within a GLFW window using C++. The model, sourced from an OBJ file and textured from a BMP file, rotates continuously around its central axis. Users can zoom in/out on the model.

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

## Usage
```fish
./scop model.obj texture.bmp
```
`Up Arrow` and `Down Arrow` for the zoom functions.

## Vulkan Concepts
Vulkan is a low-overhead, cross-platform 3D graphics and computing API. This project followed the "Hello Triangle" tutorial, extending the concepts learned to render a textured 3D model.

#### Initialization
Vulkan interacts with the GPU through Physical Devices and Queue Families. A Physical Device represents a GPU on your machine, while Queue Families categorize the operations (e.g., graphics, compute) that queues in this family can execute. Queues are created from Queue Families and are interfaces to the GPU, allowing commands to be sent for execution.

#### Vulkan Surface and Swap Chain
Vulkan interacts with the window system via a surface, while a Swap Chain organizes the image buffers to be displayed.
- The Swap Chain holds a series of images where each image represents a "frame" that can be rendered to. It efficiently handles the swapping of the image being displayed on the screen with the next image to be rendered.
- Image Views represent a specific view into an image, allowing an image to be used with a specific part of the Vulkan pipeline.

#### Render Pass and Pipeline
The Graphics Pipeline is a series of stages that process data sequentially to render an image to the screen. It encompasses several stages, including vertex shading, fragment shading, and fixed-function stages for various processing (e.g., tessellation, geometry shading).

#### Buffer and Memory Management
1. Vertex Buffers store vertex data, while Uniform Buffers store data that remains consistent across a single render pass.
2. Staging Buffers are used as temporary buffers to transfer data to GPU-accessible buffers efficiently.
3. Depth Buffers store depth information to handle overlapping objects correctly.
4. Command Buffers store commands that will be submitted to the GPU.
5. Framebuffers are collections of memory attachments (e.g., color, depth, stencil attachments) where the output of rendering commands is stored.

## BMP Texture Loader
The BMP loader is a critical component for rendering textures on the model. The provided code showcases a C++ implementation of a simple BMP file loader, which reads the BMP file, extracts necessary header information, and processes pixel data for further usage in Vulkan.\

### Reading BMP Files
The BMP file is opened in binary mode using `std::ifstream`. The file and info headers are read to acquire metadata such as dimensions, offset to pixel data and bit count. Additionally, for 32-bit BMPs with a color header, the color header is read to ensure the pixel data is in the expected format. The reader navigates to the pixel data's offset, calculates the data size, and reads the pixel data. If the image width isn't divisible by 4 (a requirement for BMP), padding is calculated, and each row is read separately with padding.

Post BMP file reading, the pixel data resides in the `data` vector within the `BMP` struct and can be loaded through Vulkan
```cpp
vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
memcpy(data, my_loader.data.data(), static_cast<size_t>(imageSize));
vkUnmapMemory(device, stagingBufferMemory);
```

### Applying the texture through spherical projection
Spherical projection, also known as spherical mapping, is a method used to map a 2D texture onto a 3D model. In this technique, each vertex of the 3D model is mapped to a 2D point on the texture using spherical coordinates.

For each vertex, spherical coordinates are computed using its Cartesian coordinates.
```cpp
float theta = atan2(vertex.pos.z, vertex.pos.x) / (2 * M_PI);
float phi = acos(vertex.pos.y / length(vertex.pos)) / M_PI;
```
The spherical coordinates are then mapped to texture coordinates, which are used to sample the texture. Basically you wrap the flat texture around your 3D object by projecting it onto the object's surface.

## OBJ Loader
### Reading OBJ Files
The OBJ loader is responsible for reading the geometry data from an OBJ file and preparing it for rendering. The loader reads through the file line by line. If a line starts with "v", it's a vertex line and the vertex data is read and stored. If a line starts with "f", it's a face line, and the indices of the vertices that form the face are read and stored. The vertices are stored temporarily, and the vertex indices from the face data are used to organize the vertices into the final format for rendering.

### Loading the OBJ in Vulkan
I call the `loadObj` function to load the vertex data from the OBJ file into `temp_vertices`. Following this, I calculate the center of mass of the model to reposition the vertices around the center, which ensures a balanced distribution of vertices around the origin, aiding in a better rendering and manipulation of the model.

Then, I iterate through each vertex in `temp_vertices`, create a Vertex structure, assign the position and a color, and compute the texture coordinates using the `computeUVs` function (spherical projection). Lastly, I perform vertex deduplication by checking if a vertex is unique, which helps in optimizing the data by eliminating duplicate vertices and thus, conserving memory and improving rendering performance.


