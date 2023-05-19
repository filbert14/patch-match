cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(pm)

add_library(pm STATIC)
target_include_directories(pm PUBLIC src)
target_link_libraries(pm PUBLIC DearImGui)

add_executable(pmgui src/pmgui.cpp)
target_link_libraries(pmgui PUBLIC pm)

### Dear ImGui
add_library(DearImGui STATIC)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_SRC_DIR lib/imgui)
file(GLOB_RECURSE IMGUI_SRC_FILES ${IMGUI_SRC_DIR}/*.cpp)
target_sources(DearImGui PUBLIC ${IMGUI_SRC_FILES})
set(IMGUI_INCLUDE_DIRS ${IMGUI_SRC_DIR} ${IMGUI_SRC_DIR}/backends)

target_include_directories(DearImGui PUBLIC
        ${IMGUI_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR})

target_link_libraries(DearImGui PUBLIC
        ${CMAKE_DL_LIBS}
        ${SDL2_LIBRARIES}
        ${OPENGL_LIBRARIES})