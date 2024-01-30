#include "scop.hpp"

void Scop::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) !=
		VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout!");
}

void Scop::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					 uniformBuffers[i], uniformBuffersMemory[i]);
		vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}

void Scop::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time =
		std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	// Apply rotation
	ubo.model =
		Mat4::rotate(Mat4(1.0f), time * radians(90.0f), Vec3(0.0f, 1.0f, 0.0f));

	// Adjust model scale
	ubo.model = Mat4::scale(ubo.model, Vec3(modelScale, modelScale, modelScale));

	// Camera position and view
	ubo.view = Mat4::lookAt(cameraPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	// Perspective projection
	ubo.proj = Mat4::perspective(radians(45.0f),
								swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	// Copy UBO bytes to uniform buffer memory
	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Scop::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Scop::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()),
							   descriptorWrites.data(), 0, nullptr);
	}
}

// GLMD Mat4 debug

//////
//void printCustomMat(const Mat4& mat) {
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			std::cout << mat[i][j] << " ";
//		}
//		std::cout << std::endl;
//	}
//}

// Function to print glm::mat4 for debugging
//void printGLMMat(const glm::mat4& mat) {
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			std::cout << mat[i][j] << " ";
//		}
//		std::cout << std::endl;
//	}
//}

//void printUBOInfo() {
//	std::cout << "\nSize of UniformBufferObject: " << sizeof(UniformBufferObject) << std::endl;
//	std::cout << "Offset of model: " << offsetof(UniformBufferObject, model) << std::endl;
//	std::cout << "Offset of view: " << offsetof(UniformBufferObject, view) << std::endl;
//	std::cout << "Offset of proj: " << offsetof(UniformBufferObject, proj) << std::endl;

//	std::cout << "Size of Mat4: " << sizeof(Mat4) << std::endl;

//	std::cout << "Size of UniformBufferObject2: " << sizeof(UniformBufferObject2) << std::endl;
//	std::cout << "Offset of model: " << offsetof(UniformBufferObject2, model) << std::endl;
//	std::cout << "Offset of view: " << offsetof(UniformBufferObject2, view) << std::endl;
//	std::cout << "Offset of proj: " << offsetof(UniformBufferObject2, proj) << std::endl;

//	std::cout << "Size of glm::mat4: " << sizeof(glm::mat4) << std::endl;
//}

//void Scop::updateUniformBuffer(uint32_t currentImage) {
//	static auto startTime = std::chrono::high_resolution_clock::now();
//	auto currentTime = std::chrono::high_resolution_clock::now();
//	float time =
//		std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//	std::cout << "--------------------PASS-------------------" << std::endl;

//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	UniformBufferObject2 ubo2{};
//	ubo2.model =
//		glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//	// Print UBO matrices
//	std::cout << "GLM Rotation Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printGLMMat(ubo2.model);
//	std::cout << "\nview" << std::endl;
//	printGLMMat(ubo2.view);
//	std::cout << "\nproj" << std::endl;
//	printGLMMat(ubo2.proj);

//	// Adjust model scale
//	ubo2.model = glm::scale(ubo2.model, glm::vec3(modelScale, modelScale, modelScale));
//	 Print UBO matrices
//	std::cout << "\nGLM Scale Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printGLMMat(ubo2.model);
//	std::cout << "\nview" << std::endl;
//	printGLMMat(ubo2.view);
//	std::cout << "\nproj" << std::endl;
//	printGLMMat(ubo2.proj);

//	// Camera position and view
//	ubo2.view = glm::lookAt(cameraPos2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	// Print UBO matrices
//	std::cout << "\nGLM LookAt Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printGLMMat(ubo2.model);
//	std::cout << "\nview" << std::endl;
//	printGLMMat(ubo2.view);
//	std::cout << "\nproj" << std::endl;
//	printGLMMat(ubo2.proj);

