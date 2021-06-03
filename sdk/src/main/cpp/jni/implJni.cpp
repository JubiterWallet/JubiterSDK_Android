//
// Created by FT on 2018/4/16.
//

#include "implJni.h"

#ifdef HC
#include <JUB_SDK_Hcash.h>
#else

#endif


#ifndef HC
// 标准钱包的 class path
#define NATIVE_API_CLASS "com/jubiter/sdk/jni/NativeApi"
#else
// HC Wallet钱包的class path
    //#define NATIVE_API_CLASS "com/legendwd/hyperpay/main/hardwarewallet/jubnative/NativeApi"
#endif


//================================== 软件钱包 ===========================================

JNIEXPORT jbyteArray JNICALL native_GenerateMnemonic(JNIEnv *env, jclass clz, jbyteArray param) {
    std::string paramString = jbyteArray2stdString(env, param);

    JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH strength;
    JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH_Parse(paramString, &strength);

    JUB_ENUM_MNEMONIC_STRENGTH jubStrength;
    switch (strength) {
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH128:
            jubStrength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128;
            break;
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH192:
            jubStrength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192;
            break;
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH256:
            jubStrength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256;
            break;
        default:
            jubStrength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128;
    }

    JUB_CHAR_PTR pMnemonic = nullptr;
    JUB_RV rv = JUB_GenerateMnemonic_soft(jubStrength, &pMnemonic);
    return buildPbRvString("JUB_GenerateMnemonic_soft", env, rv, pMnemonic);
}


JNIEXPORT jint JNICALL native_CheckMnemonic(JNIEnv *env, jclass clz, jstring mnemonic) {
    std::string strMnemonic = jstring2stdString(env, mnemonic);
    JUB_RV rv = JUB_CheckMnemonic(strMnemonic.c_str());
    if (JUBR_OK != rv) {
        LOG_ERR("JUB_CheckMnemonic rv: %08lx", rv);
        return rv;
    }
    return rv;
}


JNIEXPORT jbyteArray JNICALL
native_GenerateSeed(JNIEnv *env, jclass clz, jstring mnemonic, jstring passphrase) {
    std::string strMnemonic = jstring2stdString(env, mnemonic);
    std::string strPassphrase = jstring2stdString(env, passphrase);
    JUB_BYTE seed[64] = {0,};
    JUB_RV rv = JUB_GenerateSeed_soft(strMnemonic.c_str(), strPassphrase.c_str(), seed, nullptr);
    std::string strSeed = CharPtr2HexStr(seed, 64);
    return buildPbRvString("JUB_GenerateSeed_soft", env, rv, strSeed);

}

JNIEXPORT jbyteArray JNICALL
native_SeedToMasterPrivateKey(JNIEnv *env, jclass clz, jstring seed, jbyteArray curve) {
    std::string strCurve = jbyteArray2stdString(env, curve);
    std::string strSeed = jstring2stdString(env, seed);

    JUB::Proto::Common::CURVES enum_curve;
    JUB::Proto::Common::CURVES_Parse(strCurve, &enum_curve);

    std::vector<unsigned char> vSeed = HexStr2CharPtr(strSeed);

    JUB_CHAR_PTR xprv = nullptr;
    JUB_RV rv = JUB_SeedToMasterPrivateKey_soft(&vSeed[0], vSeed.size(),
                                                (JUB_ENUM_CURVES) enum_curve,
                                                &xprv);
    return buildPbRvString("JUB_SeedToMasterPrivateKey_soft", env, rv, xprv);
}

//================================= JUB_SDK_DEV_h ================================================

