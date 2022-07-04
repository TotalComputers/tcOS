project(tcOS)
cmake_minimum_required(VERSION 3.20)

add_subdirectory(thirdparty/libuv)
include_directories(thirdparty/libuv/include)

add_subdirectory(thirdparty/openssl)
include_directories(thirdparty/openssl/include)

add_subdirectory(thirdparty/zlib)
include_directories(thirdparty/zlib)

file(GLOB SRC *.cpp client/*.cpp client/protocol/*.cpp client/pipeline/*.cpp client/encryption/*.cpp client/graphics/*.cpp common/*.cpp)

add_executable(tcOS ${SRC})
target_link_libraries(tcOS uv crypto ssl zlib)