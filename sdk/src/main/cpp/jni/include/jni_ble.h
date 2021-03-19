//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITER_SDK_JNI_BLE_H
#define JUBITER_SDK_JNI_BLE_H

#include "jni_comm.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


JNIEXPORT int JNICALL
native_BLEInitDevice(JNIEnv *env, jclass clz);

JNIEXPORT jint JNICALL
native_BLEStartScan(JNIEnv *env, jclass clz, jobject scanCallback);

JNIEXPORT jint JNICALL
native_BLEStopScan(JNIEnv *env, jclass clz);

JNIEXPORT jint JNICALL
native_BLEConnectDevice(JNIEnv *env, jclass clz, jstring name,
                        jstring deviceMAC, jintArray handle, jint timeout,
                        jobject disCallback);

JNIEXPORT jint JNICALL
native_BLECancelConnect(JNIEnv *env, jclass clz, jstring name, jstring address);

JNIEXPORT jint JNICALL
native_BLEDisconnectDevice(JNIEnv *env, jclass clz, jint deviceHandle);

JNIEXPORT jint JNICALL
native_BLEIsConnectDevice(JNIEnv *env, jclass clz, jint deviceHandle);



jclass getBleClass(JNIEnv *env);

std::vector<JNINativeMethod> getBleNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITER_SDK_JNI_BLE_H