//	ubo2.proj = glm::perspective(glm::radians(45.0f),
//								swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
//	ubo2.proj[1][1] *= -1;
//	// Print UBO matrices
//	std::cout << "\nGLM Perspective Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printGLMMat(ubo2.model);
//	std::cout << "\nview" << std::endl;
//	printGLMMat(ubo2.view);
//	std::cout << "\nproj" << std::endl;
//	printGLMMat(ubo2.proj);

//	//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	UniformBufferObject ubo{};

//	ubo.model =
//		Mat4::rotate(Mat4(1.0f), time * radians(90.0f), Vec3(0.0f, 1.0f, 0.0f));
//	 Print UBO matrices
//	std::cout << "\nCustom Rotation Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printCustomMat(ubo.model);
//	std::cout << "\nview" << std::endl;
//	printCustomMat(ubo.view);
//	std::cout << "\nproj" << std::endl;
//	printCustomMat(ubo.proj);

//	// Adjust model scale
//	ubo.model = Mat4::scale(ubo.model, Vec3(modelScale, modelScale, modelScale));
//	// Print UBO matrices
//	std::cout << "\nCustom Scale Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printCustomMat(ubo.model);
//	std::cout << "\nview" << std::endl;
//	printCustomMat(ubo.view);
//	std::cout << "\nproj" << std::endl;
//	printCustomMat(ubo.proj);

//	// Camera position and view
//	ubo.view = Mat4::lookAt(cameraPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
//	// Print UBO matrices
//	std::cout << "\nCustom LookAt Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printCustomMat(ubo.model);
//	std::cout << "\nview" << std::endl;
//	printCustomMat(ubo.view);
//	std::cout << "\nproj" << std::endl;
//	printCustomMat(ubo.proj);

//	ubo.proj = Mat4::perspective(radians(45.0f),
//								swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
//	ubo.proj[1][1] *= -1;
//	// Print UBO matrices
//	std::cout << "\nCustom Perspective Matrix:" << std::endl;
//	std::cout << "model" << std::endl;
//	printCustomMat(ubo.model);
//	std::cout << "\nview" << std::endl;
//	printCustomMat(ubo.view);
//	std::cout << "\nproj" << std::endl;
//	printCustomMat(ubo.proj);

//	//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// Print Ubos
//	printUBOInfo();
//	// Print UBO bytes
//	std::cout << "UBO Bytes:" << std::endl;
//	const unsigned char* p = reinterpret_cast<const unsigned char*>(&ubo);
//	for (size_t i = 0; i < sizeof(ubo); ++i) {
//		std::cout << std::hex << static_cast<int>(p[i]) << " ";
//		if ((i + 1) % 16 == 0) std::cout << std::endl;
//	}
//	std::cout << std::dec << std::endl;

//	// Print UBO2 bytes
//	std::cout << "UBO2 Bytes:" << std::endl;
//	const unsigned char* p2 = reinterpret_cast<const unsigned char*>(&ubo2);
//	for (size_t i = 0; i < sizeof(ubo2); ++i) {
//		std::cout << std::hex << static_cast<int>(p2[i]) << " ";
//		if ((i + 1) % 16 == 0) std::cout << std::endl;
//	}
//	std::cout << std::dec << std::endl;
//	std::cout << "Size of UBO: " << sizeof(UniformBufferObject) << std::endl;
//	std::cout << "Size of UBO2: " << sizeof(UniformBufferObject2) << std::endl;

//	// Copy UBO bytes to uniform buffer memory
//	//memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
//	if (currentImage % 2 == 0) {
//		memcpy(uniformBuffersMapped[currentImage], &ubo2, sizeof(ubo2));
//	} else {
//		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
//	}
//	std::cout << "\ncurrentImage: " << currentImage << "\n" << std::endl;
//	std::cout << "\ncurrentImage: " << uniformBuffersMapped[currentImage] << "\n" << std::endl;
//}
