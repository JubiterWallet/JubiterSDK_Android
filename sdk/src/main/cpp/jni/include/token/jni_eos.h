//
// Created by Journey on 2021/4/12.
//

#ifndef JUBITERSDK_JNI_EOS_H
#define JUBITERSDK_JNI_EOS_H

#include <jni.h>
#include "jni_comm.h"
#include <Jub_EOS.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextEOS(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_GetAddressEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray format);

//JNIEXPORT jbyteArray JNICALL
//native_SetMyAddressEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_BuildActionEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_CalculateMemoHashEOS(JNIEnv *env, jclass clz, jstring memo);

jclass getEosClass(JNIEnv *env);

std::vector<JNINativeMethod> getEosNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_EOS_H
