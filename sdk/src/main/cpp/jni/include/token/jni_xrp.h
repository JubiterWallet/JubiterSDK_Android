//
// Created by Journey on 2021/4/12.
//

#ifndef JUBITERSDK_JNI_XRP_H
#define JUBITERSDK_JNI_XRP_H

#include <jni.h>
#include "jni_comm.h"
#include "JUB_SDK_XRP.h"
#include <Jub_Ripple.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextXRP(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_GetAddressXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray format);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_CheckAddressXRP(JNIEnv *env, jclass clz, jint contextID, jstring address);

jclass getXrpClass(JNIEnv *env);

std::vector<JNINativeMethod> getXrpNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_XRP_H
