//
// Created by Journey on 2021/4/12.
//

#ifndef JUBITERSDK_JNI_BTC_H
#define JUBITERSDK_JNI_BTC_H


#include <jni.h>
#include "jni_comm.h"
#include <Jub_Bitcoin.pb.h>
#include "JUB_SDK_BTC.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL native_GetMainHDNodeBTC(JNIEnv *env, jclass clz, jint contextID);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetAddressBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jint JNICALL
native_CheckAddressBTC(JNIEnv *env, jclass clz, jint contextID, jstring address);

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray jTX);

JNIEXPORT jint JNICALL
native_SetUnitBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray junit);

JNIEXPORT jbyteArray JNICALL
native_BuildUSDTOutputs(JNIEnv *env, jclass clz, jint contextID, jstring USDTTO, jlong amount);

JNIEXPORT jbyteArray JNICALL
native_BuildQRC20Output(JNIEnv *env, jclass clz, jint contextID,
                        jstring contractAddr, jint decimal,
                        jstring symbol, jlong gasLimit,
                        jlong gasPrice, jstring to,
                        jstring amount);


jclass getBtcClass(JNIEnv *env);

std::vector<JNINativeMethod> getBtcNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_BTC_H
