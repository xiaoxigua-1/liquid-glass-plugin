cmake_minimum_required(VERSION 3.27)

project(liquid-glass
    DESCRIPTION "Liquid Glass plugin for Hyprland"
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "*.cpp")
file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/shaders/*")

set(SHADERS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/generated")
set(SHADERS_OUTPUT_FILE "${SHADERS_OUTPUT_DIR}/shaders.h")

add_custom_command(
    OUTPUT ${SHADERS_OUTPUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_FILE=${SHADERS_OUTPUT_FILE} -DSHADERS="${SHADERS}" -P ${CMAKE_SOURCE_DIR}/embed_shaders.cmake
    DEPENDS ${SHADERS} ${CMAKE_SOURCE_DIR}/embed_shaders.cmake
    COMMENT "Embedding GLSL shaders into ${SHADERS_OUTPUT_FILE}"
)

add_custom_target(embed_shaders DEPENDS ${SHADERS_OUTPUT_FILE})
add_library(liquid-glass SHARED ${SRC})
add_dependencies(liquid-glass embed_shaders)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET
    hyprland
    libdrm
    libinput
    libudev
    pangocairo
    pixman-1
    wayland-server
    xkbcommon
)

target_include_directories(liquid-glass PRIVATE ${SHADERS_OUTPUT_DIR})
target_link_libraries(liquid-glass PRIVATE rt PkgConfig::deps)

install(TARGETS liquid-glass)
