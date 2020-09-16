//
// Created by Journey on 2020/9/14.
//
#include "jni_comm.h"

// 保存 JavaVM
JavaVM *g_vm = NULL;


std::string jbyteArray2stdString(JNIEnv *env, jbyteArray jbytes) {
    jbyte *data = (jbyte *) env->GetByteArrayElements(jbytes, 0);
    jsize size = env->GetArrayLength(jbytes);
    return std::string(data, data + size);
}

jbyteArray stdString2jbyteArray(JNIEnv *env, std::string str) {
    jbyteArray jarray = env->NewByteArray(str.size());
    jsize size = str.size();
    env->SetByteArrayRegion(jarray, 0, size, (const jbyte *) &str[0]);
    return jarray;
}


jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, JUB_CHAR_PTR str) {
    JUB::Proto::Common::ResultString resultString;
    resultString.set_state_code(rv);
    if (JUBR_OK == rv) { resultString.set_value(str); }

    std::string result;
    resultString.SerializeToString(&result);
    if (nullptr != str) {
        JUB_FreeMemory(str);
    }
    return stdString2jbyteArray(env, result);
}

jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, std::string str) {
    JUB::Proto::Common::ResultString resultString;
    resultString.set_state_code(rv);
    if (JUBR_OK == rv) { resultString.set_value(str); }

    std::string result;
    resultString.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
}

jbyteArray buildPbRvUInt(JNIEnv *env, JUB_RV rv, uint32_t res) {
    LOG_ERR("rv: %d, value: %d", rv, res);
    JUB::Proto::Common::ResultInt resultInt;
    resultInt.set_state_code(rv);
    resultInt.set_value(res);

    std::string result;
    resultInt.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
}

std::string jstring2stdString(JNIEnv *env, jstring jstr) {
    char *pStr = const_cast<char *>(env->GetStringUTFChars(jstr, NULL));
    std::string stdString = pStr;
    env->ReleaseStringUTFChars(jstr, pStr);
    return stdString;
}
