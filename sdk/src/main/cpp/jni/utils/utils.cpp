//
// Created by FT on 2018/4/26.
//

#include <utils.h>
#include <JUB_SDK.h>
#include <logUtils.h>
#include <sstream>
#include <iostream>


static JavaVM *jVM = NULL;
static jobject j_BleScanCalbak = NULL;
static jobject j_BleDiscCalbak = NULL;
// 初始化参数
jobject j_BleInitParam = NULL;

jobject j_NfcInitParam = NULL;
static jobject jActivity = NULL;
static jobject j_NfcScanCallback = NULL;
static jobject j_NfcDiscCallback = NULL;

// BLE
void setBleScanCallback(JUB_VOID_PTR javaVM, jobject callback) {
    jVM = (JavaVM *) javaVM;
    if (NULL != j_BleScanCalbak) {
        j_BleScanCalbak = NULL;
    }
    j_BleScanCalbak = callback;
}

void setBleDiscCallback(JUB_VOID_PTR javaVM, jobject callback) {
    jVM = (JavaVM *) javaVM;
    j_BleDiscCalbak = callback;
}

// NFC
void setNfcInitParam(JavaVM *javaVM, jobject initParam) {
    jVM = javaVM;

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    // 获取 activity
    // todo: 不使用 NewGlobalRef 调用就崩溃
    jclass clz = env->GetObjectClass(initParam);
    jfieldID activityID = env->GetFieldID(clz, "mContext", "Landroid/app/Activity;");
    jActivity = env->GetObjectField(initParam, activityID);

    jfieldID scanCallbackID = env->GetFieldID(clz, "mNfcScanCallback",
                                              "Lcom/jubiter/sdk/jni/nfc/NfcScanCallback;");
    jobject scanCallback = env->GetObjectField(initParam, scanCallbackID);
    j_NfcScanCallback = env->NewGlobalRef(scanCallback);

    jfieldID discCallbackID = env->GetFieldID(clz, "mNfcDiscCallback",
                                              "Lcom/jubiter/sdk/jni/nfc/NfcDiscCallback;");
    jobject discCallback = env->GetObjectField(initParam, discCallbackID);
    j_NfcDiscCallback = env->NewGlobalRef(discCallback);
}


void callback(unsigned int type) {
    LOG_ERR("callback type: %d", type);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_BleInitParam);
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
    env->CallIntMethod(j_BleInitParam, onCallbackID, operationType);
    env->DeleteLocalRef(clazz);
}

void bleScanCallback(JUB_BYTE_PTR name, JUB_BYTE_PTR uuid, JUB_ULONG devType) {
    LOG_ERR("scanCallback devName: %s  uuid: %s  devType: %lu", name, uuid, devType);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_BleScanCalbak);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        env->DeleteLocalRef(clazz);
        return;
    }

    jmethodID onScanResultID = env->GetMethodID(clazz, "onScanResult",
                                                "(Ljava/lang/String;Ljava/lang/String;I)V");
    if (NULL == onScanResultID) {
        LOG_ERR("NULL == onScanResultID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jstring devName = env->NewStringUTF((const char *) name);
    jstring devUuid = env->NewStringUTF((const char *) uuid);

    env->CallVoidMethod(j_BleScanCalbak, onScanResultID, devName, devUuid, devType);
    env->DeleteLocalRef(devName);
    env->DeleteLocalRef(devUuid);
    env->DeleteLocalRef(clazz);
}

void bleDiscCallback(JUB_BYTE_PTR devname) {
    LOG_ERR("disconnectCallback name: %s", devname);

    JNIEnv *env = NULL;
    if ((jVM->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_BleDiscCalbak);
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
    env->CallVoidMethod(j_BleDiscCalbak, onDisconnectID, devName);
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
    if ((jVM->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        LOG_ERR("GetEnv failed");
        return;
    }

    jclass clazz = env->GetObjectClass(j_NfcScanCallback);
    if (NULL == clazz) {
        LOG_ERR("NULL == clazz");
        env->DeleteLocalRef(clazz);
        return;
    }

    jmethodID onScanResultID = env->GetMethodID(clazz, "onScanResult",
                                                "(ILjava/lang/String;I)V");
    if (NULL == onScanResultID) {
        LOG_ERR("NULL == onScanResultID");
        env->DeleteLocalRef(clazz);
        clazz = NULL;
        return;
    }

    jstring devUuid = env->NewStringUTF((const char *) uuid);
    env->CallVoidMethod(j_NfcScanCallback, onScanResultID, errorCode, devUuid, devType);

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
JUB_RV jobjectToBLEInitParam(JNIEnv *env, JavaVM *javaVM, DEVICE_INIT_PARAM *param) {

    CHECK_NULL(env, JUBR_ARGUMENTS_BAD)
    CHECK_NULL(javaVM, JUBR_ARGUMENTS_BAD)

    param->param = javaVM;
    param->scanCallBack = reinterpret_cast<DEV_ScanCallBack>(bleScanCallback);
    param->discCallBack = bleDiscCallback;
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
JUB_RV jobjectToNFCInitParam(JNIEnv *env, JavaVM *javaVM, jobject initParam,
                             NFC_DEVICE_INIT_PARAM *param) {

    CHECK_NULL(env, JUBR_ARGUMENTS_BAD)
    CHECK_NULL(javaVM, JUBR_ARGUMENTS_BAD)

    // 设置回调
    setNfcInitParam(javaVM, initParam);

    param->jvm = javaVM;
    param->context = &jActivity;
    param->crt = "7F2181DD93104A754269746572323030363234536572420F7777772E6674736166652E636F6D305F200E7777772E6674736166652E636F6D950200805F2504202006245F2404202505245300BF20007F4946B041044CC8DE78E2CF8E8DFB98A8B0F33892B30C95D1A6E4ACF89596DD9A27BB5EEFC3C077A907974AF9F4EC89A6C07BC57446C571906E74232705AFCE04EEC2E8DD65F001005F3746304402200CA51AEF1C2BD29C98BA7BA0B9E1944B534C0D069329512524702ABF796B7E1602207C6738EBE9B2C5D17D4228CC17EECE18F1BBB2B0B25F5C8E88A6F460B92154B3";
    param->sk = "267B28D8BC760F6DA3C5C3D2CA30EF78357FF1F93F4BD9C6103F01C32E10EAB4";
    param->keyLength = 16;
    param->hostID = "8080808080808080";
    param->cardGroupID = "6a75626974657277616c6c6574";
    param->scanCallBack = reinterpret_cast<NFC_ScanFuncCallBack >(nfcScanCallback);

    return JUBR_OK;
}

/**
 * 创建 java 结果对象
 * @param className
 * @param errorCode
 * @return
 */
jobject newJavaObject(JNIEnv *env, const char *className, int errorCode, jstring message) {
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
        if (0x0F >= (int) v[i]) {
            ss << 0;
        }
        ss << (int) v[i];
    }
    return ss.str();
}

std::string CharPtr2HexStr(unsigned char *data, size_t len) {
    std::vector<unsigned char> v(data, data + len);
    return CharPtr2HexStr(v);
}

std::vector<unsigned char> HexStr2CharPtr(std::string str) {
    std::vector<unsigned char> v;

    std::string strHex;
    auto l = str.size();
    if (l % 2) {
        strHex = "0";
    } else {
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