JNIEXPORT jbyteArray JNICALL native_GetDeviceInfo(JNIEnv *env, jclass clz, jint deviceID) {

    JUB_DEVICE_INFO info;
    JUB_RV rv = JUB_GetDeviceInfo((JUB_UINT16) deviceID, &info);

    JUB::Proto::Common::ResultAny resultDeviceInfo;
    resultDeviceInfo.set_state_code(rv);
    if (rv == JUBR_OK) {
        JUB::Proto::Common::DeviceInfo deviceInfo;
        deviceInfo.set_label(info.label);
        deviceInfo.set_sn(info.sn);
        deviceInfo.set_pin_retry(info.pinRetry);
        deviceInfo.set_pin_max_retry(info.pinMaxRetry);
        JUB_CHAR bleVersion[5] = {0};
        JUB_CHAR firmVersion[5] = {0};
        memcpy(bleVersion, info.bleVersion, 4);
        memcpy(firmVersion, info.firmwareVersion, 4);
        deviceInfo.set_ble_version(bleVersion);
        deviceInfo.set_firmware_version(firmVersion);
        resultDeviceInfo.add_value()->PackFrom(deviceInfo);
    }

    std::string result;
    resultDeviceInfo.SerializeToString(&result);
    return stdString2jbyteArray("JUB_GetDeviceInfo", env, result);
};

JNIEXPORT jbyteArray JNICALL native_GetDeviceCert(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_CHAR_PTR cert = nullptr;
    JUB_RV rv = JUB_GetDeviceCert((JUB_UINT16) deviceID, &cert);
    return buildPbRvString("JUB_GetDeviceCert", env, rv, cert);
}

JNIEXPORT jbyteArray JNICALL
native_SendAPDU(JNIEnv *env, jclass clz, jint deviceID, jstring jApdu) {
    auto strAPDU = jstring2stdString(env, jApdu);
    JUB_CHAR_PTR response = nullptr;
    JUB_RV rv = JUB_SendOneApdu((JUB_UINT16) deviceID, (JUB_CHAR_PTR) strAPDU.c_str(), &response);
    return buildPbRvString("JUB_SendOneApdu", env, rv, response);
}

JNIEXPORT jboolean JNICALL native_IsBootLoader(JNIEnv *env, jclass clz, jint deviceID) {
    return (jboolean) JUB_IsBootLoader((JUB_UINT16) deviceID);
}

JNIEXPORT jint JNICALL native_SetTimeOut(JNIEnv *env, jclass clz, jint contextID, jint jTimeOut) {
    return static_cast<jint>(JUB_SetTimeOut(static_cast<JUB_UINT16>(contextID),
                                            static_cast<JUB_UINT16>(jTimeOut)));
}

JNIEXPORT jbyteArray JNICALL native_EnumApplets(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_CHAR_PTR appList = nullptr;
    JUB_RV rv = JUB_EnumApplets((JUB_UINT16) deviceID, &appList);
    return buildPbRvString("JUB_EnumApplets", env, rv, appList);
}

JNIEXPORT jbyteArray JNICALL native_EnumSupportCoins(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_CHAR_PTR appList = nullptr;
    JUB_RV rv = JUB_EnumSupportCoins((JUB_UINT16) deviceID, &appList);
    return buildPbRvString("JUB_EnumSupportCoins", env, rv, appList);
}

JNIEXPORT jbyteArray JNICALL
native_GetAppletVersion(JNIEnv *env, jclass clz, jint deviceID, jstring appID) {
    auto strAppID = jstring2stdString(env, appID);
    JUB_VERSION_PTR appletVersion;
    JUB_RV rv = JUB_GetAppletVersion((JUB_UINT16) deviceID, (JUB_CHAR_PTR) strAppID.c_str(),
                                     appletVersion);

    JUB::Proto::Common::Version pbVersion;
    pbVersion.set_major(appletVersion->major);
    pbVersion.set_minor(appletVersion->minor);
    pbVersion.set_patch(appletVersion->patch);

    std::string result;
    pbVersion.SerializeToString(&result);

    return buildPbRvString("JUB_GetAppletVersion", env, rv, result);
}

