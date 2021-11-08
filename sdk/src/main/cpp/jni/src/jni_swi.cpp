#include "jni_swi.h"

#define SWI_NATIVE_CLASS "com/jubiter/sdk/jni/SwiNativeApi"

JNIEXPORT jbyteArray JNICALL
native_SWIConnectDevice(JNIEnv *env, jclass clz){
    JUB_UINT16 deviceID;
    JUB_RV rv = JUB_ConnetDeviceSWI(&deviceID);
    return buildPbRvUInt("JUB_ConnetDeviceSWI", env, rv, deviceID);
}

JNIEXPORT jint JNICALL
native_SWIDisconnectDevice(JNIEnv *env, jclass clz, jint deviceHandle){
    JUB_RV rv = JUB_DisconnetDeviceSWI(deviceHandle);
    LOG_DEBUG("JUB_DisconnetDeviceSWI rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_SWIBuildFromMasterPrivateKey(JNIEnv *env, jclass clz,
                                    jint deviceHandle,
                                    jbyteArray curve,
                                    jstring masterPrivateKey){
    std::string strCurve = jbyteArray2stdString(env, curve);
    auto strMasterPrivateKey = jstring2stdString(env, masterPrivateKey);

    JUB::Proto::Common::CURVES enum_curve;
    JUB::Proto::Common::CURVES_Parse(strCurve, &enum_curve);
    JUB_RV rv = JUB_BuildFromMasterPrivateKey(deviceHandle,
                                 static_cast<JUB_ENUM_CURVES>(enum_curve),
                                 strMasterPrivateKey.c_str());
    LOG_DEBUG("JUB_BuildFromMasterPrivateKey rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_SWIBuildFromMnemonic(JNIEnv *env, jclass clz,
                            jint deviceHandle,
                            jstring passphrase,
                            jstring mnemonic){
    auto strPassphrase = jstring2stdString(env, passphrase);
    auto strMnemonic = jstring2stdString(env, mnemonic);
    JUB_RV rv = JUB_BuildFromMnemonic(deviceHandle, strPassphrase.c_str(), strMnemonic.c_str());
    LOG_DEBUG("JUB_BuildFromMnemonic rv: %d", rv);
    return rv;

}


JNINativeMethod swiNativeMethods[] = {

        {
                "nativeSWIConnectDevice",
                "()[B",
                (void *) native_SWIConnectDevice
        },
        {
                "nativeSWIDisconnectDevice",
                "(I)I",
                (void *) native_SWIDisconnectDevice
        },
        {
                "nativeSWIBuildFromMasterPrivateKey",
                "(I[BLjava/lang/String;)I",
                (void *) native_SWIBuildFromMasterPrivateKey
        },
        {
                "nativeSWIBuildFromMnemonic",
                "(ILjava/lang/String;Ljava/lang/String;)I",
                (void *) native_SWIBuildFromMnemonic
        },
};


jclass getSwiClass(JNIEnv *env) {
    return env->FindClass(SWI_NATIVE_CLASS);
}

std::vector<JNINativeMethod> getSwiNativeMethods() {
    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(swiNativeMethods) / sizeof(swiNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(swiNativeMethods[i]);
    }
    return methodList;
}
