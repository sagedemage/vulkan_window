#ifndef TRIANGLE_APPLICATION_H
#define TRIANGLE_APPLICATION_H

/* Third party libraries */
#include <vulkan/vulkan.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIUS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <mat4x4.hpp>
#include <vec4.hpp>

/* Standard libraries */
#include <Windows.h>

#include <algorithm>  // Required for std::clamp
#include <array>
#include <cstdint>  // Required for uint32_t
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>  // Required for std::numeric_limits
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Enable the standard diagnostic layers provided by the Vulkan SDK
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

// Declare a list of required device extensions
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#define NDEBUG

#ifndef NDEBUG  // not debug
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class TriangleApplication {
   private:
    GLFWwindow* window{};
    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device{};
    VkQueue graphicsQueue{};
    VkSurfaceKHR surface{};
    VkQueue presentQueue{};
    VkSwapchainKHR swapChain{};
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat{};
    VkExtent2D swapChainExtent{};

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete();
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanUp();
    static void checkExtensionSupport();
    void createInstance();
    static bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData);
    void setupDebugMessenger();
    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator);
    static void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createLogicalDevice();
    void createSurface();
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSwapChain();
    void createImageViews();

   public:
    void run();
};

#endif  // TRIANGLE_APPLICATION_H