JNIEXPORT jbyteArray JNICALL native_QueryBattery(JNIEnv *env, jclass clz, jint deviceID) {
    JUB_BYTE battery = 0;
    JUB_RV rv = JUB_QueryBattery(deviceID, &battery);
    return buildPbRvUInt("JUB_QueryBattery", env, rv, battery & 0xFF);
}

//===================================== JUB_SDK_COMM_H ============================================

JNIEXPORT jint JNICALL native_ClearContext(JNIEnv *env, jclass clz, jint contextID) {
    return (jint) JUB_ClearContext(contextID);
}

JNIEXPORT jint JNICALL native_ShowVirtualPwd(JNIEnv *env, jclass clz, jint contextID) {
    return (jint) JUB_ShowVirtualPwd(contextID);
}

JNIEXPORT jint JNICALL native_CancelVirtualPwd(JNIEnv *env, jclass clz, jint contextID) {
    return (jint) JUB_CancelVirtualPwd(contextID);
}

JNIEXPORT jbyteArray JNICALL
native_VerifyPIN(JNIEnv *env, jclass clz, jint contextID, jstring jPin) {
    auto strPin = jstring2stdString(env, jPin);
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_VerifyPIN(contextID, (JUB_CHAR_PTR) strPin.c_str(), &retry);
    return buildPbRvUInt("JUB_VerifyPIN", env, rv, retry);
}

#ifdef HC

JNIEXPORT jint JNICALL native_HCCreateContext(JNIEnv *env, jclass clz, jintArray jContextId, jboolean isMultiSig,
                                              jstring jJSON, jlong deviceInfo) {

#define MAIN_PATH      "main_path"
#define M              "m"
#define N              "n"
#define MASTER_KEY     "cosigner"

    if (NULL == jJSON) {
        return JUBR_ARGUMENTS_BAD;
    }

    int length = env->GetStringLength(jJSON);
    if (0 == length) {
        errorCode = JUBR_ARGUMENTS_BAD;
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_UINT16 *pContextID = (JUB_UINT16 *) env->GetIntArrayElements(jContextId, NULL);
    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    CONTEXT_CONFIG_HC cfg;
    CONTEXT_CONFIG_MULTISIG_HC multiCfg;
    cfg.mainPath = (char *) root[MAIN_PATH].asCString();

    // 缓存是否是多重签名标记
    globalMultiSig = isMultiSig;

    JUB_RV rv = JUBR_OK;
    if (isMultiSig) {
        multiCfg.transType = p2sh_multisig;
        multiCfg.mainPath = cfg.mainPath;
        multiCfg.m = root[M].asInt64();
        multiCfg.n = root[N].asInt64();

        int keySize = root[MASTER_KEY].size();
        std::vector<std::string> masterKey;
        for (int i = 0; i < keySize; ++i) {
            std::string key = root[MASTER_KEY][i].asString();
            masterKey.push_back(key);
        }
        multiCfg.vCosignerMainXpub = masterKey;

        rv = JUB_CreateMultiSigContextHC(multiCfg, deviceInfo, pContextID);
    } else {
        rv = JUB_CreateContextHC(cfg, deviceInfo, pContextID);
    }

    if (rv != JUBR_OK) {
        LOG_ERR("JUB_CreateContextBTC: %08lx", rv);
    } else {
        LOG_INF("contextID: %d", *pContextID);
    }
    env->ReleaseIntArrayElements(jContextId, (jint *) pContextID, 0);
    return rv;




//    JUB_UINT16 *pContextID = (JUB_UINT16 *) env->GetIntArrayElements(jContextId, NULL);
//    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));
//
//    Json::Reader reader;
//    Json::Value root;
//    reader.parse(pJSON, root);
//
//    CONTEXT_CONFIG_HC cfg;
//    cfg.mainPath = (char *) root[MAIN_PATH].asCString();
//    JUB_RV rv = JUB_CreateContextHC(cfg, deviceInfo, pContextID);
//    if (rv != JUBR_OK) {
//        LOG_ERR("JUB_CreateContextHC: %08lx", rv);
//    } else {
//        LOG_INF("contextID: %d", *pContextID);
//    }
//    env->ReleaseIntArrayElements(jContextId, (jint *) pContextID, 0);
//    return rv;
}

JNIEXPORT jobjectArray native_HCGetAddress(JNIEnv *env, jclass clz, jlong contextID, jstring jJSON) {

#define BIP32_PATH   "bip32_path"
#define CHANGE       "change"
#define INDEX        "addressIndex"

    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }
    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, FALSE));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    int input_number = root[BIP32_PATH].size();
    jobjectArray array = env->NewObjectArray(2 * input_number, clazz, 0);
    for (int i = 0; i < input_number; i++) {
        JUB_CHAR_PTR xpub;

        BIP44_Path path;
        path.change = (JUB_ENUM_BOOL) root[BIP32_PATH][i][CHANGE].asBool();
        path.addressIndex = static_cast<JUB_UINT64>(root[BIP32_PATH][i][INDEX].asInt());

        JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
        if (rv != JUBR_OK) {
            LOG_ERR("JUB_GetHDNodeHC: %08lx", rv);
            env->SetObjectArrayElement(array, 2 * i, NULL);
            env->SetObjectArrayElement(array, 2 * i + 1, NULL);
        } else {
            jstring jsXpub = env->NewStringUTF(xpub);
            JUB_CHAR_PTR pAddress = NULL;
            rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &pAddress);
            if (rv != JUBR_OK) {
                LOG_ERR("JUB_GetAddressHC: %08lx", rv);
                env->SetObjectArrayElement(array, 2 * i, jsXpub);
                env->SetObjectArrayElement(array, 2 * i + 1, NULL);
            } else {
                jstring address = env->NewStringUTF(pAddress);
                env->SetObjectArrayElement(array, 2 * i, jsXpub);
                env->SetObjectArrayElement(array, 2 * i + 1, address);
            }
        }
    }
    return array;
}

