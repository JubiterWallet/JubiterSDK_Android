//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITERSDK_JNI_BLE_H
#define JUBITERSDK_JNI_BLE_H

#include "jni_comm.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus



jclass getBleClass(JNIEnv *env);

std::vector<JNINativeMethod> getBleNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_BLE_H
