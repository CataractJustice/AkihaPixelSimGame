cmake_minimum_required(VERSION 3.22.1)
project(Akiha)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES GameAssets/*.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -Wno-volatile")

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ./EngineLink)
target_include_directories(${PROJECT_NAME} PUBLIC ./GameAssets)
