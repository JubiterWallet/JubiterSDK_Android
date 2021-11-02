
#ifndef JUBITERSDK_JNI_CKB_H
#define JUBITERSDK_JNI_CKB_H

#include <jni.h>
#include "jni_comm.h"
#include <Jub_NervosCKB.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_CreateContextCKB(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeCKB(JNIEnv *env, jclass clz, jint contextID);

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeCKB(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32);

JNIEXPORT jbyteArray JNICALL
native_GetAddressCKB(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow);

JNIEXPORT int JNICALL
native_CheckAddressCKB(JNIEnv *env, jclass clz, jint contextID, jstring address);

JNIEXPORT jbyteArray JNICALL
native_SignTransactionCKB(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx);


jclass getCKBClass(JNIEnv *env);

std::vector<JNINativeMethod> getCKBNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif //JUBITERSDK_JNI_CKB_H
