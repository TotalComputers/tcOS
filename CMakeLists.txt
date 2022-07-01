project(tcOS)
cmake_minimum_required(VERSION 3.20)

add_subdirectory(thirdparty/libuv)
include_directories(thirdparty/libuv/include)

add_executable(tcOS main.cpp client/ByteBuffer.cpp client/client.cpp client/protocol/protocol.cpp client/protocol/packet.cpp client/protocol/clientbound_con_success.cpp client/protocol/clientbound_creation_request.cpp client/protocol/clientbound_destroy.cpp)
target_link_libraries(tcOS uv)