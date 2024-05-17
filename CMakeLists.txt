cmake_minimum_required(VERSION 3.12)
project(OpenGLExploration)

# Set the output directories for the build files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Export compile commands for use with tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include FetchContent module
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Fetch cglm library
fetchcontent_declare(
  cglm
  GIT_REPOSITORY https://github.com/recp/cglm.git
  GIT_TAG v0.8.5
  GIT_PROGRESS TRUE
)

# Populate and add cglm subdirectory if not already done
if(NOT cglm_POPULATED)
  message(STATUS "Populating cglm...")
  fetchcontent_populate(cglm)
  add_subdirectory(${cglm_SOURCE_DIR} ${cglm_BINARY_DIR})
endif()

# Find and link GLFW
find_package(glfw3 3.4 REQUIRED)

# Add your source files
set(SOURCES
    src/glad.c
    src/shader.c
    src/stb_image.c
    src/textures.c
)

# Include directories
include_directories(include ${cglm_SOURCE_DIR}/include)

# Add the executable
add_executable(MyGLApp ${SOURCES})

# Link libraries
target_link_libraries(MyGLApp PRIVATE glfw cglm)

# Link math library (libm)
target_link_libraries(MyGLApp PRIVATE m)

