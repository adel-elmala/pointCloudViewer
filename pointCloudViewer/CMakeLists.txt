cmake_minimum_required(VERSION 3.0.0)

project ("PointCloudViewer")


include_directories(./include)


find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(tinyply CONFIG REQUIRED)


#include_directories(${GLEW_INCLUDE_DIRS})
#include_directories(${GLFW_INCLUDE_DIRS})
#include_directories(${OPENGL_INCLUDE_DIRS})

set(APP_NAME "pointCloudViewer")


add_library(parser "src/parser.cpp" "include/parser.h")
target_link_libraries(parser PUBLIC assimp::assimp)

add_library(parser2 "src/parser2.cpp" "include/parser2.h")
target_link_libraries(parser2 PUBLIC tinyply)

add_library(camera "src/camera.cpp" "include/camera.h")
target_link_libraries(camera PUBLIC glm::glm)

add_library(renderer "src/renderer.cpp" "include/renderer.h")
target_link_libraries(renderer PUBLIC GLEW::GLEW glm::glm)

add_library(windowMgr "src/windowMgr.cpp" "include/windowMgr.h")
target_link_libraries(windowMgr PUBLIC GLEW::GLEW glfw camera)

add_library(shaderMgr  "src/GlShaderMgr.cpp" "include/GlShaderMgr.h")
target_link_libraries(shaderMgr PUBLIC GLEW::GLEW glfw)

add_library(shaderMgr2 "src/shaderMgr.cpp" "include/shaderMgr.h")
target_link_libraries(shaderMgr2 PUBLIC GLEW::GLEW glfw)

add_library(gui "src/gui.cpp" "include/gui.h")
target_link_libraries(gui PUBLIC imgui::imgui)


add_executable(${APP_NAME} "main.cpp" )


target_link_libraries(${APP_NAME} PRIVATE parser shaderMgr2 camera renderer windowMgr gui parser2)

