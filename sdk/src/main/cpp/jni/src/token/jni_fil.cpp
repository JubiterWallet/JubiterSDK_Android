//
// Created by Journey on 2021/6/28.
//

#include "jni_fil.h"

JNIEXPORT jbyteArray JNICALL
native_CreateContextFIL(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_FIL cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextFIL(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextFIL 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextFIL 2", env, JUBR_ARGUMENTS_BAD, 0);
    }
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeFIL(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeFIL(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeFIL", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeFIL(JNIEnv *env,
                    jclass clz,
                    jint contextID,
                    jbyteArray format,
                    jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeFIL(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeFIL 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeFIL 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressFIL(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressFIL(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressFIL 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressFIL 2", env, JUBR_ARGUMENTS_BAD, "");
}

//JNIEXPORT jbyteArray JNICALL
//native_SetMyAddressFIL(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
//    BIP44_Path bip32Path;
//    if (parseBip44Path(env, bip32, &bip32Path)) {
//        JUB_CHAR_PTR address = nullptr;
//        JUB_RV rv = JUB_SetMyAddressFIL(contextID, bip32Path, &address);
//        return buildPbRvString("JUB_SetMyAddressFIL 1", env, rv, address);
//    }
//    return buildPbRvString("JUB_SetMyAddressFIL 2", env, JUBR_ARGUMENTS_BAD, "");
//}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionFIL(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray tx) {
    JUB::Proto::Filecoin::TransactionFIL pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        BIP44_Path bip32Path;
        bip32Path.change = (JUB_ENUM_BOOL) pbTx.path().change();
        bip32Path.addressIndex = pbTx.path().address_index();

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionFIL(contextID,
                                           bip32Path,
                                           pbTx.nonce(),
                                           pbTx.gas_limit(),
                                           (JUB_CHAR_PTR) pbTx.gas_fee_cap_in_atto().c_str(),
                                           (JUB_CHAR_PTR) pbTx.gas_premium_in_atto().c_str(),
                                           (JUB_CHAR_PTR) pbTx.to().c_str(),
                                           (JUB_CHAR_PTR) pbTx.value_in_atto().c_str(),
                                           (JUB_CHAR_PTR) pbTx.input().c_str(),
                                           &raw);

        return buildPbRvString("JUB_SignTransactionFIL", env, rv, raw);
    }
    return buildPbRvString("JUB_SignTransactionFIL", env, JUBR_ARGUMENTS_BAD, "");
}


JNINativeMethod filNativeMethods[] = {
        {
                "nativeFILCreateContext",
                "([BI)[B",
                (void *) native_CreateContextFIL
        },
        {
                "nativeFILGetMainHDNode",
                "(I[B)[B",
                (void *) native_GetMainHDNodeFIL
        },
        {
                "nativeFILGetHDNode",
                "(I[B[B)[B",
                (void *) native_GetHDNodeFIL
        },
        {
                "nativeFILGetAddress",
                "(I[BZ)[B",
                (void *) native_GetAddressFIL
        },
//        {
//                "nativeFILSetAddress",
//                "(I[B)[B",
//                (void *) native_SetMyAddressFIL
//        },
        {
                "nativeFILSignTransaction",
                "(I[B)[B",
                (void *) native_SignTransactionFIL
        },
};


#define FIL_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getFilClass(JNIEnv *env) {
    return env->FindClass(FIL_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getFilNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(filNativeMethods) / sizeof(filNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(filNativeMethods[i]);
    }
    return methodList;
}