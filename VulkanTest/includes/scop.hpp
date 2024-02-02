#ifndef SCOP_HPP
#define SCOP_HPP
#define GLFW_INCLUDE_VULKAN

#include "utils.hpp"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Scop {
public:
	Scop(const char *modelPath, const char *texturePath)
		: MODEL_PATH(modelPath), TEXTURE_PATH(texturePath){};
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	void setFramebufferResized(bool resized) { framebufferResized = resized; };

private:
	const char *MODEL_PATH;
	const char *TEXTURE_PATH;
	GLFWwindow *window;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	std::vector<VkPipeline> graphicsPipelines;
	size_t currentPipelineIndex = 0;

	VkCommandPool commandPool;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	// it is recommended to use a single buffer to store multiple buffers (like vertexBuffer) and
	// use offsets in commands (like vkCmdBindVertexBuffers). The reason is that your data will be
	// more cache friendly, because it's closer together in memory. This is especially important
	// for integrated graphics cards that use shared memory.

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void *> uniformBuffersMapped;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

	bool framebufferResized = false;

	Vec3 cameraPos = Vec3(0.0f, 0.0f, 3.0f);
	Vec3 cameraFront = Vec3(0.0f, 0.0f, -1.0f);
	Vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);

	//glm::vec3 cameraPos2 = glm::vec3(0.0f, 0.0f, 3.0f);
	//glm::vec3 cameraFront2 = glm::vec3(0.0f, 0.0f, -1.0f);
	//glm::vec3 cameraUp2 = glm::vec3(0.0f, 1.0f, 0.0f);

	const float scaleFactor = 0.01f;
	float modelScale = 0.01f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float rollAngle = 0.0f;

	void initWindow();
	void initVulkan();
	void mainLoop();
	void drawFrame();
	void cleanup();

	void createInstance();

	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	void createLogicalDevice();

	void createSurface();
	VkSurfaceFormatKHR
	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
	VkPresentModeKHR
	chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	void createSwapChain();
	void recreateSwapChain();
	void cleanupSwapChain();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
					 VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
					 VkDeviceMemory &imageMemory);
	void createImageViews();
	void createRenderPass();
	void createPipelineLayout();
	VkPipeline createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
	void createFramebuffers();

	void createCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void createSyncObjects();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
					  VkBuffer &buffer, VkDeviceMemory &bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void createVertexBuffer();
	void createIndexBuffer();

	void createDescriptorSetLayout();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void createDescriptorSets();

	void createTextureImage();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
							   VkImageLayout newLayout);
	void createTextureImageView();
	void createTextureSampler();

	void loadModel();
	void computeUVs(Vertex &vertex);

	VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
								 VkFormatFeatureFlags features);
	void createDepthResources();
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
	VkShaderModule createShaderModule(const std::vector<char> &node);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	std::vector<const char *> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				  VkDebugUtilsMessageTypeFlagsEXT messageType,
				  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
};

#endif
