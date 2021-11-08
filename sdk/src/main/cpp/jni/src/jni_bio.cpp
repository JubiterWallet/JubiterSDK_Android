

#include "jni_bio.h"

#define BIO_NATIVE_CLASS "com/jubiter/sdk/jni/BioNativeApi"


JNIEXPORT jbyteArray JNICALL
native_BIOIdentityVerify(JNIEnv *env,
                         jclass clz,
                         jint deviceID,
                         jint mode) {
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_IdentityVerify(deviceID,
                                   static_cast<JUB_ENUM_IDENTITY_VERIFY_MODE>(mode),
                                   &retry);
    LOG_DEBUG("JUB_IdentityVerify rv: %d", rv);
    return buildPbRvUInt("JUB_IdentityVerify", env, rv, retry);
}

JNIEXPORT jbyteArray JNICALL
native_BIOIdentityVerifyPIN(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint mode,
                            jstring jPin) {
    JUB_ULONG retry = 0;
    auto strPin = jstring2stdString(env, jPin);
    JUB_RV rv = JUB_IdentityVerifyPIN(deviceID,
                                      static_cast<JUB_ENUM_IDENTITY_VERIFY_MODE>(mode),
                                      (JUB_CHAR_PTR) strPin.c_str(),
                                      &retry);
    LOG_DEBUG("JUB_IdentityVerifyPIN rv: %d", rv);
    return buildPbRvUInt("JUB_IdentityVerifyPIN", env, rv, retry);
}


JNIEXPORT jint JNICALL
native_BIOIdentityShowNineGrids(JNIEnv *env,
                                jclass clz,
                                jint deviceID) {
    JUB_RV rv = JUB_IdentityShowNineGrids(deviceID);
    LOG_DEBUG("JUB_IdentityShowNineGrids rv: %d", rv);
    return rv;
}

JNIEXPORT jint JNICALL
native_BIOIdentityCancelNineGrids(JNIEnv *env,
                                  jclass clz,
                                  jint deviceID) {
    JUB_RV rv = JUB_IdentityCancelNineGrids(deviceID);
    LOG_DEBUG("JUB_IdentityCancelNineGrids rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BIOEnrollFingerprint(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint fpTimeout,
                            jbyteArray fgptIndex) {
    JUB_BYTE *pFgptIndex = (JUB_BYTE *) env->GetByteArrayElements(fgptIndex, NULL);
    JUB_ULONG pTimes;
    JUB_BYTE pFgptID;
    JUB_RV rv = JUB_EnrollFingerprint(deviceID,
                                      fpTimeout,
                                      pFgptIndex,
                                      &pTimes,
                                      &pFgptID);
    LOG_DEBUG("JUB_EnrollFingerprint rv: %d", rv);

    JUB::Proto::Common::ResultAny resultState;
    resultState.set_state_code(rv);
    if (rv == JUBR_OK) {
        EnrollFpState state;
        state.set_next_index(*pFgptIndex);
        state.set_remaining_times(pTimes);
        state.set_modality_id(pFgptID);

        resultState.add_value()->PackFrom(state);
    }

    std::string result;
    resultState.SerializeToString(&result);
    return stdString2jbyteArray("JUB_EnrollFingerprint", env, result);
}


JNIEXPORT jbyteArray JNICALL
native_BIOEnumFingerprint(JNIEnv *env,
                          jclass clz,
                          jint deviceID) {
    JUB_CHAR_PTR fgptList = nullptr;
    JUB_RV rv = JUB_EnumFingerprint(deviceID,
                                    &fgptList);
    LOG_DEBUG("JUB_EnumFingerprint rv: %d", rv);
    return buildPbRvString("JUB_EnumFingerprint", env, rv, fgptList);
}

JNIEXPORT jint JNICALL
native_BIOEraseFingerprint(JNIEnv *env,
                           jclass clz,
                           jint deviceID,
                           jint fpTimeout) {
    JUB_RV rv = JUB_EraseFingerprint(deviceID,
                                     fpTimeout);
    LOG_DEBUG("JUB_EraseFingerprint rv: %d", rv);
    return rv;

}

JNIEXPORT jint JNICALL
native_BIODeleteFingerprint(JNIEnv *env,
                            jclass clz,
                            jint deviceID,
                            jint fpTimeout,
                            jbyte fgptID) {
    JUB_RV rv = JUB_DeleteFingerprint(deviceID,
                                      fpTimeout,
                                      fgptID);
    LOG_DEBUG("JUB_DeleteFingerprint rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BIOVerifyFgptForIntl(JNIEnv *env,
                            jclass clz,
                            jint deviceID) {
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_VerifyFgptForIntl(deviceID,
                                      &retry);
    LOG_DEBUG("JUB_VerifyFgptForIntl rv: %d", rv);
    return buildPbRvUInt("JUB_VerifyFgptForIntl", env, rv, retry);
}

JNIEXPORT jbyteArray JNICALL
native_BIOVerifyFingerprint(JNIEnv *env,
                            jclass clz,
                            jint contextID) {
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_VerifyFingerprint(contextID,
                                      &retry);
    LOG_DEBUG("JUB_VerifyFingerprint rv: %d", rv);
    return buildPbRvUInt("JUB_VerifyFingerprint", env, rv, retry);
}


JNINativeMethod BIONativeMethods[] = {
        {
                "nativeBIOIdentityVerify",
                "(II)[B",
                (void *) native_BIOIdentityVerify
        },
        {
                "nativeBIOIdentityVerifyPIN",
                "(IILjava/lang/String;)[B",
                (void *) native_BIOIdentityVerifyPIN
        },
        {
                "nativeBIOIdentityShowNineGrids",
                "(I)I",
                (void *) native_BIOIdentityShowNineGrids
        },
        {
                "nativeBIOIdentityCancelNineGrids",
                "(I)I",
                (void *) native_BIOIdentityCancelNineGrids
        },
        {
                "nativeBIOEnrollFingerprint",
                "(II[B)[B",
                (void *) native_BIOEnrollFingerprint
        },
        {
                "nativeBIOEnumFingerprint",
                "(I)[B",
                (void *) native_BIOEnumFingerprint
        },
        {
                "nativeBIOEraseFingerprint",
                "(II)I",
                (void *) native_BIOEraseFingerprint
        },
        {
                "nativeBIODeleteFingerprint",
                "(IIB)I",
                (void *) native_BIODeleteFingerprint
        },
        {
                "nativeBIOVerifyFgptForIntl",
                "(I)[B",
                (void *) native_BIOVerifyFgptForIntl
        },
        {
                "nativeBIOVerifyFingerprint",
                "(I)[B",
                (void *) native_BIOVerifyFingerprint
        },
};


jclass getBioClass(JNIEnv *env) {
    return env->FindClass(BIO_NATIVE_CLASS);
}

std::vector<JNINativeMethod> getBioNativeMethods() {
    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(BIONativeMethods) / sizeof(BIONativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(BIONativeMethods[i]);
    }
    return methodList;
}