JNIEXPORT jstring JNICALL native_HCShowAddress(JNIEnv *env, jclass clz, jlong contextID, jint change, jint index) {
    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_GetAddressHC(static_cast<JUB_UINT16>(contextID), path, BOOL_TRUE, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetAddressHC: %08lx", rv);
        errorCode = static_cast<int>(rv);
        return NULL;
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return address;
}

JNIEXPORT jstring native_HCGetMainHDNode(JNIEnv *env, jclass clz, jlong contextID) {
    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeHC(contextID, &xpub);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetMainHDNodeHC: %08lx", rv);
        errorCode = rv;
        return NULL;
    }
    jstring mainPub = env->NewStringUTF(xpub);
    JUB_FreeMemory(xpub);
    return mainPub;
}

JNIEXPORT jstring JNICALL native_HCTransaction(JNIEnv *env, jclass clz, jlong contextID, jstring jJSON) {
#define INPUTS         "inputs"
#define BIP32_PATH     "bip32_path"
#define CHANGE         "change"
#define INDEX          "addressIndex"
#define AMOUNT         "amount"
#define UNSIGNED_TRANS "unsigned_trans"
#define CHANGE_ADDRESS "change_address"

    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));
    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    std::vector<INPUT_HC> inputs;
    std::vector<OUTPUT_HC> outputs;

    int input_number = root[INPUTS].size();
    for (int i = 0; i < input_number; i++) {
        INPUT_HC input;
        input.path.change = (JUB_ENUM_BOOL) root[INPUTS][i][BIP32_PATH][CHANGE].asBool();
        input.path.addressIndex = static_cast<JUB_UINT64>(root[INPUTS][i][BIP32_PATH][INDEX].asInt());
        input.amount = static_cast<JUB_UINT64>(root[INPUTS][i][AMOUNT].asInt());
        inputs.push_back(input);
    }

    int output_number = root[OUTPUTS].size();
    for (int i = 0; i < output_number; i++) {
        OUTPUT_HC output;
        output.changeAddress = (JUB_ENUM_BOOL) root[OUTPUTS][i][CHANGE_ADDRESS].asBool();
        if (output.changeAddress) {
            output.path.change = (JUB_ENUM_BOOL) root[OUTPUTS][i][BIP32_PATH][CHANGE].asBool();
            output.path.addressIndex = static_cast<JUB_UINT64>(root[OUTPUTS][i][BIP32_PATH][INDEX].asInt());
        }
        outputs.push_back(output);
    }

    char *unsigned_trans = const_cast<char *>(root[UNSIGNED_TRANS].asCString());

    char *raw = NULL;
    JUB_SetUnitBTC(static_cast<JUB_UINT16>(contextID), BTC);
    JUB_RV rv = JUB_SignTransactionHC(static_cast<JUB_UINT16>(contextID),
                                      &inputs[0],
                                      (JUB_UINT16) inputs.size(),
                                      &outputs[0],
                                      (JUB_UINT16) outputs.size(),
                                      unsigned_trans,
                                      &raw);

    // JUBR_MULTISIG_OK 表示多重签名结果不完整，需要后续再次签名
    if (rv != JUBR_OK && rv != JUBR_MULTISIG_OK) {
        errorCode = static_cast<int>(rv);
        return NULL;
    }
    // 用于判断多签是否完成
    errorCode  = rv;
    jstring rawString = env->NewStringUTF(raw);
    JUB_FreeMemory(raw);
    return rawString;
}

