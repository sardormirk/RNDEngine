cmake_minimum_required(VERSION 3.27)

project(OpenGLProj)
set(CMAKE_CXX_STANDARD 17)
add_executable(main "Source/main.cpp" "Source/glad.c")

set(glm_DIR "${CMAKE_SOURCE_DIR}/library/glm" ) # if necessary
find_package(glm REQUIRED)
target_link_libraries(main PRIVATE glm::glm)

find_library (
    GLFW_LIB
    NAMES libglfw.3.3.dylib # what to look for
    HINTS "${CMAKE_SOURCE_DIR}/library" # where to look
    NO_DEFAULT_PATH # do not search system default paths
)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include/")

target_link_libraries(main PRIVATE ${GLFW_LIB})