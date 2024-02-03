#include "scop.hpp"

static void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
	auto app = reinterpret_cast<Scop *>(glfwGetWindowUserPointer(window));
	app->setFramebufferResized(true);
}

void Scop::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Scop::initVulkan() {
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createPipelineLayout();
	VkPipeline pipeline1 = createGraphicsPipeline("shaders/vert.spv", "shaders/frag.spv");
	VkPipeline pipeline2 = createGraphicsPipeline("shaders/vert2.spv", "shaders/frag2.spv");
	VkPipeline pipeline3 = createGraphicsPipeline("shaders/vert3.spv", "shaders/frag3.spv");
	graphicsPipelines.push_back(pipeline1);
	graphicsPipelines.push_back(pipeline2);
	graphicsPipelines.push_back(pipeline3);
	createCommandPool();
	createDepthResources();
	createFramebuffers();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	loadModel();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}

void Scop::mainLoop() {
	const float maxVangle = radians(87.0f);
	const float minVangle = radians(-87.0f);

	float angle = 0.0f;
	float verticalAngle = 0.0f;
	const float rotationSpeed = 0.05f;
	bool rKeyPressedLastFrame = false;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		bool rKeyPressedNow = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
		if (rKeyPressedNow && !rKeyPressedLastFrame) {
			currentPipelineIndex = (currentPipelineIndex + 1) % 3;
		}
		rKeyPressedLastFrame = rKeyPressedNow;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			modelScale += scaleFactor;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			modelScale -= scaleFactor;

		// Yaw
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			verticalAngle += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			verticalAngle -= rotationSpeed;
		// Pitch
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			angle += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			angle -= rotationSpeed;

		// Roll
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			rollAngle += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			rollAngle -= rotationSpeed;

		float radius = 5.0f;
		cameraPos.x = cos(verticalAngle) * sin(angle) * radius;
		cameraPos.y = sin(verticalAngle) * radius;
		cameraPos.z = cos(verticalAngle) * cos(angle) * radius;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		verticalAngle = clamp(verticalAngle, minVangle, maxVangle);
		modelScale = clamp(modelScale, 0.01f, 1.0f);
		drawFrame();
	}

	vkDeviceWaitIdle(device);
}

void Scop::drawFrame() {
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result =
		vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
							  VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
	updateUniformBuffer(currentFrame);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	/* if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	gives a race condition validation layer error when resizing the window. This is because
	during the resize event, the size of the SurfaceCapabilities (see querySwapChainSupport) is
	asynchronous compared to the GLFW framebuffer size callback. No fix seems available */
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		framebufferResized = false;
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Scop::cleanup() {
	cleanupSwapChain();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}
	vkDestroySampler(device, textureSampler, nullptr);
	vkDestroyImageView(device, textureImageView, nullptr);
	vkDestroyImage(device, textureImage, nullptr);
	vkFreeMemory(device, textureImageMemory, nullptr);
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);
	for (auto pipeline : graphicsPipelines)
		vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyDevice(device, nullptr);
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}
