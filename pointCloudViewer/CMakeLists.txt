cmake_minimum_required(VERSION 2.8)

project ("PointCloudViewer")

set(GLFW_SOURCE_DIR "external/glfw")
set(GLM_SOURCE_DIR "external/glm")
set(GLEW_SOURCE_DIR "external/glew-2.1.0")


# Building only the GLFW lib
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

add_subdirectory(${GLFW_SOURCE_DIR} ${GLEW_SOURCE_DIR})

include_directories(${GLFW_SOURCE_DIR}/include
                    ${GLFW_SOURCE_DIR}/deps
                    ${GLM_SOURCE_DIR}
                    ${GLEW_SOURCE_DIR}/include
                    ./include)

add_library(glew STATIC
  ${GLEW_SOURCE_DIR}/src/glew.c
  ${GLEW_SOURCE_DIR}/include)
#set(CMAKE_PREFIX_PATH "C:/Users/a.refaat/projects/pointCloudViewer/assimp/out/install/x64-Debug/lib/cmake/Assimp")

#set_target_properties(assimp PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(glew ${GLFW_LIBRARIES})

add_definitions(
  -DGLEW_STATIC
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
find_package(assimp CONFIG REQUIRED)
#include_directories("./external/assimp/out/install/x64-Debug/include")
#add_subdirectory(external/assimp/)


set(APP_NAME "pointCloudViewer")
#add_executable(${APP_NAME} "SimpleDrawShaderMgr.cpp" "src/parser.cpp" "src/GlShaderMgr.cpp")
add_executable(${APP_NAME} "main.cpp" "src/parser.cpp" "src/GlShaderMgr.cpp" "src/camera.cpp" "src/renderer.cpp" "src/windowMgr.cpp")


target_link_libraries(${APP_NAME} glfw ${GLFW_LIBRARIES} glew)
target_link_libraries(${APP_NAME} PRIVATE assimp::assimp)