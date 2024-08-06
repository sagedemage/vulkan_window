cmake_minimum_required(VERSION 3.30)
# Set the project Name
project(VulkanWindow)

# Set the C++ Standard to compile against
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C++ Flags
set(OPTIMIZE_FLAG "-O2")
set(WARNING_FLAGS "-Werror -Wpedantic -Wall -Wextra -Wno-zero-as-null-pointer-constant -Wno-unsafe-buffer-usage -Wno-c++98-compat-pedantic -Wno-documentation -Wno-documentation-unknown-command -Wno-nonportable-system-include-path -Wno-sign-conversion -Wno-shadow -Wno-cast-function-type-strict -Wno-old-style-cast -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZE_FLAG} ${WARNING_FLAGS}")

# Set Directories for Vulkan
set(VULKAN_DIR "C:\\VulkanSDK\\1.3.290.0\\Lib\\cmake")
set(VULKAN_INCLUDE_DIRS "C:\\VulkanSDK\\1.3.290.0\\Include")
set(VULKAN_LIB_DIR "C:\\VulkanSDK\\1.3.290.0\\Lib")
set(VULKAN_LIB "C:\\VulkanSDK\\1.3.290.0\\Lib\\vulkan-1.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\dxcompiler.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\GenericCodeGen.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\glslang-default-resource-limits.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\glslang.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\MachineIndependent.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\OSDependent.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\shaderc.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\shaderc_combined.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\shaderc_shared.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\shaderc_util.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-c-shared.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-c.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-core.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-cpp.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-glsl.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-hlsl.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-msl.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-reflect.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\spirv-cross-util.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-diff.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-link.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-lint.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-opt.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-reduce.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools-shared.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV-Tools.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPIRV.lib"
"C:\\VulkanSDK\\1.3.290.0\\Lib\\SPVRemapper.lib"
)

# Set Directories for GLFW
set(GLFW_INCLUDE_DIRS "C:\\vclib\\glfw-3.4.bin.WIN64\\include")
set(GLFW_LIB_DIR "C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022")

set(GLFW_LIB "C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3.lib" 
"C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3_mt.lib" 
"C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3dll.lib"
)

# GLEW
set(GLEW_INCLUDE_DIRS "C:\\vclib\\glew-2.1.0\\include")
set(GLEW_LIB_DIR "C:\\vclib\\glew-2.1.0\\lib")
set(GLEW_LIB "C:\\vclib\\glew-2.1.0\\lib\\Release\\x64\\glew32.lib"
"C:\\vclib\\glew-2.1.0\\lib\\Release\\x64\\glew32s.lib"
)

# GLM
set(GLM_INCLUDE_DIRS "C:\\vclib\\glm")
set(GLM_LIB_DIR "C:\\vclib\\glm")

find_package(Vulkan REQUIRED)

include_directories(${GLFW_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

add_executable(VulkanWindow 
	src/vulkan_window.cpp
	src/triangle_application.cpp
	src/triangle_application.hpp
)

target_link_libraries(${PROJECT_NAME} ${VULKAN_LIB} ${GLFW_LIB})

