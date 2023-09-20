#define GLFW_INCLUDE_VULKAN

#include "utils.hpp"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	void setFramebufferResized(bool resized) { framebufferResized = resized; };

private:
	GLFWwindow *window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;
	uint32_t currentFrame = 0;
	bool framebufferResized = false;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	// it is recommended to use a single buffer to store multiple buffers (like vertexBuffer) and
	// use offsets in commands (like vkCmdBindVertexBuffers). The reason is that your data will be
	// more cache friendly, because it's closer together in memory. This is especially important
	// for integrated graphics cards that use shared memory.

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

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

	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();

	void createCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void createSyncObjects();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
					  VkBuffer &buffer, VkDeviceMemory &bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createVertexBuffer();
	void createIndexBuffer();

	VkShaderModule createShaderModule(const std::vector<char> &node);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	std::vector<const char *> getRequiredExtensions();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				  VkDebugUtilsMessageTypeFlagsEXT messageType,
				  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
};
