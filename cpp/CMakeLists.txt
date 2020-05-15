project(JuBiterSDK)

cmake_minimum_required(VERSION 3.6.0)

set (CMAKE_CXX_STANDARD 14)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/JubiterSDK/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/JubiterSDK/trezor-crypto)
#set (protobuf_BUILD_TESTS OFF)
#set (protobuf_BUILD_PROTOC_BINARIES OFF)
option(protobuf_BUILD_TESTS "Build tests" OFF)
option(protobuf_BUILD_PROTOC_BINARIES "Build libprotoc and protoc compiler" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Protobuffer/cmake)