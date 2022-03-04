//
// Created by Journey on 2021/4/12.
//

#ifndef JUBITERSDK_JNI_ETH_H
#define JUBITERSDK_JNI_ETH_H


#include <jni.h>
#include "jni_comm.h"
#include "JUB_SDK_ETH.h"
#include <Jub_Ethereum.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray format);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetAddressETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_BuildERC20AbiETH(JNIEnv *env, jclass clz, jint contextID, jstring tokenName,
                        jint unitDP,
                        jstring contractAddress,
                        jstring address,
                        jstring amountInWei);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_SignByteStringETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jstring data);

JNIEXPORT jbyteArray JNICALL
native_SignTypedDataETH(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jbyteArray bip32,
                        jstring data,
                        jboolean enableTypedData_V4);

JNIEXPORT jbyteArray JNICALL
native_SignContractETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);

JNIEXPORT jbyteArray JNICALL
native_BuildERC721AbiETH(JNIEnv *env,
                         jclass clz,
                         jint contextID,
                         jstring tokenName,
                         jstring contractAddress,
                         jstring tokenFrom,
                         jstring tokenTo,
                         jstring tokenID);


jclass getEthClass(JNIEnv *env);

std::vector<JNINativeMethod> getEthNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif //JUBITERSDK_JNI_ETH_H
