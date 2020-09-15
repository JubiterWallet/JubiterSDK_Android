//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITERSDK_JNI_BLE_H
#define JUBITERSDK_JNI_BLE_H


#include <jni.h>
#include <JUB_SDK.h>
#include <utils/utils.h>
#include "jni_comm.h"
#include "utils/logUtils.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

std::vector<JNINativeMethod> getBleNativeMethods();



#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_BLE_H
