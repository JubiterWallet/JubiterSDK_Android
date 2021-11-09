#ifndef JUBITER_SDK_JNI_SWI_H
#define JUBITER_SDK_JNI_SWI_H

#include "jni_comm.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_SWIConnectDevice(JNIEnv *env, jclass clz);

JNIEXPORT jint JNICALL
native_SWIDisconnectDevice(JNIEnv *env, jclass clz, jint deviceHandle);

JNIEXPORT jbyteArray JNICALL
native_GenerateMnemonic(JNIEnv *env,
                        jclass clz,
                        jbyteArray param);

JNIEXPORT jint JNICALL
native_CheckMnemonic(JNIEnv *env,
                     jclass clz,
                     jstring mnemonic);

JNIEXPORT jint JNICALL
native_SWIBuildFromMasterPrivateKey(JNIEnv *env, jclass clz,
                                    jint deviceHandle,
                                    jbyteArray curve,
                                    jstring masterPrivateKey);

JNIEXPORT jint JNICALL
native_SWIBuildFromMnemonic(JNIEnv *env, jclass clz,
                            jint deviceHandle,
                            jstring passphrase,
                            jstring mnemonic);


jclass getSwiClass(JNIEnv *env);

std::vector<JNINativeMethod> getSwiNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITER_SDK_JNI_SWI_H
