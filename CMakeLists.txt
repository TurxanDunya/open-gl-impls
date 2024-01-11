cmake_minimum_required(VERSION 3.5)

project(OPEN-GL-IMPLS)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROjECT_INCLUDE_DIR ${PROJECT_DIR}/include)
set(PROjECT_SOURCE_DIR ${PROJECT_DIR}/src-3d)
set(PROJECT_SOURCES 
            ${PROjECT_SOURCE_DIR}/main.cpp
            ${PROjECT_SOURCE_DIR}/vendor/glad.c
            ${PROjECT_SOURCE_DIR}/shaderprogram/program.cpp
            ${PROjECT_SOURCE_DIR}/shapes/square.cpp
            ${PROjECT_SOURCE_DIR}/texture/texturemanager.cpp)
set(PROJECT_HEADERS
            ${PROjECT_SOURCE_DIR}/shaderprogram/program.hpp
            ${PROjECT_SOURCE_DIR}/shapes/square.hpp
            ${PROjECT_SOURCE_DIR}/texture/texturemanager.hpp)

find_package(glfw3 3.2 REQUIRED)
find_package(glm REQUIRED)

include_directories(${PROjECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)