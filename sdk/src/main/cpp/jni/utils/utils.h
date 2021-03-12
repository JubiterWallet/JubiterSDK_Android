//
// Created by FT on 2018/4/26.
//

#ifndef JUBITER_UTILS_H
#define JUBITER_UTILS_H


#include <jni.h>
#include <JUB_SDK.h>
#include <JUB_CORE.h>
#include <string>
#include <vector>

#ifndef CHECK_NULL
#define CHECK_NULL(OBJ, ERR) if (OBJ == NULL) {return ERR;}
#endif


void setBleScanCallback(JUB_VOID_PTR javaVM, jobject callback);

void setBleDiscCallback(JUB_VOID_PTR javaVM, jobject callback);

void callback(unsigned int type);

void bleScanCallback(JUB_BYTE_PTR devname, JUB_BYTE_PTR uuid, JUB_ULONG devType);

void bleDiscCallback(JUB_BYTE_PTR devname);

void setNfcInitParam(JavaVM *javaVM, jobject initParam);

int getMnemonicStrength(jint strength);

JUB_RV jobjectToBLEInitParam(JNIEnv *env, JavaVM* javaVM, DEVICE_INIT_PARAM *param);

JUB_RV jobjectToNFCInitParam(JNIEnv *env, JavaVM* javaVM, jobject initParam, NFC_DEVICE_INIT_PARAM *param);

jobject newJavaObject(JNIEnv *env, const char * className, int errorCode, jstring message);

std::string CharPtr2HexStr(std::vector<unsigned char> v);
std::string CharPtr2HexStr(unsigned char* data,size_t len);

std::vector<unsigned char> HexStr2CharPtr(std::string str);


#endif //JUBITER_UTILS_H
