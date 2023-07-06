cmake_minimum_required(VERSION 3.1...3.26)

project(Denoiser VERSION 1.0
    DESCRIPTION "Denoiser"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(Denoiser SHARED src/denoiser_library/denoiser.hpp src/denoiser_library/denoiser.cpp src/denoiser_library/lib.cpp)
target_link_libraries(Denoiser PRIVATE ${CMAKE_SOURCE_DIR}/libs/oidn/lib/OpenImageDenoise.lib)
include_directories(${CMAKE_SOURCE_DIR}/libs/oidn/include)