cmake_minimum_required(VERSION 3.2)
project(opengl)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCE_FILES src/main.cpp)

add_executable(opengl ${SOURCE_FILES})
target_link_libraries(opengl ${CONAN_LIBS})
