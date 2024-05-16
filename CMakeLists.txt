cmake_minimum_required(VERSION 3.12)
project(OpenGLExploration)

# Find GLFW package
find_package(glfw3 3.4 REQUIRED)

# Add your source files
set(SOURCES src/glad.c src/shader.c src/stb_image.c src/textures.c)

# Include directory
include_directories(include)

# Add the executable
add_executable(MyGLApp ${SOURCES})

# Link GLFW
target_link_libraries(MyGLApp PRIVATE glfw)

