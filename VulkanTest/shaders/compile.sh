/usr/local/bin/glslc shader.vert -o vert2.spv
/usr/local/bin/glslc shader.frag -o frag2.spv

# spirv-dis vert.spv -o vert.spvasm (disassembler)
# spirv-dis frag.spv -o frag.spvasm (disassembler)
# vulkan includes libshaderc libraries to compile GLSL code to SPIR-V from within an application
