project(JuBiterSDK)

cmake_minimum_required(VERSION 3.6.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/protos/protobuf_java/cmake)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/protos)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/trezor-crypto)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/src)