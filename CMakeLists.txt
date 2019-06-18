cmake_minimum_required(VERSION 3.13)
project(pa3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cs221util)
include_directories(cs221util/lodepng)

add_executable(pa3
        cs221util/lodepng/lodepng.cpp
        cs221util/lodepng/lodepng.h
        cs221util/catch.hpp
        cs221util/HSLAPixel.cpp
        cs221util/HSLAPixel.h
        cs221util/json.hpp
        cs221util/PNG.cpp
        cs221util/PNG.h
        cs221util/RGB_HSL.h
        cs221util/RGBAPixel.cpp
        cs221util/RGBAPixel.h
        main.cpp
        stats.cpp
        stats.h
        testComp.cpp
        toqutree.cpp
        toqutree.h)
