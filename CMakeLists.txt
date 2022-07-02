project(tcOS)
cmake_minimum_required(VERSION 3.20)

add_subdirectory(thirdparty/libuv)
include_directories(thirdparty/libuv/include)

file(GLOB SRC *.cpp client/*.cpp client/protocol/*.cpp client/pipeline/*.cpp)

add_executable(tcOS ${SRC})
target_link_libraries(tcOS uv)