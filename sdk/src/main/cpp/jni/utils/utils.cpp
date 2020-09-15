//
// Created by FT on 2018/4/26.
//

#include <utils.h>
#include <JUB_SDK.h>
#include <logUtils.h>
#include <sstream>
#include <iostream>


static JavaVM *jVM = NULL;
static jobject j_ObjScanCalbak = NULL;
static jobject j_ObjDiscCalbak = NULL;
// 初始化参数
jobject j_InitParam    = NULL;


void setScanCallbackObj(JUB_VOID_PTR javaVM, jobject callback) {
    jVM  = (JavaVM *)javaVM;
    if (NULL != j_ObjScanCalbak) {
        j_ObjScanCalbak = NULL;
    }
    j_ObjScanCalbak = callback;
}

void setDiscCallbackObj(JUB_VOID_PTR javaVM, jobject callback) {
    jVM  = (JavaVM *)javaVM;
    j_ObjDiscCalbak = callback;
}


void callback(unsigned int type) {
    LOG_ERR("callback type: %d", type);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void**) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_InitParam);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        env->DeleteLocalRef(clazz);
        return;
    }

    jmethodID onCallbackID = env->GetMethodID(clazz, "funcCallBack", "(I)I");
    if (NULL == onCallbackID) {
        LOG_ERR("NULL == onCallbackID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jint operationType = type;
    env->CallIntMethod(j_InitParam, onCallbackID, operationType);
    env->DeleteLocalRef(clazz);
}

void scanCallback(JUB_BYTE_PTR devname, JUB_BYTE_PTR uuid, JUB_ULONG devType) {
    LOG_ERR("scanCallback devname: %s  uuid: %s  devType: %lu", devname, uuid, devType);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void**) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_ObjScanCalbak);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        env->DeleteLocalRef(clazz);
        return;
    }

    jmethodID onScanResultID = env->GetMethodID(clazz, "onScanResult", "(Ljava/lang/String;Ljava/lang/String;I)V");
    if (NULL == onScanResultID) {
        LOG_ERR("NULL == onScanResultID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jstring devName = env->NewStringUTF((const char *) devname);
    jstring devUuid = env->NewStringUTF((const char *) uuid);

    env->CallVoidMethod(j_ObjScanCalbak, onScanResultID, devName, devUuid, devType);
    env->DeleteLocalRef(devName);
    env->DeleteLocalRef(devUuid);
    env->DeleteLocalRef(clazz);
}

void disconnectCallback(JUB_BYTE_PTR devname) {
    LOG_ERR("disconnectCallback name: %s", devname);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void**) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_ObjDiscCalbak);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        return;
    }

    jmethodID onDisconnectID = env->GetMethodID(clazz, "onDisconnect", "(Ljava/lang/String;)V");
    if (NULL == onDisconnectID) {
        LOG_ERR("NULL == onDisconnectID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jstring devName = env->NewStringUTF((const char *) devname);
    env->CallVoidMethod(j_ObjDiscCalbak, onDisconnectID, devName);
    env->DeleteLocalRef(devName);
    env->DeleteLocalRef(clazz);
}


/**
 * NFC 扫描回调
 * @param errorCode
 * @param uuid
 * @param devType
 */
void nfcScanCallback(int errorCode, JUB_BYTE_PTR uuid, JUB_ULONG devType) {
    LOG_ERR("NFC callback errorCode: %d  uuid: %s  devType: %lu", errorCode, uuid, devType);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void**) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_ObjScanCalbak);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        env->DeleteLocalRef(clazz);
        return;
    }

    jmethodID onScanResultID = env->GetMethodID(clazz, "onScanResult", "(Ljava/lang/String;Ljava/lang/String;I)V");
    if (NULL == onScanResultID) {
        LOG_ERR("NULL == onScanResultID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jstring devUuid = env->NewStringUTF((const char *) uuid);

    env->DeleteLocalRef(devUuid);
    env->DeleteLocalRef(clazz);
}


/**
 * 蓝牙初始化参数
 * @param env
 * @param javaVM
 * @param param
 * @return
 */
JUB_RV jobjectToBLEInitParam(JNIEnv *env, JUB_VOID_PTR javaVM, DEVICE_INIT_PARAM *param) {

    CHECK_NULL(env, JUBR_ARGUMENTS_BAD)
    CHECK_NULL(javaVM, JUBR_ARGUMENTS_BAD)

    param->param = javaVM;
    param->scanCallBack = reinterpret_cast<DEV_ScanCallBack>(scanCallback);
    param->discCallBack = disconnectCallback;
    param->callBack = reinterpret_cast<DEV_ReadCallBack>(callback);

    return JUBR_OK;
}

/**
 * NFC 初始化参数
 * @param env
 * @param javaVM
 * @param param
 * @return
 */
JUB_RV jobjectToNFCInitParam(JNIEnv *env, JUB_VOID_PTR javaVM, NFC_DEVICE_INIT_PARAM *param) {

    CHECK_NULL(env, JUBR_ARGUMENTS_BAD)
    CHECK_NULL(javaVM, JUBR_ARGUMENTS_BAD)

    // 需传递上下文
    param->param = javaVM;
    param->crt = "7F2181E89310434552545F4150505F45434B41303031420D6A75626974657277616C6C65745F200D6A75626974657277616C6C6574950200805F2504202005255F2404202505245300BF200EEF0C8D0A820182028203820482057F4946B041048FD3FAB3907C5CC8CD193EB2B653EA179115B7F305C9E21DE6D29C0736A3B82025B219F24BDA86D80F5AE262521E124F4C6691A0C47B1FB72D95895E9312CB0DF001005F3746304402204D75EAA2F09604A9597DA905D680EB619B8ADCF080E5AD6950E1DBF26195C9E2022067649AFB4A8BC380B382520499C6F2BB350A8519B0ECDBE0B7374AA898826D0E";
    param->sk = "B66BE8CB7512A6DFF741839EE8C5092D6987A5D7790E93B52EBB16FCD4EAD7AA";
    param->keyLength = 16;
    param->hostID = "8080808080808080";
    param->scanCallBack = reinterpret_cast<NFC_ScanFuncCallBack >(nfcScanCallback);

    return JUBR_OK;
}

/**
 * 创建 java 结果对象
 * @param className
 * @param errorCode
 * @return
 */
jobject newJavaObject(JNIEnv *env, const char * className, int errorCode, jstring message) {
    if (nullptr == env) {
        return nullptr;
    }

    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOG_ERR(">>>> clazz == NULL");
        return nullptr;
    }
    LOG_ERR(">>>> FindClass success");

    jmethodID constructor = env->GetMethodID(clazz, "<init>", "(ILjava/lang/String;)V");
    jobject newObject = env->NewObject(clazz, constructor, errorCode, message);
    return newObject;
}

std::string CharPtr2HexStr(std::vector<unsigned char> v) {

    std::stringstream ss;
    ss << std::hex;

    size_t len = v.size();
    for (size_t i(0); i < len; ++i) {
        if (0x0F >= (int)v[i]) {
            ss << 0;
        }
        ss << (int)v[i];
    }
    return ss.str();
}

std::string CharPtr2HexStr(unsigned char* data,size_t len){
    std::vector<unsigned char> v(data,data+len);
    return CharPtr2HexStr(v);
}

std::vector<unsigned char> HexStr2CharPtr(std::string str) {
    std::vector<unsigned char> v;

    std::string strHex;
    auto l = str.size();
    if (l%2) {
        strHex = "0";
    }
    else {
        strHex = "";
    }
    strHex += str;
    unsigned int c;
    std::string sub;
    for (int i = 0; i < l; i += 2) {
        sub = strHex.substr(i, 2);
        std::istringstream hex_chars_stream(sub.c_str());
        hex_chars_stream >> std::hex >> c;
        v.push_back(c);
    }

    return v;
}