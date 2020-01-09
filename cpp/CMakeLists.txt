project(JuBiterSDK)

cmake_minimum_required(VERSION 3.6.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/JubiterSDK/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/JubiterSDK/trezor-crypto)
set (protobuf_BUILD_TESTS OFF)
set (protobuf_BUILD_PROTOC_BINARIES OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/protobuffer/cmake)