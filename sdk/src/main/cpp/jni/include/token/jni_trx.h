//
// Created by Journey on 2021/3/19.
//

#ifndef JUBITERSDK_JNI_TRX_H
#define JUBITERSDK_JNI_TRX_H

#include <jni.h>
#include "jni_comm.h"
#include <Tron.pb.h>
#include <Discover.pb.h>
#include <balance_contract.pb.h>
#include <asset_issue_contract.pb.h>
#include <smart_contract.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextTRX(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

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
                          jstring jPackedContractInPb);

JNIEXPORT jbyteArray JNICALL
native_TRXBuildTRC20Abi(JNIEnv *env, jclass clz, jint contextID, jstring tokenName,
                        jint unitDP,
                        jstring contractAddress,
                        jstring address,
                        jstring amount);

JNIEXPORT jbyteArray JNICALL
native_TRXPackContract(JNIEnv *env, jclass obj, jlong contextID, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_TRXBuildTRC721Abi(JNIEnv *env,
                         jclass clz,
                         jint contextID,
                         jstring tokenName,
                         jstring contractAddress,
                         jstring tokenFrom,
                         jstring tokenTo,
                         jstring tokenID);

jclass getTrxClass(JNIEnv *env);

std::vector<JNINativeMethod> getTrxNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITERSDK_JNI_TRX_H
