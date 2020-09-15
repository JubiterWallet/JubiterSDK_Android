//
// Created by Journey on 2020/9/14.
//

#include "include/jni_nfc.h"

JNIEXPORT jint JNICALL native_NFCInitDevice(JNIEnv *env, jclass clz, jobject initParam) {
    // 初始化参数转换
    NFC_DEVICE_INIT_PARAM nfcInitParam;
    jobjectToNFCInitParam(env, g_vm, initParam, &nfcInitParam);
    JUB_RV rv = JUB_initNFCDevice(nfcInitParam);
    return rv;
}

JNIEXPORT jbyteArray JNICALL native_NFCConnectDevice(JNIEnv *env, jclass clz, jstring deviceUUID) {
    auto strDeviceUUID = jstring2stdString(env, deviceUUID);
    JUB_UINT16 deviceID;
    JUB_RV rv = JUB_connectNFCDevice((JUB_BYTE_PTR) strDeviceUUID.c_str(), &deviceID);
    return buildPbRvUInt(env, rv, deviceID);
}

JNIEXPORT jint JNICALL native_NFCDisconnectDevice(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_RV rv = JUB_disconnectNFCDevice(deviceID);
    return rv;
}

JNIEXPORT jint JNICALL native_NFCIsConnect(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_RV rv = JUB_isDeviceNFCConnect(deviceID);
    return rv;
}

JNIEXPORT jint JNICALL native_NFCReset(JNIEnv *env, jclass clz, jint contextID) {
    JUB_RV rv = JUB_Reset(contextID);
    return rv;
}

JNIEXPORT jint JNICALL
native_NFCGenerateSeed(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jbyteArray curve) {
    std::string strCurve = jbyteArray2stdString(env, curve);
    auto strPin = jstring2stdString(env, jPin);

    JUB::Proto::Common::CURVES enum_curve;
    JUB::Proto::Common::CURVES_Parse(strCurve, &enum_curve);
    JUB_RV rv = JUB_GenerateSeed(deviceID, strPin.c_str(),
                                 static_cast<JUB_ENUM_CURVES>(enum_curve));
    return rv;
}

JNIEXPORT jint
native_NFCImportMnemonic(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jstring jMnemonic) {
    auto strPin = jstring2stdString(env, jPin);
    auto strMnemonic = jstring2stdString(env, jMnemonic);
    JUB_RV rv = JUB_ImportMnemonic(deviceID, strPin.c_str(), strMnemonic.c_str());
    return rv;
}


JNIEXPORT jbyteArray
native_NFCExportMnemonic(JNIEnv *env, jclass clz, jint deviceID, jstring jPin, jstring jMnemonic) {
    auto strPin = jstring2stdString(env, jPin);
    JUB_CHAR_PTR mnemonic = nullptr;

    JUB_RV rv = JUB_ExportMnemonic(deviceID, strPin.c_str(), &mnemonic);
    return buildPbRvString(env, rv, mnemonic);
}

JNIEXPORT jint
native_NFCChangePIN(JNIEnv *env, jclass clz, jint deviceID, jstring jOriginPin, jstring jNewPin) {
    auto strOriginPin = jstring2stdString(env, jOriginPin);
    auto strNewPin = jstring2stdString(env, jNewPin);
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_ChangePIN(deviceID, strOriginPin.c_str(), strNewPin.c_str(), &retry);
}


JNINativeMethod nfcNativeMethods[] = {
        {
                "nativeNFCInitDevice",
                "(Lcom/jubiter/sdk/jni/NFCInitParam;)I",
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
                "(ILjava/lang/String;Ljava/lang/String;)[B",
                (void *) native_NFCExportMnemonic
        },
        {
                "nativeNFCChangePIN",
                "(ILjava/lang/String;Ljava/lang/String;)I",
                (void *) native_NFCChangePIN
        },
};

std::vector<JNINativeMethod> getNfcNativeMethods() {
    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(nfcNativeMethods) / sizeof(nfcNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(nfcNativeMethods[i]);
    }
    return methodList;
}