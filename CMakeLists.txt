project(tcOS)
cmake_minimum_required(VERSION 3.20)

file(GLOB SRC *.c *.cpp *.c *.h *.hpp)

add_subdirectory(thirdparty/libuv)
include_directories(thirdparty/libuv/include)

add_executable(tcOS ${SRC})
target_link_libraries(tcOS uv)