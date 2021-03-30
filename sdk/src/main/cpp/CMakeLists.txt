project(coreNDK)

cmake_minimum_required(VERSION 3.6.0)

# **************************************************************************************************
#                               引入相关头文件
# 注：因为是分开编译且涉及相互调用，所以需要引入其他库模块中的头文件
# **************************************************************************************************

# 引入外部库路径
set(OUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../../cpp/JubiterSDK)
message(">>>外部目录： ${OUT_DIRECTORY}")

#############################################################################################

#############################################################################################

include_directories(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp)
include_directories(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/Tron/protocol)
include_directories(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core)
include_directories(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract)
#include_directories(${OUT_DIRECTORY}/deps/protobuf/src)
include_directories(${OUT_DIRECTORY}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni/include/token)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jni/utils)


# **************************************************************************************************
#                      查找指定目录下的所有源文件，并将其存入指定变量中
# **************************************************************************************************

aux_source_directory(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp SRC_LIST)
aux_source_directory(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core SRC_LIST)
aux_source_directory(${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/Tron/protocol/core/contract SRC_LIST)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni/src SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni/src/token SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/jni/utils SRC_LIST)

#file(SRC_LIST to_remove ${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/jubiter_bridge.grpc.pb.cc)
# 删除无用的 grpc 文件
list(REMOVE_ITEM SRC_LIST ${OUT_DIRECTORY}/deps/JubiterSDK_Common_pb/cpp/jubiter_bridge.grpc.pb.cc)


# **************************************************************************************************
#                                添加其他预构建库（第三方库）
# **************************************************************************************************

find_library(libLog
        log
        )


# 需要改成平台自适应的宏
if(CMAKE_BUILD_TYPE MATCHES "Debug"
        OR CMAKE_BUILD_TYPE MATCHES "None")
    message(">>>> link debug library")
    link_directories(${OUT_DIRECTORY}/deps/bleTransmit/android/${ANDROID_ABI}/debug)
    link_directories(${OUT_DIRECTORY}/deps/nfcTransmit/android/${ANDROID_ABI}/debug)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    message(">>>> link release library")
    link_directories(${OUT_DIRECTORY}/deps/bleTransmit/android/${ANDROID_ABI}/release)
    link_directories(${OUT_DIRECTORY}/deps/nfcTransmit/android/${ANDROID_ABI}/release)
endif()

# 连接 protobuf 静态库
#link_directories(${OUT_DIRECTORY}/protos/libs/${ANDROID_ABI})

# **************************************************************************************************
#                                        生成动态库
# **************************************************************************************************

add_library(
        coreNDK
        SHARED
        ${SRC_LIST}
)


# **************************************************************************************************
#                                        链接所有相关库
# 切记链接日志库，否则编译报错 undefined reference to '__android_log_print'
# **************************************************************************************************

target_link_libraries(
        coreNDK
        JUB_SDK
        TrezorCrypto
        BTSDK
        NFCSDK
        libprotobuf
        ${libLog}
)