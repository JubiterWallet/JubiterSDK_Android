//
// Created by Journey on 2020/9/14.
//

#ifndef JUBITER_SDK_JNI_COMM_H
#define JUBITER_SDK_JNI_COMM_H

#include <jni.h>
#include <string>
#include <JUB_SDK_COMM.h>
#include <Jub_Common.pb.h>
#include <utils/utils.h>
#include "utils/logUtils.h"


extern JavaVM *g_vm;

bool parseBip44Path(JNIEnv *env, jbyteArray jbytes, BIP44_Path *bip44Path);

std::string jbyteArray2stdString(JNIEnv *env, jbyteArray jbytes);

jbyteArray stdString2jbyteArray(std::string funcName, JNIEnv *env, std::string str);

jbyteArray buildPbRvString(std::string funcName, JNIEnv *env, JUB_RV rv, JUB_CHAR_PTR str);

jbyteArray buildPbRvString(std::string funcName, JNIEnv *env, JUB_RV rv, std::string str);

jbyteArray buildPbRvUInt(std::string funcName, JNIEnv *env, JUB_RV rv, uint32_t res);

std::string jstring2stdString(JNIEnv *env, jstring jstr);

template<typename T>
bool parseFromJbyteArray(JNIEnv *env, jbyteArray jbytes, T *pb) {
    auto pStr = jbyteArray2stdString(env, jbytes);
    return pb->ParseFromString(pStr);
}

#endif //JUBITER_SDK_JNI_COMM_H
