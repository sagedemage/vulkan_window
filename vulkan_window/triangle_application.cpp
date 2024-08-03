/* Local header files */
#include "triangle_application.hpp"

bool TriangleApplication::QueueFamilyIndices::IsComplete() {
    // Checks if the graphicsFamily and presentFamily objects
    // contain a value
    return graphicsFamily.has_value() && presentFamily.has_value();
}

void TriangleApplication::Run() {
    InitWindow();
    InitVulkan();
    MainLoop();
    CleanUp();
}

void TriangleApplication::InitWindow() {
    /* Initialize the GLFW window */
    // Initialize GLFW libary
    glfwInit();

    // Inform GLFW to not create an OpenGL context
    // GLFW was originally designed to create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Disable handling resized windows
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create the GLFW window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
}

void TriangleApplication::InitVulkan() {
    /* Initialize Vulkan */
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
}

void TriangleApplication::MainLoop() {
    /* Main game loop */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void TriangleApplication::CleanUp() {
    /* Clean up resources */
    for (auto* image_view : swapChainImageViews) {
        vkDestroyImageView(device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);

    vkDestroyDevice(device, nullptr);

    if (ENABLE_VALIDATION_LAYERS) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);

    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void TriangleApplication::CheckExtensionSupport() {
    /* Checking for extension support */
    // Count the amount of supported extensions
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    // Query extension details
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count,
                                           extensions.data());

    std::cout << "available extensions: " << std::endl;
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

void TriangleApplication::CreateInstance() {
    /* Create instance */
    if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport()) {
        throw std::runtime_error(
            "validation layers requested, but not available!");
    }

    // Fill in some information about the application
    // This data is optional.
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Informs the Vulkan driver which global extensions and
    // validation layers we want to use
    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    // Add an extension to inferface with the window system for GLFW
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = nullptr;

    // This function returns an array of required Vulkan instance extensions
    // for creating Vulkan surfaces on GLFW windows
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;

    // Retreive the required list of extensions
    auto extensions = GetRequiredExtensions();
    create_info.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};

    // Modify the VkInstanceCreateInfo struct to include the validation layer
    // names if they are enabled
    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount =
            static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        PopulateDebugMessengerCreateInfo(debug_create_info);
        create_info.pNext =
            reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(
                &debug_create_info);
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    // Create an instance
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error(
            "vkCreateInstance ERROR: failed to create instance!");
    }

    // checkExtensionSupport();
}

bool TriangleApplication::CheckValidationLayerSupport() {
    /* Checks if all of the requested layers are available */
    // List all of the available layers
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // Check if all of the layers in the validationLayers exist in the
    // availableLayers list
    for (const char* layer_name : VALIDATION_LAYERS) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> TriangleApplication::GetRequiredExtensions() {
    /* Retrieve the required list of extensions based on if the
    validation layers are enabled or disabled */
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions = nullptr;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions,
                                        glfw_extensions + glfw_extension_count);

    if (ENABLE_VALIDATION_LAYERS) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL TriangleApplication::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    /* Debug to the console for validation layers */
    std::string debug_msg = "validation layer: " +
                            static_cast<std::string>(pCallbackData->pMessage);
    std::cerr << debug_msg << std::endl;
    OutputDebugStringA(debug_msg.c_str());

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        // Message is important enought to show
        debug_msg = "validation layer ERROR: " +
                    static_cast<std::string>(pCallbackData->pMessage);
        throw std::runtime_error(debug_msg.c_str());
    }

    return VK_FALSE;
}

