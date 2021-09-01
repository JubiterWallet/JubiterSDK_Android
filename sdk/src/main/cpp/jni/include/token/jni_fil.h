//
// Created by Journey on 2021/6/28.
//

#ifndef JUBITERSDK_JNI_FIL_H
#define JUBITERSDK_JNI_FIL_H

#include <jni.h>
#include "jni_comm.h"
#include "Jub_Filecoin.pb.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextFIL(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeFIL(JNIEnv *env, jclass clz, jint contextID, jbyteArray format);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeFIL(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetAddressFIL(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionFIL(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);

//JNIEXPORT jbyteArray JNICALL
//native_BuildERC721AbiETH(JNIEnv *env,
//                         jclass clz,
//                         jint contextID,
//                         jstring tokenName,
//                         jstring contractAddress,
//                         jstring tokenFrom,
//                         jstring tokenTo,
//                         jstring tokenID);
//
//
jclass getFilClass(JNIEnv *env);

std::vector<JNINativeMethod> getFilNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif //JUBITERSDK_JNI_FIL_H
