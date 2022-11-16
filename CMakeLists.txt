cmake_minimum_required(VERSION 3.14)
project(Fluid_simulation) #

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/FluidCube2D.h src/FluidCube2D.cpp src/test.cpp) #
add_executable(Fluid_simulation ${SOURCE_FILES}) #
include_directories(/opt/homebrew/Cellar/sfml/2.5.1_2/include) #

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Fluid_simulation sfml-system sfml-window sfml-graphics sfml-audio sfml-network) #