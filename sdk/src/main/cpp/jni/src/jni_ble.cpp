//
// Created by Journey on 2020/9/14.
//
#include "include/jni_ble.h"

#define BLE_NATIVE_CLASS "com/jubiter/sdk/jni/BleNativeApi"

JNIEXPORT int JNICALL
native_BLEInitDevice(JNIEnv *env, jclass clz) {
    LOG_ERR(">>> in native_BLEInitDevice");

    DEVICE_INIT_PARAM initParam;

    // 初始化参数转换
    jobjectToBLEInitParam(env, g_vm, &initParam);

    JUB_RV rv = JUB_initDevice(initParam);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_initDevice ret: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL
native_BLEStartScan(JNIEnv *env, jclass clz, jobject scanCallback) {
    jobject objParam = env->NewGlobalRef(scanCallback);
    setBleScanCallback(g_vm, objParam);

    JUB_RV rv = JUB_enumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_enumDevices rv: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL
native_BLEStopScan(JNIEnv *env, jclass clz) {
    JUB_RV rv = JUB_stopEnumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_stopEnumDevices rv: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL
native_BLEConnectDevice(JNIEnv *env,
                        jclass clz,
                        jstring name,
                        jstring deviceMAC,
                        jintArray handle,
                        jint timeout,
                        jobject disCallback) {
    JUB_BYTE_PTR pName = (JUB_BYTE_PTR) (env->GetStringUTFChars(name, NULL));
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) (env->GetStringUTFChars(deviceMAC, NULL));
    JUB_UINT16 *pHandle = reinterpret_cast<JUB_UINT16 *>(env->GetIntArrayElements(handle, NULL));

    jobject objParam = env->NewGlobalRef(disCallback);
    setBleDiscCallback(g_vm, objParam);

    LOG_DEBUG("JUB_connectDevice name: %s", pName);
    LOG_DEBUG("JUB_connectDevice address: %s", pAddress);
    LOG_DEBUG("JUB_connectDevice timeout: %ld", timeout);

    JUB_RV rv = JUB_connectDevice(pName, pAddress, 1, pHandle, timeout);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_connectDevice rv: %08lx", rv);
        env->ReleaseIntArrayElements(handle, reinterpret_cast<jint *>(pHandle), 0);
        return rv;
    }
    env->ReleaseIntArrayElements(handle, reinterpret_cast<jint *>(pHandle), 0);
    return rv;
}

JNIEXPORT jint JNICALL
native_BLECancelConnect(JNIEnv *env,
                        jclass clz,
                        jstring name,
                        jstring address) {
    JUB_BYTE_PTR pName = (JUB_BYTE_PTR) env->GetStringUTFChars(name, NULL);
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) env->GetStringUTFChars(address, NULL);

    JUB_RV rv = JUB_cancelConnect(pName, pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_cancelConnect rv: %08lx", rv);
        env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
        return rv;
    }

    env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
    return rv;
}


JNIEXPORT jint JNICALL
native_BLEDisconnectDevice(JNIEnv *env, jclass clz, jint deviceHandle) {
    JUB_RV rv = JUB_disconnectDevice(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_disconnectDevice rv: %08lx", rv);
    }
    return rv;
}


JNIEXPORT jint JNICALL
native_BLEIsConnectDevice(JNIEnv *env,
                          jclass clz,
                          jint deviceHandle) {
    JUB_RV rv = JUB_isDeviceConnect(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_isDeviceConnect rv: %08lx", rv);
    }
    return rv;
}


JNINativeMethod bleNativeMethods[] = {
        {
                "nativeBLEInitDevice",
                "()I",
                (void *) native_BLEInitDevice
        },
        {
                "nativeBLEStartScan",
                "(Lcom/jubiter/sdk/jni/ble/InnerScanCallback;)I",
                (void *) native_BLEStartScan
        },
        {
                "nativeBLEStopScan",
                "()I",
                (void *) native_BLEStopScan
        },
        {
                "nativeBLEConnectDevice",
                "(Ljava/lang/String;Ljava/lang/String;[IILcom/jubiter/sdk/jni/ble/InnerDiscCallback;)I",
                (void *) native_BLEConnectDevice
        },
        {
                "nativeBLEDisconnectDevice",
                "(I)I",
                (void *) native_BLEDisconnectDevice
        },
        {
                "nativeBLEIsConnected",
                "(I)I",
                (void *) native_BLEIsConnectDevice
        },
        {
                "nativeBLECancelConnect",
                "(Ljava/lang/String;Ljava/lang/String;)I",
                (void *) native_BLECancelConnect
        },
};


jclass getBleClass(JNIEnv *env) {
    return env->FindClass(BLE_NATIVE_CLASS);
}

std::vector<JNINativeMethod> getBleNativeMethods() {
    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(bleNativeMethods) / sizeof(bleNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(bleNativeMethods[i]);
    }
    return methodList;
}
