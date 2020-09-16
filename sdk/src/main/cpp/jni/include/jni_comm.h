//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITERSDK_JNI_COMM_H
#define JUBITERSDK_JNI_COMM_H

#include <jni.h>
#include <string>
#include <JUB_SDK_COMM.h>
#include <Jub_Common.pb.h>
#include <utils/utils.h>
#include "utils/logUtils.h"


extern JavaVM *g_vm;

std::string jbyteArray2stdString(JNIEnv *env, jbyteArray jbytes);

jbyteArray stdString2jbyteArray(JNIEnv *env, std::string str);

jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, JUB_CHAR_PTR str);

jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, std::string str);

jbyteArray buildPbRvUInt(JNIEnv *env, JUB_RV rv, uint32_t res);

std::string jstring2stdString(JNIEnv *env, jstring jstr);

//template<typename T>
//bool parseFromJbyteArray(JNIEnv *env, jbyteArray jbytes, T *pb);

//bool parseBip44Path(JNIEnv *env, jbyteArray jbytes, BIP44_Path *bip44Path);

#endif //JUBITERSDK_JNI_COMM_H
