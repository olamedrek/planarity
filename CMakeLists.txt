cmake_minimum_required(VERSION 3.6)
project(planarity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Graph.cpp Graph.h PlanarityTest.cpp PlanarityTest.h CoordinatesBuilder.cpp CoordinatesBuilder.h Embedding.cpp Embedding.h main.cpp check_drawing.cpp check_drawing.h)
add_executable(planarity ${SOURCE_FILES})