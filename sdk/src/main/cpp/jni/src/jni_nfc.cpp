//
// Created by Journey on 2020/9/14.
//

#include "include/jni_nfc.h"

#define NFC_NATIVE_CLASS "com/jubiter/sdk/jni/NfcNativeApi"

JNIEXPORT jint JNICALL
native_NFCInitDevice(JNIEnv *env, jclass clz, jobject initParam) {
    // 初始化参数转换
    NFC_DEVICE_INIT_PARAM nfcInitParam;
    jobjectToNFCInitParam(env, g_vm, initParam, &nfcInitParam);
    JUB_RV rv = JUB_initNFCDevice(nfcInitParam);
    LOG_DEBUG("JUB_initNFCDevice rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_NFCConnectDevice(JNIEnv *env, jclass clz, jstring deviceUUID) {
    auto strDeviceUUID = jstring2stdString(env, deviceUUID);
    JUB_UINT16 deviceID;
    JUB_RV rv = JUB_connectNFCDevice((JUB_BYTE_PTR) strDeviceUUID.c_str(), &deviceID);
    return buildPbRvUInt("JUB_connectNFCDevice", env, rv, deviceID);
}

JNIEXPORT jint JNICALL
native_NFCDisconnectDevice(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_RV rv = JUB_disconnectNFCDevice(deviceID);
    LOG_DEBUG("JUB_disconnectNFCDevice rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_NFCIsConnect(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_RV rv = JUB_isDeviceNFCConnect(deviceID);
    LOG_DEBUG("JUB_isDeviceNFCConnect rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_NFCReset(JNIEnv *env, jclass clz, jint contextID) {
    JUB_RV rv = JUB_Reset(contextID);
    LOG_DEBUG("JUB_Reset rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_NFCGenerateSeed(JNIEnv *env,
                       jclass clz,
                       jint deviceID,
                       jstring jPin,
                       jbyteArray curve) {
    std::string strCurve = jbyteArray2stdString(env, curve);
    auto strPin = jstring2stdString(env, jPin);

    JUB::Proto::Common::CURVES enum_curve;
    JUB::Proto::Common::CURVES_Parse(strCurve, &enum_curve);
    JUB_RV rv = JUB_GenerateSeed(deviceID, strPin.c_str(),
                                 static_cast<JUB_ENUM_CURVES>(enum_curve));
    LOG_DEBUG("JUB_GenerateSeed rv: %d", rv);
    return rv;
}

JNIEXPORT jint
native_NFCImportMnemonic(JNIEnv *env,
                         jclass clz,
                         jint deviceID,
                         jstring jPin,
                         jstring jMnemonic) {
    auto strPin = jstring2stdString(env, jPin);
    auto strMnemonic = jstring2stdString(env, jMnemonic);
    JUB_RV rv = JUB_ImportMnemonic(deviceID, strPin.c_str(), strMnemonic.c_str());
    LOG_DEBUG("JUB_ImportMnemonic rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_NFCExportMnemonic(JNIEnv *env,
                         jclass clz,
                         jint deviceID,
                         jstring jPin) {
    auto strPin = jstring2stdString(env, jPin);
    JUB_CHAR_PTR mnemonic;

    JUB_RV rv = JUB_ExportMnemonic(deviceID, strPin.c_str(), &mnemonic);
    return buildPbRvString("JUB_ExportMnemonic", env, rv, mnemonic);
}

JNIEXPORT jbyteArray JNICALL
native_NFCChangePIN(JNIEnv *env,
                    jclass clz,
                    jint deviceID,
                    jstring jOriginPin,
                    jstring jNewPin) {
    auto strOriginPin = jstring2stdString(env, jOriginPin);
    auto strNewPin = jstring2stdString(env, jNewPin);
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_ChangePIN(deviceID, strOriginPin.c_str(), strNewPin.c_str(), &retry);
    return buildPbRvUInt("JUB_ChangePIN", env, rv, retry);
}

JNIEXPORT jbyteArray JNICALL
native_NFCHasRootKey(JNIEnv *env,
                     jclass clz,
                     jint deviceID) {
    JUB_ENUM_DEVICE_ROOT_KEY_STATUS status;
    JUB_RV rv = JUB_GetDeviceRootKeyStatus(deviceID, &status);

    JUB::Proto::Common::ResultAny rootKeyState;
    rootKeyState.set_state_code(rv);

    if (rv == JUBR_OK) {
        JUB::Proto::Common::RootKeyStatus rootKeyStatus;
        if (status == JUB_ENUM_DEVICE_ROOT_KEY_STATUS::HAS_PIN) {
            rootKeyStatus.set_status(JUB::Proto::Common::RootKeyStatus::HAS_PIN);
        } else if (status == JUB_ENUM_DEVICE_ROOT_KEY_STATUS::HAS_ROOT_KEY) {
            rootKeyStatus.set_status(JUB::Proto::Common::RootKeyStatus::HAS_ROOT_KEY);
        } else if (status == JUB_ENUM_DEVICE_ROOT_KEY_STATUS::RESETTED) {
            rootKeyStatus.set_status(JUB::Proto::Common::RootKeyStatus::RESETTED);
        }
        rootKeyState.add_value()->PackFrom(rootKeyStatus);
    }

    std::string result;
    rootKeyState.SerializeToString(&result);
    return stdString2jbyteArray("JUB_GetRootKeyStatus", env, result);
}

JNIEXPORT jint JNICALL
native_NFCSetLabel(JNIEnv *env,
                   jclass clz,
                   jint deviceID,
                   jstring jLabel) {
    auto label = jstring2stdString(env, jLabel);
    JUB_RV rv = JUB_SetLabel(deviceID, label.c_str());
    LOG_DEBUG("JUB_SetLabel rv: %d", rv);
    return rv;
}

JNINativeMethod nfcNativeMethods[] = {
        {
                "nativeNFCInitDevice",
                "(Lcom/jubiter/sdk/jni/nfc/NFCInitParam;)I",
                (void *) native_NFCInitDevice
        },
        {
                "nativeNFCConnectDevice",
                "(Ljava/lang/String;)[B",
                (void *) native_NFCConnectDevice
        },
        {
                "nativeNFCDisconnectDevice",
                "(I)I",
                (void *) native_NFCDisconnectDevice
        },
        {
                "nativeNFCIsConnect",
                "(I)I",
                (void *) native_NFCIsConnect
        },
        {
                "nativeNFCReset",
                "(I)I",
                (void *) native_NFCReset
        },
        {
                "nativeNFCGenerateSeed",
                "(ILjava/lang/String;[B)I",
                (void *) native_NFCGenerateSeed
        },
        {
                "nativeNFCImportMnemonic",
                "(ILjava/lang/String;Ljava/lang/String;)I",
                (void *) native_NFCImportMnemonic
        },
        {
                "nativeNFCExportMnemonic",
                "(ILjava/lang/String;)[B",
                (void *) native_NFCExportMnemonic
        },
        {
                "nativeNFCChangePIN",
                "(ILjava/lang/String;Ljava/lang/String;)[B",
                (void *) native_NFCChangePIN
        },
        {
                "nativeNFCHasRootKey",
                "(I)[B",
                (void *) native_NFCHasRootKey
        },
        {
            "nativeNFCSetLabel",
            "(ILjava/lang/String;)I",
            (void *) native_NFCSetLabel
        }
};

jclass getNfcClass(JNIEnv *env) {
    return env->FindClass(NFC_NATIVE_CLASS);
}

std::vector<JNINativeMethod> getNfcNativeMethods() {
    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(nfcNativeMethods) / sizeof(nfcNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(nfcNativeMethods[i]);
    }
    return methodList;
}