#endif

//=======================================================================================


/**
 * JNINativeMethod由三部分组成:
 * (1)Java中的函数名;
 * (2)函数签名,格式为(输入参数类型)返回值类型;
 * (3)native函数名
 */
JNINativeMethod gMethods[] = {
        {
                "nativeShowVirtualPWD",
                "(I)I",
                (void *) native_ShowVirtualPwd
        },
        {
                "nativeCancelVirtualPWD",
                "(I)I",
                (void *) native_CancelVirtualPwd
        },
        {
                "nativeVerifyPIN",
                "(ILjava/lang/String;)[B",
                (void *) native_VerifyPIN
        },
        {
                "nativeGetDeviceInfo",
                "(I)[B",
                (void *) native_GetDeviceInfo
        },
        {
                "nativeSendAPDU",
                "(ILjava/lang/String;)[B",
                (void *) native_SendAPDU
        },
        {
                "nativeGetDeviceCert",
                "(I)[B",
                (void *) native_GetDeviceCert
        },
        {
                "nativeEnumApplets",
                "(I)[B",
                (void *) native_EnumApplets
        },
        {
                "nativeGetAppletVersion",
                "(ILjava/lang/String;)[B",
                (void *) native_GetAppletVersion
        },
        {
                "nativeQueryBattery",
                "(I)[B",
                (void *) native_QueryBattery
        },
        {
                "nativeEnumSupportCoins",
                "(I)[B",
                (void *) native_EnumSupportCoins
        },
        {
                "nativeClearContext",
                "(I)I",
                (void *) native_ClearContext
        },
        {
                "nativeIsBootLoader",
                "(I)Z",
                (void *) native_IsBootLoader
        },
        {
                "nativeSetTimeout",
                "(II)I",
                (void *) native_SetTimeOut
        },
        {
                "nativeGenerateMnemonic",
                "([B)[B",
                (void *) native_GenerateMnemonic
        },
        {
                "nativeCheckMnemonic",
                "(Ljava/lang/String;)I",
                (void *) native_CheckMnemonic
        },
        {
                "nativeGenerateSeed",
                "(Ljava/lang/String;Ljava/lang/String;)[B",
                (void *) native_GenerateSeed
        },
        {
                "nativeSeedToMasterPrivateKey",
                "(Ljava/lang/String;[B)[B",
                (void *) native_SeedToMasterPrivateKey
        },

#ifdef HC
        {
            "nativeHCCreateContext",
            "([IZLjava/lang/String;J)I",
            (void *) native_HCCreateContext
        },
        {
            "nativeHCGetAddress",
            "(JLjava/lang/String;)[Ljava/lang/String;",
            (void *) native_HCGetAddress
        },
        {
            "nativeHCShowAddress",
            "(JII)Ljava/lang/String;",
            (void *) native_HCShowAddress
        },
        {
            "nativeHCGetMainHDNode",
            "(J)Ljava/lang/String;",
            (void *) native_HCGetMainHDNode
        },
        {
            "nativeHCTransaction",
            "(JLjava/lang/String;)Ljava/lang/String;",
            (void *) native_HCTransaction
        },

#endif
};


