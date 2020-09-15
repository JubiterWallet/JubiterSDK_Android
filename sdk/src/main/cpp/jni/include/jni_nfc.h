//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITERSDK_JNI_NFC_H
#define JUBITERSDK_JNI_NFC_H

#include <jni.h>
#include <JUB_SDK.h>
#include "jni_comm.h"
#include "utils/logUtils.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jint JNICALL native_NFCInitDevice(JNIEnv *env, jclass clz, jobject initParam);

JNIEXPORT jbyteArray JNICALL native_NFCConnectDevice(JNIEnv *env, jclass clz, jstring deviceUUID);

JNIEXPORT jint JNICALL native_NFCDisconnectDevice(JNIEnv *env, jclass clz, jint deviceID);

JNIEXPORT jint JNICALL native_NFCIsConnect(JNIEnv *env, jclass clz, jint deviceID);

JNIEXPORT jint JNICALL native_NFCReset(JNIEnv *env, jclass clz, jint contextID);

JNIEXPORT jint JNICALL
native_NFCGenerateSeed(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jbyteArray curve);

JNIEXPORT jint
native_NFCImportMnemonic(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jstring jMnemonic);

JNIEXPORT jbyteArray
native_NFCExportMnemonic(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jstring jMnemonic);

JNIEXPORT jint
native_NFCChangePIN(JNIEnv *env, jclass clz, jint deviceID, jstring jOriginPin, jstring jNewPin);


std::vector<JNINativeMethod> getNfcNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_NFC_H