void TriangleApplication::SetupDebugMessenger() {
    if (!ENABLE_VALIDATION_LAYERS) {
        return;
    }

    // Fill in the details about the messenger and its callback
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    PopulateDebugMessengerCreateInfo(create_info);

    // Create the extension object if it is available
    if (CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr,
                                     &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

VkResult TriangleApplication::CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    /* Proxy function to create the debug messenger */
    // Looks up the address of the VkDebugUtilsMessengerEXT object
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void TriangleApplication::DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator) {
    /* Proxy function to destroy the debug messenger */
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void TriangleApplication::PopulateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    /* Fill in the details about the messenger and its callback to the structure
     */
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
}

void TriangleApplication::PickPhysicalDevice() {
    // Count the number of graphics cards with Vulkan support
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (device_count == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    // Allocate an array to hold all of the VkPhysicalDevice handles
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    // Device suitability checks
    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool TriangleApplication::IsDeviceSuitable(VkPhysicalDevice device) {
    /* Queue family lookup function to ensure the device can process the
    commands to use */
    QueueFamilyIndices indices = FindQueueFamilies(device);

    bool extensions_supported = CheckDeviceExtensionSupport(device);

    // Verify swap chain support is adequate
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        SwapChainSupportDetails swap_chain_support =
            QuerySwapChainSupport(device);
        swap_chain_adequate = !swap_chain_support.formats.empty() &&
                              !swap_chain_support.presentModes.empty();
    }

    return indices.IsComplete() && extensions_supported && swap_chain_adequate;
}

TriangleApplication::QueueFamilyIndices TriangleApplication::FindQueueFamilies(
    VkPhysicalDevice device) {
    /* Logic to find queue family indices to populate the struct with */
    QueueFamilyIndices indices;

    // The process to retreive the list of queue families
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                             nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                             queue_families.data());

    // Find at lest one queue family that supports VK_QUEUE_GRAPHICS_BIT
    int i = 0;
    for (const auto& queue_family : queue_families) {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        // Look for a queue family that is capable of presenting to the window
        // surface
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                             &present_support);

        if (present_support) {
            indices.presentFamily = i;
        }

        if (indices.IsComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void TriangleApplication::CreateLogicalDevice() {
    // Specify the queues to be created
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    // Set multiple VkDeviceQueueCreateInfo structs to create a queue
    // from both families which are mandatory for the required queues
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    std::set<uint32_t> unique_queue_families = {};

    if (indices.graphicsFamily.has_value() &&
        indices.presentFamily.has_value()) {
        unique_queue_families.insert(indices.graphicsFamily.value());
        unique_queue_families.insert(indices.presentFamily.value());
    } else {
        throw std::runtime_error(
            "Indices's graphics and present Families contain no value!");
    }

    float queue_priority = 1.0F;

    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    // Specify the device features to be used
    VkPhysicalDeviceFeatures device_features{};

    // Create the logical device
    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount =
        static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;

    // Enabling device extensions
    // Using a swapchain requires enabling the VK_KHR_swapchain
    create_info.enabledExtensionCount =
        static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    create_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    // Specify the validation layers for the logical device if the validation
    // layers is enabled
    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount =
            static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    // Instantiate the logical device
    if (vkCreateDevice(physicalDevice, &create_info, nullptr, &device) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    if (indices.graphicsFamily.has_value() &&
        indices.presentFamily.has_value()) {
        // Retrieve queue handles
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0,
                         &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0,
                         &presentQueue);
    } else {
        throw std::runtime_error(
            "Indices's graphics and present Families contain no value!");
    }
}

void TriangleApplication::CreateSurface() {
    // Cross platform way to create the window surface via GLFW
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

bool TriangleApplication::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    /* Enumerate the extensions and check if all of the required
    extensions are included in them */
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                         nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count,
                                         available_extensions.data());

    std::set<std::string> required_extensions(DEVICE_EXTENSIONS.begin(),
                                              DEVICE_EXTENSIONS.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

TriangleApplication::SwapChainSupportDetails
TriangleApplication::QuerySwapChainSupport(VkPhysicalDevice device) {
    TriangleApplication::SwapChainSupportDetails details;

    // Query basic surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                              &details.capabilities);

    // Querying the supported surface formats
    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count,
                                         nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count,
                                             details.formats.data());
    }

    // Querying the supported presentation modes
    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                              &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.presentModes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &present_mode_count, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR TriangleApplication::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    // Go through a list to find if the preferred combination is available
    //
    // VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: SRGB color space is supported
    // or not supported
    //
    // VK_FORMAT_B8G8R8A8_SRGB: store the BGRA channels in that order
    // with each channels containing a 8 bit unsigned integer.
    // This would result in a total of 32 bits per pixel.
    for (const auto& available_format : availableFormats) {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return available_format;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR TriangleApplication::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
    // VK_PRESENT_MODE_MAILBOX_KHR: This helps to avoid tearing and maintain a
    // low letency. Use this if energy is not an issue.
    //
    // VK_PRESENT_MODE_FIFO_KHR: If energy usage is an issue, use this.
    // This is recommended for mobile devices.
    for (const auto& available_present_mode : availablePresentModes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return available_present_mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D TriangleApplication::ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities) {
    // The swap extent is the resolution of the swap chain images and it's
    // usually always exactly equal to the resolution of the window that the
    // program draws in pixels.
    //
    // Using a high CPI display like Apple's Retina display), screen coordinates
    // don't correspond to pixels. The issue is that the resolution of the
    // window in pixels will be larger than the resolution in screen
    // coordinates.
    // The orginal WIDTH and HEIGHT variables will not work.
    // The glfwGetFramebufferSize function is required to query the resolution
    // of the window in pixels.
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                static_cast<uint32_t>(height)};

    actual_extent.width =
        std::clamp(actual_extent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);

    actual_extent.height =
        std::clamp(actual_extent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actual_extent;
}

void TriangleApplication::CreateSwapChain() {
    SwapChainSupportDetails swap_chain_support =
        QuerySwapChainSupport(physicalDevice);
    VkSurfaceFormatKHR surface_format =
        ChooseSwapSurfaceFormat(swap_chain_support.formats);
    VkPresentModeKHR present_mode =
        ChooseSwapPresentMode(swap_chain_support.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swap_chain_support.capabilities);

    // Decide how many images the program would like to have in the swap chain.
    // Request one more image than the minimum to prevent waiting on the driver
    // to complete internal operations before we can get another image to render
    // to.
    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

    // Make sure the program does not exceed the maximum number of images while
    // doing this. 0 means there is no maximum.
    if (swap_chain_support.capabilities.maxImageCount > 0 &&
        image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    // Specify the details of the swap chain
    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;

    // Specify the details of the swap chain images
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Handle swap chain images that will be used across multiple queue
    // families.
    //
    // VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a
    // time and this requires explicit ownership tranfers. This option offers
    // the best performance.
    //
    // VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue
    // faimilies without explicit onwership tranfers.

    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::array<uint32_t, 2> queue_family_indices = {0};

    if (indices.graphicsFamily.has_value() &&
        indices.presentFamily.has_value()) {
        queue_family_indices[0] = indices.graphicsFamily.value();
        queue_family_indices[1] = indices.presentFamily.value();
    } else {
        throw std::runtime_error(
            "Indices's graphics and present Families contain no value!");
    }

    if (indices.graphicsFamily != indices.presentFamily) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices.data();
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;      // Optional
        create_info.pQueueFamilyIndices = nullptr;  // Optional
    }

    // Specify the transformation to be applied to images in the
    // swap chain if it is supported
    create_info.preTransform = swap_chain_support.capabilities.currentTransform;

    // Specify if the alpha channel should be used for blending with other
    // windows in the window system. It is a good idea to ignore the alpha
    // channel via VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR.
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // Specify the presentation engine.
    // Enabling clipping provides the program the best performace.
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    // Existing non-retried swapchain currently associated with the surface
    create_info.oldSwapchain = VK_NULL_HANDLE;

    // Create the swap chain
    if (vkCreateSwapchainKHR(device, &create_info, nullptr, &swapChain) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // Retrieve the swap chain images
    // 1. First query the final number of images via vkGetSwapchainImagesKHR.
    // 2. Resize the container.
    // 3. Retrieve the handles via the vkGetSwapchainImagesKHR again.
    vkGetSwapchainImagesKHR(device, swapChain, &image_count, nullptr);
    swapChainImages.resize(image_count);
    vkGetSwapchainImagesKHR(device, swapChain, &image_count,
                            swapChainImages.data());

    // Store the format and extent for the swap chain images
    swapChainImageFormat = surface_format.format;
    swapChainExtent = extent;
}

void TriangleApplication::CreateImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        // Parameters for image view creation
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swapChainImages[i];

        // Specify how the image data should be interpreted
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapChainImageFormat;

        // Swizzle the color channels around
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // The subresource describes the image's purpose
        // and which part of the image should be accessed.
        //
        // The images will be used as color targets without
        // any mipmapping levels or multiple layers.
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        // Create the image view
        if (vkCreateImageView(device, &create_info, nullptr,
                              &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}