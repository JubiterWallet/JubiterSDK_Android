//
// Created by Journey on 2020/9/14.
//
#include "jni_comm.h"

// 保存 JavaVM
JavaVM *g_vm = NULL;



bool parseBip44Path(JNIEnv *env, jbyteArray jbytes, BIP44_Path *bip44Path) {
    JUB::Proto::Common::Bip44Path pbBip44Path;
    bool rv = parseFromJbyteArray(env, jbytes, &pbBip44Path);
    if (rv) {
        bip44Path->addressIndex = pbBip44Path.address_index();
        bip44Path->change = (JUB_ENUM_BOOL) pbBip44Path.change();
    }
    return rv;
}


std::string jbyteArray2stdString(JNIEnv *env, jbyteArray jbytes) {
    jbyte *data = (jbyte *) env->GetByteArrayElements(jbytes, 0);
    jsize size = env->GetArrayLength(jbytes);
    return std::string(data, data + size);
}

jbyteArray stdString2jbyteArray(std::string funcName, JNIEnv *env, std::string str) {
    if (funcName.length() != 0) {
        LOG_ERR("%s value: %s", funcName.c_str(), str.c_str());
    }

    jbyteArray jarray = env->NewByteArray(str.size());
    jsize size = str.size();
    env->SetByteArrayRegion(jarray, 0, size, (const jbyte *) &str[0]);
    return jarray;
}


jbyteArray buildPbRvString(std::string funcName, JNIEnv *env, JUB_RV rv, JUB_CHAR_PTR str) {
    if (str == nullptr) {
        LOG_ERR("%s rv: %d", funcName.c_str(), rv);
    } else {
        LOG_ERR("%s rv: %d, value: %s", funcName.c_str(), rv, str);
    }

    JUB::Proto::Common::ResultString resultString;
    resultString.set_state_code(rv);
    if (JUBR_OK == rv) {
        resultString.set_value(str);
    }

    std::string result;
    resultString.SerializeToString(&result);
    if (nullptr != str) {
        JUB_FreeMemory(str);
    }
    return stdString2jbyteArray("", env, result);
}

jbyteArray buildPbRvString(std::string funcName, JNIEnv *env, JUB_RV rv, std::string str) {
    LOG_ERR("%s rv: %d, value: %s", funcName.c_str(), rv, str.c_str());

    JUB::Proto::Common::ResultString resultString;
    resultString.set_state_code(rv);
    if (JUBR_OK == rv) {
        resultString.set_value(str);
    }

    std::string result;
    resultString.SerializeToString(&result);
    return stdString2jbyteArray("", env, result);
}

jbyteArray buildPbRvUInt(std::string funcName, JNIEnv *env, JUB_RV rv, uint32_t res) {
    LOG_ERR("%s rv: %d, value: %d", funcName.c_str(), rv, res);
    JUB::Proto::Common::ResultInt resultInt;
    resultInt.set_state_code(rv);
    resultInt.set_value(res);

    std::string result;
    resultInt.SerializeToString(&result);
    return stdString2jbyteArray("", env, result);
}

std::string jstring2stdString(JNIEnv *env, jstring jstr) {
    char *pStr = const_cast<char *>(env->GetStringUTFChars(jstr, NULL));
    std::string stdString = pStr;
    env->ReleaseStringUTFChars(jstr, pStr);
    return stdString;
}
