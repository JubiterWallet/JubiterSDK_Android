//
// Created by FT on 2018/4/26.
//

#ifndef JUBITER_UTILS_H
#define JUBITER_UTILS_H


#include <jni.h>
#include <JUB_SDK.h>
#include <JUB_core.h>
#include <string>
#include <vector>


#ifndef CHECK_NULL
#define CHECK_NULL(OBJ, ERR) if (OBJ == NULL) {return ERR;}
#endif





void setScanCallbackObj(JUB_VOID_PTR javaVM, jobject callback);

void setDiscCallbackObj(JUB_VOID_PTR javaVM, jobject callback);

void callback(unsigned int type);

void scanCallback(JUB_BYTE_PTR devname, JUB_BYTE_PTR uuid, JUB_ULONG devType);

void disconnectCallback(JUB_BYTE_PTR devname);

int getMnemonicStrength(jint strength);

JUB_RV jobjectToBLEInitParam(JNIEnv *env, JUB_VOID_PTR javaVM, DEVICE_INIT_PARAM *param);

JUB_RV jobjectToNFCInitParam(JNIEnv *env, JUB_VOID_PTR javaVM, NFC_DEVICE_INIT_PARAM *param);

jobject newJavaObject(JNIEnv *env, const char * className, int errorCode, jstring message);

std::string CharPtr2HexStr(std::vector<unsigned char> v);
std::string CharPtr2HexStr(unsigned char* data,size_t len);

std::vector<unsigned char> HexStr2CharPtr(std::string str);


#endif //JUBITER_UTILS_H
