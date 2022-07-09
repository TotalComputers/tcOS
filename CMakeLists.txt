project(tcOS)
cmake_minimum_required(VERSION 3.20)

add_subdirectory(thirdparty/libuv)
include_directories(thirdparty/libuv/include)

add_subdirectory(thirdparty/openssl)
include_directories(thirdparty/openssl/include)

add_subdirectory(thirdparty/zlib)
include_directories(thirdparty/zlib)

add_subdirectory(thirdparty/glad)
include_directories(thirdparty/glad/include)

add_subdirectory(thirdparty/glfw)
include_directories(thirdparty/glfw/include)

add_subdirectory(thirdparty/glm)
include_directories(thirdparty/glm)

file(GLOB SRC *.cpp client/*.cpp client/protocol/*.cpp client/pipeline/*.cpp client/encryption/*.cpp client/graphics/*.cpp common/*.cpp graphics/*.cpp graphics/internal/*.cpp graphics/ui/*.cpp)

add_executable(tcOS ${SRC})
target_link_libraries(tcOS uv crypto ssl zlibstatic glad glfw)