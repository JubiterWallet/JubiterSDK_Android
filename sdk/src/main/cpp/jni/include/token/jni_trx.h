//
// Created by Journey on 2021/3/19.
//

#ifndef JUBITERSDK_JNI_TRX_H
#define JUBITERSDK_JNI_TRX_H

#include <jni.h>
#include "jni_comm.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextTRX(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_CreateContextTRX_soft(JNIEnv *env, jclass clz, jbyteArray jcfg, jstring xprv);

JNIEXPORT jbyteArray JNICALL
native_GetAddressTRX(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jbyteArray JNICALL
native_CheckAddressTRX(JNIEnv *env, jclass clz, jint contextID, jstring address);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeTRX(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeTRX(JNIEnv *env, jclass clz, jint contextID, jbyteArray format);

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressTRX(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionTRX(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32,
                          jbyteArray jPackedContractInPb);

JNIEXPORT jbyteArray JNICALL
native_TRXBuildTRC20Abi(JNIEnv *env, jclass clz, jint contextID, jstring tokenName,
                        jint unitDP,
                        jstring contractAddress,
                        jstring address,
                        jstring amount);

//JNIEXPORT jbyteArray JNICALL
//native_TRXPackContract(JNIEnv *env, jclass obj, jlong contextID, jbyteArray tx);


jclass getTrxClass(JNIEnv *env);

std::vector<JNINativeMethod> getTrxNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_TRX_H
