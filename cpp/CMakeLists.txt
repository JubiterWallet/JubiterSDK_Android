project(JuBiterSDK)

cmake_minimum_required(VERSION 3.6.0)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/protobuf_cpp/)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/protobuf_cpp/protobuf/cmake)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/trezor-crypto)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Jub_SDK_CXX/src)