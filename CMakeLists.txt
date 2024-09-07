cmake_minimum_required(VERSION 3.29)
# Set the project Name
project(VulkanWindow)

# Set the C++ Standard to compile against
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C++ Flags
set(OPTIMIZE_FLAG "-O2")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# Clang compiler
	set(WARNING_FLAGS "-Werror -Wpedantic -Wall -Wextra -Wno-zero-as-null-pointer-constant -Wno-unsafe-buffer-usage -Wno-c++98-compat-pedantic -Wno-documentation -Wno-documentation-unknown-command -Wno-nonportable-system-include-path -Wno-sign-conversion -Wno-shadow -Wno-cast-function-type-strict -Wno-old-style-cast -Wno-unused-parameter")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# MSVC compiler
	set(WARNING_FLAGS "/Wall /Wv:19.31")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZE_FLAG} ${WARNING_FLAGS}")

# Set Directories for Vulkan
set(VULKAN_DIR "C:\\VulkanSDK\\1.3.290.0\\Lib\\cmake")
set(VULKAN_INCLUDE_DIRS "C:\\VulkanSDK\\1.3.290.0\\Include")
set(VULKAN_LIB_DIR "C:\\VulkanSDK\\1.3.290.0\\Lib")
set(VULKAN_LIB "vulkan-1.lib")

# Set Directories for GLFW
set(GLFW_INCLUDE_DIRS "C:\\vclib\\glfw-3.4.bin.WIN64\\include")
set(GLFW_LIB_DIR "C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022")

set(GLFW_LIBS "C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3.lib"
"C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3_mt.lib" 
"C:\\vclib\\glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3dll.lib"
)

# GLM
set(GLM_INCLUDE_DIRS "C:\\vclib\\glm")
set(GLM_LIB_DIR "C:\\vclib\\glm")

find_package(Vulkan REQUIRED)

include_directories(SYSTEM ${GLFW_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
link_directories(${VULKAN_LIB_DIR} ${GLFW_LIB_DIR} ${GLM_LIB_DIR})

file(COPY src/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lint_codebase.ps1 DESTINATION ${CMAKE_BINARY_DIR})

add_executable(VulkanWindow 
	src/vulkan_window.cpp
	src/triangle_application.cpp
	src/triangle_application.hpp
)

target_link_libraries(${PROJECT_NAME} ${VULKAN_LIB} ${GLFW_LIBS})

