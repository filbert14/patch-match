cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(pm)

### Example applications
add_executable(reconstruction src/reconstruction.cpp)
target_link_libraries(reconstruction PUBLIC pm)

add_executable(inpainting src/inpainting.cpp)
target_link_libraries(inpainting PUBLIC inp)

### PatchMatch
add_library(pm STATIC)
target_sources(pm PUBLIC src/pm.cpp src/pm_helper.cpp)
target_include_directories(pm PUBLIC src)

### Inpainting
add_library(inp STATIC)
target_sources(inp PUBLIC src/inp.cpp)
target_include_directories(inp PUBLIC src)
target_link_libraries(inp PUBLIC pm)

### Eigen
find_package(Eigen3 REQUIRED)
target_include_directories(pm PUBLIC ${Eigen_INCLUDE_DIRS})
target_link_libraries(pm PUBLIC ${Eigen_LIBRARIES})

### OpenCV
find_package(OpenCV REQUIRED)
target_link_libraries(pm PUBLIC ${OpenCV_LIBRARIES})