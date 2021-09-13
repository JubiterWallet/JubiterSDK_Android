#ifndef JUBITER_SDK_JNI_BIO_H
#define JUBITER_SDK_JNI_BIO_H

#include "jni_comm.h"
#include <jubiter_blue.pb.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

JNIEXPORT jbyteArray JNICALL
native_BIOIdentityVerify(JNIEnv *env,
                         jclass clz,
                         jint deviceID,
                         jint mode);

JNIEXPORT jbyteArray JNICALL
native_BIOIdentityVerifyPIN(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint mode,
                            jstring jPin);

JNIEXPORT jint JNICALL
native_BIOIdentityShowNineGrids(JNIEnv *env,
                                jclass clz,
                                jint deviceID);

JNIEXPORT jint JNICALL
native_BIOIdentityCancelNineGrids(JNIEnv *env,
                                  jclass clz,
                                  jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_BIOEnrollFingerprint(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint fpTimeout,
                            jbyteArray fgptIndex);


JNIEXPORT jbyteArray JNICALL
native_BIOEnumFingerprint(JNIEnv *env,
                          jclass clz,
                          jint deviceID);

JNIEXPORT jint JNICALL
native_BIOEraseFingerprint(JNIEnv *env,
                           jclass clz,
                           jint deviceID,
                           jint fpTimeout);

JNIEXPORT jint JNICALL
native_BIODeleteFingerprint(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint fpTimeout,
                            jbyte fgptID);

JNIEXPORT jbyteArray JNICALL
native_BIOVerifyFgptForIntl(JNIEnv *env,
                            jclass clz,
                            jint deviceID);

JNIEXPORT jbyteArray JNICALL
native_BIOVerifyFingerprint(JNIEnv *env,
                            jclass clz,
                            jint contextID);

jclass getBioClass(JNIEnv *env);

std::vector <JNINativeMethod> getBioNativeMethods();

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif //JUBITER_SDK_JNI_BIO_H