/**
 * JNI_OnLoad 默认会在 System.loadLibrary 过程中自动调用到，因而可利用此函数，进行动态注册
 * JNI 版本的返回视对应 JDK 版本而定
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint ret = JNI_ERR;

    // 获取 env 指针
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOG_ERR(">>> get env fail");
        return ret;
    }

    jint version = env->GetVersion();
    LOG_DEBUG(">>> jni version: %x", version);

    // 保存全局 JVM 以便在动态注册的接口中使用 env 环境
    ret = env->GetJavaVM(&g_vm);
    if (ret != JNI_OK) {
        LOG_ERR(">>> GetJavaVM fail");
        return ret;
    }
    LOG_DEBUG(">>> GetJavaVM success");

    // 获取类引用
    jclass clazz = env->FindClass(NATIVE_API_CLASS);
    if (clazz == NULL) {
        LOG_ERR(">>> clazz == NULL");
        return ret;
    }
    LOG_DEBUG(">>> FindClass success");

    std::vector<JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(gMethods) / sizeof(gMethods[0]); i < count; ++i) {
        methodList.push_back(gMethods[i]);
    }

    // 注册函数列表
    std::map<jclass, std::vector<JNINativeMethod>> nativeMethodMap;
    // device
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getBleClass(env), getBleNativeMethods()));
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getNfcClass(env), getNfcNativeMethods()));
    // coin
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getBtcClass(env), getBtcNativeMethods()));
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getEthClass(env), getEthNativeMethods()));
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getEosClass(env), getEosNativeMethods()));
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getXrpClass(env), getXrpNativeMethods()));
    nativeMethodMap.insert(std::pair<jclass, std::vector<JNINativeMethod>> (getTrxClass(env), getTrxNativeMethods()));

    for (auto iterator = nativeMethodMap.begin(); iterator != nativeMethodMap.end(); ++iterator) {
        jclass tmpClazz = iterator->first;
        std::vector<JNINativeMethod> tmpMethodList = iterator->second;

        const char* localName = "";

#ifdef DEBUG
        jclass cls_clazz = env->GetObjectClass(tmpClazz);
        jmethodID methodId = env->GetMethodID(cls_clazz, "getSimpleName", "()Ljava/lang/String;");
        jstring methodName = (jstring) env->CallObjectMethod(tmpClazz, methodId);
        localName = env->GetStringUTFChars(methodName, JNI_FALSE);
#endif

        if (env->RegisterNatives(tmpClazz, tmpMethodList.data(), tmpMethodList.size()) < JNI_OK) {
            LOG_ERR(">>> RegisterNatives %s fail", localName);
            return ret;
        }

        LOG_DEBUG(">>> RegisterNatives %s ok", localName);
    }

    // 注册 JNI 方法
    if (env->RegisterNatives(clazz, methodList.data(), methodList.size()) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives fail");
        return ret;
    }
    LOG_DEBUG(">>> jni onLoad success");

    // 成功
    return JNI_VERSION_1_6;
}