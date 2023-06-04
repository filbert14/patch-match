cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(pm)

add_library(pm STATIC)
target_sources(pm PUBLIC src/pm.cpp src/pm_helper.cpp)
target_include_directories(pm PUBLIC src)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC pm)

### Eigen
find_package(Eigen3 REQUIRED)
target_include_directories(pm PUBLIC ${Eigen_INCLUDE_DIRS})
target_link_libraries(pm PUBLIC ${Eigen_LIBRARIES})

### OpenCV
find_package(OpenCV REQUIRED)
target_link_libraries(pm PUBLIC ${OpenCV_LIBRARIES})