//
// Created by FT on 2018/4/16.
//

#include <jni.h>
#include "utils/logUtils.h"
#include "utils/utils.h"
#include <vector>
#include <json/json.h>
#include "implJni.h"

#include <cpp/Jub_Common.pb.h>
#include <cpp/Jub_Bitcoin.pb.h>
#include <cpp/Jub_Ethereum.pb.h>

#ifdef HC
#include <JUB_SDK_Hcash.h>
#else
#include <JUB_core.h>

#endif


#ifndef HC
// 标准钱包的 class path
#define NATIVE_API_CLASS "com/jubiter/jubnative/NativeApi"
// jn 接口返回的结果
#define RESULT_CLASS     "com/jubiter/jubnative/utils/JniResult"
#else
// HC Wallet钱包的class path
    //#define NATIVE_API_CLASS "com/legendwd/hyperpay/main/hardwarewallet/jubnative/NativeApi"
#endif


// 保存 JavaVM
JavaVM *g_vm = NULL;

// 是否是多重签名
bool globalMultiSig = false;

//==================helper=========================
jstring buildPbRvString(JNIEnv *env,JUB_RV rv,JUB_CHAR_PTR str){
    JUB::Proto::Common::ResultString resultString;
    resultString.set_rv(rv);
    if(JUBR_OK){resultString.set_res(str);}

    JUB_FreeMemory(str);
    std::string result;
    resultString.SerializeToString(&result);
    return env->NewStringUTF(result.c_str());
}

jstring buildPbRvString(JNIEnv *env,JUB_RV rv,std::string str){
    JUB::Proto::Common::ResultString resultString;
    resultString.set_rv(rv);
    if(JUBR_OK){resultString.set_res(str);}

    std::string result;
    resultString.SerializeToString(&result);
    return env->NewStringUTF(result.c_str());
}


//================================== 软件钱包 ===========================================

JNIEXPORT jstring JNICALL native_GenerateMnemonic(JNIEnv *env, jobject obj, jstring param) {
    char* pParam = const_cast<char *>(env->GetStringUTFChars(param, NULL));
    std::string paramString = pParam;

    JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH strength;
    JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH_Parse(paramString, &strength);

    JUB_MNEMONIC_STRENGTH jubStrength;
    switch (strength) {
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH128:
            jubStrength = JUB_MNEMONIC_STRENGTH::STRENGTH128;
            break;
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH192:
            jubStrength = JUB_MNEMONIC_STRENGTH::STRENGTH192;
            break;
        case JUB::Proto::Common::ENUM_MNEMONIC_STRENGTH::STRENGTH256:
            jubStrength = JUB_MNEMONIC_STRENGTH::STRENGTH256;
            break;
        default:
            jubStrength = JUB_MNEMONIC_STRENGTH::STRENGTH128;
    }

    JUB_CHAR_PTR pMnemonic;
    JUB_RV rv = JUB_GenerateMnemonic(jubStrength, &pMnemonic);
    return buildPbRvString(env,rv,pMnemonic);
}


JNIEXPORT jint JNICALL native_CheckMnemonic(JNIEnv *env, jobject obj, jstring mnemonic) {
    JUB_CHAR_PTR mnemonicPtr = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(mnemonic, NULL));
    JUB_RV rv = JUB_CheckMnemonic(mnemonicPtr);
    if (JUBR_OK != rv) {
        LOG_ERR("JUB_CheckMnemonic rv: %08x", rv);
        return rv;
    }
    return rv;
}


JNIEXPORT jstring JNICALL native_GenerateSeed(JNIEnv *env, jobject obj, jstring mnemonic, jstring passphrase) {
    JUB_CHAR_PTR mnemonicPtr = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(mnemonic, NULL));
    JUB_CHAR_PTR passphraseStr = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(passphrase, NULL));
    JUB_BYTE seed[64] = {0,};
    JUB_RV rv = JUB_GenerateSeed(mnemonicPtr, passphraseStr, seed, nullptr);
    std::string strSeed = CharPtr2HexStr(seed,64);
    return buildPbRvString(env,rv,strSeed);

}

JNIEXPORT jstring JNICALL native_SeedToMasterPrivateKey(JNIEnv *env, jobject obj,jstring seed,jstring curve){
    char* pParam = const_cast<char *>(env->GetStringUTFChars(curve, NULL));
    const char* pSeed = env->GetStringUTFChars(seed, NULL);
    std::string paramString = pParam;

    JUB::Proto::Common::CURVES enum_cure;
    JUB::Proto::Common::CURVES_Parse(paramString, &enum_cure);

    std::vector<unsigned char> vSeed = HexStr2CharPtr(pSeed);

    JUB_CHAR_PTR xprv = nullptr;
    JUB_RV rv = JUB_SeedToMasterPrivateKey(&vSeed[0],vSeed.size(),(JUB_CURVES)enum_cure,&xprv);
    return buildPbRvString(env,rv,xprv);
}


//================================= 蓝牙 ================================================

JNIEXPORT int JNICALL native_initDevice(JNIEnv *env, jobject obj) {
    LOG_ERR(">>> in native_initDevice");

    DEVICE_INIT_PARAM initParam;

    // 初始化参数转换
    jobjectToInitParam(env, g_vm, &initParam);

    JUB_RV rv = JUB_initDevice(initParam);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_initDevice ret: %08x", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_startScan(JNIEnv *env, jobject obj, jobject scanCallback) {
    jobject objParam = env->NewGlobalRef(scanCallback);
    setScanCallbackObj(g_vm, objParam);

    JUB_RV rv = JUB_enumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_enumDevices rv: %08x", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_stopScan(JNIEnv *env, jobject obj) {
    JUB_RV rv = JUB_stopEnumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_stopEnumDevices rv: %08x", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_connectDevice(JNIEnv *env, jobject obj, jstring address, jint devType,
                                            jlongArray handle, jint timeout, jobject disCallback) {
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) (env->GetStringUTFChars(address, NULL));
    JUB_UINT16 *pHandle = reinterpret_cast<JUB_UINT16 *>(env->GetLongArrayElements(handle, NULL));

    jobject objParam = env->NewGlobalRef(disCallback);
    setDiscCallbackObj(g_vm, objParam);

    JUB_RV rv = JUB_connectDevice(pAddress, devType, pHandle, timeout);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_stopEnumDevices rv: %08x", rv);
        env->ReleaseLongArrayElements(handle, reinterpret_cast<jlong *>(pHandle), 0);
        return rv;
    }
    env->ReleaseLongArrayElements(handle, reinterpret_cast<jlong *>(pHandle), 0);
    return rv;
}


JNIEXPORT jint JNICALL native_cancelConnect(JNIEnv *env, jobject obj, jstring address) {
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) env->GetStringUTFChars(address, NULL);

    JUB_RV rv = JUB_cancelConnect(pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_cancelConnect rv: %08x", rv);
        env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
        return rv;
    }

    env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
    return rv;
}


JNIEXPORT jint JNICALL native_disconnectDevice(JNIEnv *env, jobject obj, jlong deviceHandle) {
    JUB_RV rv = JUB_disconnectDevice(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_disconnectDevice rv: %08x", rv);
    }
    return rv;
}


JNIEXPORT jint JNICALL native_isConnectDevice(JNIEnv *env, jobject obj, jlong deviceHandle) {
    JUB_RV rv = JUB_isDeviceConnect(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_isDeviceConnect rv: %08x", rv);
    }
    return rv;
}

//================================= 功能 ================================================

JNIEXPORT jint JNICALL native_show(JNIEnv *env, jobject obj, jlong contextID) {
    JUB_RV rv = JUB_ShowVirtualPwd(contextID);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_showVirtualPwd rv: %08x", rv);
    }
    return rv;
}

JNIEXPORT jint JNICALL native_CancelVirtualPwd(JNIEnv *env, jobject obj, jlong contextID) {
    JUB_RV rv = JUB_CancelVirtualPwd(contextID);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_CancelVirtualPwd rv: %08x", rv);
    }
    return rv;
}


JNIEXPORT jint JNICALL native_verifyPIN(JNIEnv *env, jobject obj, jlong contextID, jbyteArray jPin) {
    JUB_CHAR_PTR pPin = (JUB_CHAR_PTR) (env->GetByteArrayElements(jPin, NULL));
    int length = env->GetArrayLength(jPin);

    // java数组没有结束符，jni层需补充
    *(pPin + length) = '\0';

    JUB_ULONG retry;
    JUB_RV ret = JUB_VerifyPIN(contextID, pPin, &retry);
    if (ret != JUBR_OK) {
        LOG_ERR("JUB_VerifyPIN: %08x", ret);
    }
    env->ReleaseByteArrayElements(jPin, (jbyte *) pPin, JNI_ABORT);
    return ret;
}

JNIEXPORT jint JNICALL native_GetDeviceInfo(JNIEnv *env, jobject obj,
                                            jobject deviceInfo, jlong deviceHandle) {

#define SET_SN               "setSn"
#define SET_LABEL            "setLabel"
#define SETPIN_RETRY         "setPin_retry"
#define SETPIN_MAX_RETRY     "setPin_max_retry"
#define SETBLE_VERSION       "setBle_version"
#define SET_FIRMWARE_VERSION "setFirmware_version"

    JUB_DEVICE_INFO info;
    JUB_RV rv = JUB_GetDeviceInfo((JUB_UINT16) deviceHandle, &info);
    if (rv == JUBR_OK) {
        jclass clazz = env->GetObjectClass(deviceInfo);
        jmethodID setSN = env->GetMethodID(clazz, SET_SN, "(Ljava/lang/String;)V");
        env->CallVoidMethod(deviceInfo, setSN, env->NewStringUTF(info.sn));

        jmethodID setLabel = env->GetMethodID(clazz, SET_LABEL, "(Ljava/lang/String;)V");
        env->CallVoidMethod(deviceInfo, setLabel, env->NewStringUTF(info.label));

        jmethodID pin_retry = env->GetMethodID(clazz, SETPIN_RETRY, "(I)V");
        env->CallVoidMethod(deviceInfo, pin_retry, info.pinRetry);

        jmethodID pin_max_retry = env->GetMethodID(clazz, SETPIN_MAX_RETRY, "(I)V");
        env->CallVoidMethod(deviceInfo, pin_max_retry, info.pinMaxRetry);

        char tmpVersion[5] = {0,};
        memcpy(tmpVersion, info.bleVersion, 4);
        jmethodID ble_version = env->GetMethodID(clazz, SETBLE_VERSION, "(Ljava/lang/String;)V");
        jstring version = env->NewStringUTF(tmpVersion);
        env->CallVoidMethod(deviceInfo, ble_version, version);

        char tmpFirewareVersion[5] = {0,};
        memcpy(tmpFirewareVersion, info.firmwareVersion, 4);
        jmethodID firmware_version = env->GetMethodID(clazz, SET_FIRMWARE_VERSION, "(Ljava/lang/String;)V");
        jstring firmware_versionStr = env->NewStringUTF(tmpFirewareVersion);
        env->CallVoidMethod(deviceInfo, firmware_version, firmware_versionStr);
    }
    return rv;
};

JNIEXPORT jobject JNICALL native_sendAPDU(JNIEnv *env, jobject obj, jlong deviceID,
                                          jstring jApdu) {
    JUB_CHAR_PTR pApdu = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jApdu, NULL));
    JUB_CHAR_PTR response = nullptr;
    JUB_RV ret = JUB_SendOneApdu(deviceID, pApdu, &response);
    if (ret == JUBR_OK) {
        jstring result = env->NewStringUTF(response);
        JUB_FreeMemory(response);
        return newJavaObject(env, RESULT_CLASS, ret, result);
    } else {
        return newJavaObject(env, RESULT_CLASS, ret, nullptr);;
    }
}

JNIEXPORT jobject JNICALL native_GetDeviceCert(JNIEnv *env, jobject obj, jlong deviceHandle) {
    JUB_CHAR_PTR cert = NULL;
    JUB_RV rv = JUB_GetDeviceCert((JUB_UINT16) deviceHandle, &cert);
    if (rv == JUBR_OK) {
        jstring result = env->NewStringUTF(cert);
        JUB_FreeMemory(cert);
        return newJavaObject(env, RESULT_CLASS, rv, result);
    } else {
        LOG_ERR("JUB_GetDeviceCert error");
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
}

JNIEXPORT jobject JNICALL native_GetAppletVersion(JNIEnv *env, jobject obj, jlong deviceHandle, jstring appID) {
    JUB_CHAR_PTR pAppID = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(appID, NULL));
    JUB_CHAR_PTR appVersion = NULL;
    JUB_RV rv = JUB_GetAppletVersion((JUB_UINT16) deviceHandle, pAppID, &appVersion);
    if (rv == JUBR_OK) {
        jstring result = env->NewStringUTF(appVersion);
        JUB_FreeMemory(appVersion);
        return newJavaObject(env, RESULT_CLASS, rv, result);
    } else {
        LOG_ERR("JUB_GetAppletVersion error");
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
}

JNIEXPORT jobject JNICALL native_EnumApplets(JNIEnv *env, jobject obj, jlong deviceHandle) {

    JUB_CHAR_PTR list = NULL;
    JUB_RV rv = JUB_EnumApplets((JUB_UINT16) deviceHandle, &list);
    if (rv == JUBR_OK) {
        jstring result = env->NewStringUTF(list);
        JUB_FreeMemory(list);
        return newJavaObject(env, RESULT_CLASS, rv, result);
    } else {
        LOG_ERR("JUB_EnumApplets error");
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
}

JNIEXPORT jint JNICALL native_SetTimeOut(JNIEnv *env, jobject obj, jlong contextID, jint jTimeOut) {
    return static_cast<jint>(JUB_SetTimeOut(static_cast<JUB_UINT16>(contextID), static_cast<JUB_UINT16>(jTimeOut)));
}

//===================================== BTC ============================================

JNIEXPORT jint JNICALL
native_BTCCreateContext(JNIEnv *env, jobject obj, jintArray jContextId, jboolean isMultiSig, jstring jJSON,
                        jlong deviceInfo) {

#define MAIN_PATH      "main_path"
#define P2SH_SEGWIT    "p2sh_segwit"
#define M              "m"
#define N              "n"
#define MASTER_KEY     "cosigner"

    if (NULL == jJSON) {
        return JUBR_ARGUMENTS_BAD;
    }

    int length = env->GetStringLength(jJSON);
    if (0 == length) {
//        errorCode = JUBR_ARGUMENTS_BAD;
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_UINT16 *pContextID = (JUB_UINT16 *) env->GetIntArrayElements(jContextId, NULL);
    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    CONTEXT_CONFIG_BTC cfg;
//    CONTEXT_CONFIG_MULTISIG_BTC multiCfg;
    cfg.mainPath = (char *) root[MAIN_PATH].asCString();
    cfg.coinType = COINBTC;

    // 缓存是否是多重签名标记
    globalMultiSig = isMultiSig;

    JUB_RV rv = JUBR_OK;
    if (isMultiSig) {
//        multiCfg.transType = p2sh_multisig;
//        multiCfg.mainPath = cfg.mainPath;
//        multiCfg.coinType = cfg.coinType;
//        multiCfg.m = root[M].asInt64();
//        multiCfg.n = root[N].asInt64();

        int keySize = root[MASTER_KEY].size();
        std::vector<std::string> masterKey;
        for (int i = 0; i < keySize; ++i) {
            std::string key = root[MASTER_KEY][i].asString();
            masterKey.push_back(key);
        }
//        multiCfg.vCosignerMainXpub = masterKey;
//
//        rv = JUB_CreateContextBTC(&multiCfg, deviceInfo, pContextID);
    } else {
        cfg.transType = p2pkh;
//        rv = JUB_CreateContextBTC(&cfg, deviceInfo, pContextID);
    }

    if (rv != JUBR_OK) {
        LOG_ERR("JUB_CreateContextBTC: %08x", rv);
    } else {
        LOG_INF("contextID: %d", *pContextID);
    }
    env->ReleaseIntArrayElements(jContextId, (jint *) pContextID, 0);
    return rv;
}


JNIEXPORT jint JNICALL native_ClearContext(JNIEnv *env, jobject obj, jint jContextId) {
    JUB_RV rv = JUB_ClearContext(jContextId);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_ClearContext: %08x", rv);
    }
    return rv;
}

// todo
JNIEXPORT jint JNICALL native_BuildUSDTOutputs(JNIEnv *env, jobject obj, jintArray jContextId) {
    JUB_RV rv = 0;

    return rv;
}


JNIEXPORT jobject JNICALL native_BTC_ShowAddress(JNIEnv *env, jobject obj, jlong contextID, jint change, jint index) {

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_GetAddressBTC(static_cast<JUB_UINT16>(contextID), path, BOOL_TRUE, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetAddressBTC: %08x", rv);
//        errorCode = static_cast<int>(rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return newJavaObject(env, RESULT_CLASS, rv, address);
}

JNIEXPORT jobject JNICALL native_BTC_SetMyAddress(JNIEnv *env, jobject obj, jlong contextID, jint change, jint index) {

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_SetMyAddressBTC(static_cast<JUB_UINT16>(contextID), path, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_SetMyAddressBTC: %08x", rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return newJavaObject(env, RESULT_CLASS, rv, address);
}

JNIEXPORT jobjectArray JNICALL native_BTCGetAddress(JNIEnv *env, jobject obj, jlong contextID, jstring jJSON) {

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

        BIP32_Path path;
        path.change = (JUB_ENUM_BOOL) root[BIP32_PATH][i][CHANGE].asBool();
        path.addressIndex = static_cast<JUB_UINT64>(root[BIP32_PATH][i][INDEX].asInt());

        JUB_RV rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
        if (rv != JUBR_OK) {
            LOG_ERR("JUB_GetHDNodeBTC: %08x", rv);
            env->SetObjectArrayElement(array, 2 * i, NULL);
            env->SetObjectArrayElement(array, 2 * i + 1, NULL);
        } else {
            jstring jsXpub = env->NewStringUTF(xpub);
            JUB_CHAR_PTR pAddress = NULL;
            rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &pAddress);
            if (rv != JUBR_OK) {
                LOG_ERR("JUB_GetAddressBTC: %08x", rv);
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

JNIEXPORT jobject JNICALL native_BTCGetMainHDNode(JNIEnv *env, jobject obj, jlong contextID) {

    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &xpub);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetHDNodeBTC: %08x", rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring mainPub = env->NewStringUTF(xpub);
    JUB_FreeMemory(xpub);
    return newJavaObject(env, RESULT_CLASS, rv, mainPub);
}


//jstring native_ParseTransactionRaw(JNIEnv *env, jobject obj, jlong contextID, jstring jRaw) {
//
//    JUB_CHAR_PTR pRaw = (JUB_CHAR_PTR) env->GetStringUTFChars(jRaw, NULL);
//
//    JUB_UINT32 lockTime;
//    std::vector<INPUT_BTC> btcInputVector;
//    std::vector<OUTPUT_BTC> btcOutputVector;
//    JUB_RV rv = JUB_ParseTransactionBTC(contextID, pRaw, btcInputVector, btcOutputVector, &lockTime);
//    if (rv != JUBR_OK) {
//        LOG_ERR("JUB_ParseTransactionBTC: %08x", rv);
//        errorCode = rv;
//        return NULL;
//    }
//
//    Json::Value root;
//    Json::Value inputArray;
//    for (int i = 0; i < btcInputVector.size(); ++i) {
//        INPUT_BTC inputBtc = btcInputVector[i];
//        Json::Value input;
//        input["multisig"] = inputBtc.type;
//        input["preHash"] = inputBtc.preHash;
//        input["preIndex"] = inputBtc.preIndex;
//        input["amount"] = inputBtc.amount;
//        input["nSequence"] = inputBtc.nSequence;
//        inputArray.append(input);
//    }
//    root["inputs"] = inputArray;
//
//    Json::Value outputArray;
//    for (int i = 0; i < btcOutputVector.size(); ++i) {
//        OUTPUT_BTC outputBtc = btcOutputVector[i];
//        Json::Value output;
//        output["multisig"] = outputBtc.type;
//        output["amount"] = outputBtc.stdOutput.amount;
//        output["address"] = outputBtc.stdOutput.address == nullptr ? "" : outputBtc.stdOutput.address;
//        outputArray.append(output);
//    }
//    root["output"] = outputArray;
//    root["lock_time"] = lockTime;
//
//    std::string rawString = root.toStyledString();
//    return env->NewStringUTF(rawString.c_str());
//}


JNIEXPORT jobject JNICALL native_BTCTransaction(JNIEnv *env, jobject obj, jlong contextID, jstring jJSON) {
#define INPUTS       "inputs"
#define PREHASH      "preHash"
#define PREINDEX     "preIndex"
#define BIP32_PATH   "bip32_path"
#define CHANGE       "change"
#define INDEX        "addressIndex"
#define AMOUNT       "amount"
#define MULTISIG     "multisig"

#define OUTPUTS      "outputs"
#define ADDRESS      "address"
#define CHANGE_ADDRESS "change_address"

//    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));
//    Json::Reader reader;
//    Json::Value root;
//    reader.parse(pJSON, root);
//
//    std::vector<INPUT_BTC> inputs;
//    std::vector<OUTPUT_BTC> outputs;
//
//    int input_number = root[INPUTS].size();
//    for (int i = 0; i < input_number; i++) {
//        INPUT_BTC input;
//        // 根据全局变量赋值
//        input.type = globalMultiSig ? P2SH_MULTISIG : P2PKH;
////        input.type = SCRIPT_BTC_TYPE(root[INPUTS][i][MULTISIG].asInt());
//        input.preHash = (char *) root[INPUTS][i][PREHASH].asCString();
//        input.preIndex = static_cast<JUB_UINT16>(root[INPUTS][i][PREINDEX].asInt());
//        input.path.change = (JUB_ENUM_BOOL) root[INPUTS][i][BIP32_PATH][CHANGE].asBool();
//        input.path.addressIndex = static_cast<JUB_UINT64>(root[INPUTS][i][BIP32_PATH][INDEX].asInt());
//        input.amount = static_cast<JUB_UINT64>(root[INPUTS][i][AMOUNT].asInt());
//        inputs.push_back(input);
//    }
//
//    int output_number = root[OUTPUTS].size();
//    for (int i = 0; i < output_number; i++) {
//        OUTPUT_BTC output;
//        // 根据全局变量赋值
//        output.type = globalMultiSig ? P2SH_MULTISIG : P2PKH;
////        output.type = SCRIPT_BTC_TYPE(root[OUTPUTS][i][MULTISIG].asInt());
//        output.stdOutput.address = (char *) root[OUTPUTS][i][ADDRESS].asCString();
//        output.stdOutput.amount = static_cast<JUB_UINT64>(root[OUTPUTS][i][AMOUNT].asInt());
//        output.stdOutput.changeAddress = (JUB_ENUM_BOOL) root[OUTPUTS][i][CHANGE_ADDRESS].asBool();
//        if (output.stdOutput.changeAddress) {
//            output.stdOutput.path.change = (JUB_ENUM_BOOL) root[OUTPUTS][i][BIP32_PATH][CHANGE].asBool();
//            output.stdOutput.path.addressIndex = static_cast<JUB_UINT64>(root[OUTPUTS][i][BIP32_PATH][INDEX].asInt());
//        }
//        outputs.push_back(output);
//    }
//
//    char *raw = NULL;
//    JUB_SetUnitBTC(static_cast<JUB_UINT16>(contextID), BTC);
//    JUB_RV rv = JUB_SignTransactionBTC(static_cast<JUB_UINT16>(contextID), &inputs[0],
//                                       (JUB_UINT16) inputs.size(),
//                                       &outputs[0], (JUB_UINT16) outputs.size(), 0, &raw);
//
//    // JUBR_MULTISIG_OK 表示多重签名结果不完整，需要后续再次签名
//    if (rv != JUBR_OK && rv != JUBR_MULTISIG_OK) {
//        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
//    }
//    // 用于判断是否多签完成
//    errorCode = rv;
//    jstring rawString = env->NewStringUTF(raw);
//    JUB_FreeMemory(raw);
//    return newJavaObject(env, RESULT_CLASS, rv, rawString);

    return nullptr;
}


//==================================== ETH ==========================================

JNIEXPORT jint JNICALL native_ETHCreateContext(JNIEnv *env, jobject obj, jintArray jContextId, jstring jJSON,
                                               jlong deviceInfo) {

#define MAIN_PATH  "main_path"
#define CHAIN_ID   "chainID"

    JUB_UINT16 *pContextID = (JUB_UINT16 *) env->GetIntArrayElements(jContextId, NULL);

    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    CONTEXT_CONFIG_ETH cfg;
    cfg.mainPath = (char *) root[MAIN_PATH].asCString();
    cfg.chainID = root[CHAIN_ID].asInt();
    int rv = JUB_CreateContextETH(cfg, static_cast<JUB_UINT16>(deviceInfo), pContextID);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetAddressETH: %08x", rv);
    } else {
        LOG_INF("JUB_GetAddressETH: %d", *pContextID);
    }
    env->ReleaseIntArrayElements(jContextId, (jint *) pContextID, FALSE);
    return rv;
}


JNIEXPORT jobject JNICALL native_ETH_Transaction(JNIEnv *env, jobject obj, jlong contextID,
                                                 jstring jJSON) {

    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL) root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce = static_cast<uint32_t>(root["ETH"]["nonce"].asDouble());
    uint32_t gasLimit = static_cast<uint32_t>(root["ETH"]["gasLimit"].asDouble());
    char *gasPriceInWei = (char *) root["ETH"]["gasPriceInWei"].asCString();
    char *valueInWei = (char *) root["ETH"]["valueInWei"].asCString();
    char *to = (char *) root["ETH"]["to"].asCString();
    char *data = (char *) root["ETH"]["data"].asCString();

    char *raw = nullptr;
    JUB_RV rv = JUB_SignTransactionETH(static_cast<JUB_UINT16>(contextID), path,
                                       nonce, gasLimit,
                                       gasPriceInWei, to, valueInWei, data, &raw);
    if (rv != JUBR_OK) {
//        errorCode = static_cast<int>(rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }

    jstring rawString = env->NewStringUTF(raw);
    JUB_FreeMemory(raw);
    return newJavaObject(env, RESULT_CLASS, rv, rawString);
}


JNIEXPORT jobject JNICALL native_ETH_ERC20_Transaction(JNIEnv *env, jobject obj, jlong contextID,
                                                       jstring jJSON) {

    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    uint32_t nonce = root["ERC20"]["nonce"].asDouble();
    uint32_t gasLimit = root["ERC20"]["gasLimit"].asDouble();
    char *gasPriceInWei = (char *) root["ERC20"]["gasPriceInWei"].asCString();
    char *to = (char *) root["ERC20"]["contract_address"].asCString();
    char *token_to = (char *) root["ERC20"]["token_to"].asCString();
    char *token_value = (char *) root["ERC20"]["token_value"].asCString();

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL) root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();

    char *abi = nullptr;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID, token_to, token_value, &abi);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_BuildERC20AbiETH: %08x", rv);
        env->ReleaseStringUTFChars(jJSON, pJSON);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }

    char *raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, 0, abi, &raw);

    JUB_FreeMemory(abi);
    if (rv != JUBR_OK) {
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }

    jstring rawString = env->NewStringUTF(raw);
    JUB_FreeMemory(raw);
    return newJavaObject(env, RESULT_CLASS, rv, rawString);
}

JNIEXPORT jint JNICALL native_ETH_Set_ERC20_Token(JNIEnv *env, jobject obj, jlong contextID,
                                                       jstring jJSON) {

    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    char *tokenName = (char *) root["ERC20Token"]["tokenName"].asCString();
    uint16_t unitDP = root["ERC20Token"]["dp"].asDouble();
    char *contractAddress = (char *) root["ERC20Token"]["contract_address"].asCString();

    JUB_RV rv = 0; // JUB_SetERC20ETHToken(contextID, tokenName, unitDP, contractAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_SetERC20ETHToken: %08x", rv);
        env->ReleaseStringUTFChars(jJSON, pJSON);
    }
    return rv;
}

JNIEXPORT jint JNICALL native_QueryBattery(JNIEnv *env, jobject obj, jlong deviceID, jintArray batteryArray) {

    jint *pBattery = env->GetIntArrayElements(batteryArray, NULL);
    JUB_BYTE battery = 0;
    JUB_RV rv = JUB_QueryBattery(deviceID, &battery);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_QueryBattery rv: %08lx", rv);
        env->ReleaseIntArrayElements(batteryArray, pBattery, 0);
        return rv;
    }
    *pBattery = battery & 0x0FF;
    env->ReleaseIntArrayElements(batteryArray, pBattery, 0);
    return rv;
}

JNIEXPORT jint JNICALL native_IsInitialize(JNIEnv *env, jobject obj, jlong deviceID) {
    JUB_UINT16 deviceId = deviceID;
    JUB_RV rv = JUB_IsInitialize(deviceId);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_IsInitialize rv: %08x", rv);
    }
    return rv;
}

JNIEXPORT jint JNICALL native_IsBootLoader(JNIEnv *env, jobject obj, jlong deviceID) {
    JUB_UINT16 deviceId = deviceID;
    JUB_RV rv = JUB_IsBootLoader(deviceId);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_IsBootLoader rv: %08x", rv);
    }
    return rv;
}

JNIEXPORT jstring JNICALL native_EnumSupportCoins(JNIEnv *env, jobject obj, jlong deviceID) {
    JUB_CHAR_PTR pCoinList = NULL;
    JUB_RV rv = Jub_EnumSupportCoins(deviceID, &pCoinList);
    if (rv != JUBR_OK) {
        LOG_ERR("Jub_EnumSupportCoins rv: %08x", rv);
    }
    jstring coinList = env->NewStringUTF(pCoinList);
    JUB_FreeMemory(pCoinList);
    return coinList;
}

JNIEXPORT jobjectArray JNICALL native_ETHGetAddress(JNIEnv *env, jobject obj, jlong contextID, jstring jJSON) {

#define BIP32_PATH   "bip32_path"
#define CHANGE       "change"
#define INDEX        "addressIndex"

    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }
    JUB_CHAR_PTR pJSON = const_cast<JUB_CHAR_PTR>(env->GetStringUTFChars(jJSON, NULL));

    Json::Reader reader;
    Json::Value root;
    reader.parse(pJSON, root);

    int input_number = root[BIP32_PATH].size();
    jobjectArray array = env->NewObjectArray(2 * input_number, clazz, 0);
    for (int i = 0; i < input_number; i++) {
        JUB_CHAR_PTR xpub;

        BIP32_Path path;
        path.change = (JUB_ENUM_BOOL) root[BIP32_PATH][i][CHANGE].asBool();
        path.addressIndex = static_cast<JUB_UINT64>(root[BIP32_PATH][i][INDEX].asInt());

        JUB_RV rv = JUB_GetHDNodeETH(contextID, XPUB, path, &xpub);
        if (rv != JUBR_OK) {
            LOG_ERR("JUB_GetHDNodeBTC: %08x", rv);
            env->SetObjectArrayElement(array, 2 * i, NULL);
            env->SetObjectArrayElement(array, 2 * i + 1, NULL);
        } else {
            jstring jsXpub = env->NewStringUTF(xpub);
            JUB_CHAR_PTR pAddress = NULL;
            rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &pAddress);
            if (rv != JUBR_OK) {
                LOG_ERR("JUB_GetAddressBTC: %08x", rv);
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

JNIEXPORT jobject JNICALL native_ETHShowAddress(JNIEnv *env, jobject obj, jlong contextID, jint index) {
    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(BOOL_FALSE);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_GetAddressETH(static_cast<JUB_UINT16>(contextID), path, BOOL_TRUE, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetAddressBTC: %08x", rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return newJavaObject(env, RESULT_CLASS, rv, address);
}

JNIEXPORT jobject JNICALL native_ETHSetAddress(JNIEnv *env, jobject obj, jlong contextID, jint index) {

    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(BOOL_FALSE);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_SetMyAddressETH(static_cast<JUB_UINT16>(contextID), path, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_SetMyAddressETH: %08x", rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return newJavaObject(env, RESULT_CLASS, rv, address);
}

JNIEXPORT jobject JNICALL native_ETHGetMainHDNode(JNIEnv *env, jobject obj, jlong contextID) {

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, XPUB, &xpub);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetMainHDNodeETH: %08x", rv);
        return newJavaObject(env, RESULT_CLASS, rv, nullptr);
    }
    jstring mainPub = env->NewStringUTF(xpub);
    JUB_FreeMemory(xpub);
    return newJavaObject(env, RESULT_CLASS, rv, mainPub);
}

JNIEXPORT jobject JNICALL native_GetVersion(JNIEnv *env, jobject obj) {
    JUB_CHAR_PTR pVersion = JUB_GetVersion();
    if (pVersion == NULL) {
        LOG_ERR("JUB_GetVersion : %s", pVersion);
        return newJavaObject(env, RESULT_CLASS, JUBR_ERROR, nullptr);
    }
    jstring version = env->NewStringUTF(pVersion);
    return newJavaObject(env, RESULT_CLASS, JUBR_OK, version);
}


//=================================== HC Wallet =========================================

#ifdef HC

JNIEXPORT jint JNICALL native_HCCreateContext(JNIEnv *env, jobject obj, jintArray jContextId, jboolean isMultiSig,
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
        LOG_ERR("JUB_CreateContextBTC: %08x", rv);
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
//        LOG_ERR("JUB_CreateContextHC: %08x", rv);
//    } else {
//        LOG_INF("contextID: %d", *pContextID);
//    }
//    env->ReleaseIntArrayElements(jContextId, (jint *) pContextID, 0);
//    return rv;
}

JNIEXPORT jobjectArray native_HCGetAddress(JNIEnv *env, jobject obj, jlong contextID, jstring jJSON) {

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

        BIP32_Path path;
        path.change = (JUB_ENUM_BOOL) root[BIP32_PATH][i][CHANGE].asBool();
        path.addressIndex = static_cast<JUB_UINT64>(root[BIP32_PATH][i][INDEX].asInt());

        JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
        if (rv != JUBR_OK) {
            LOG_ERR("JUB_GetHDNodeHC: %08x", rv);
            env->SetObjectArrayElement(array, 2 * i, NULL);
            env->SetObjectArrayElement(array, 2 * i + 1, NULL);
        } else {
            jstring jsXpub = env->NewStringUTF(xpub);
            JUB_CHAR_PTR pAddress = NULL;
            rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &pAddress);
            if (rv != JUBR_OK) {
                LOG_ERR("JUB_GetAddressHC: %08x", rv);
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

JNIEXPORT jstring JNICALL native_HCShowAddress(JNIEnv *env, jobject obj, jlong contextID, jint change, jint index) {
    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = static_cast<JUB_UINT64>(index);

    JUB_CHAR_PTR pAddress = NULL;
    JUB_RV rv = JUB_GetAddressHC(static_cast<JUB_UINT16>(contextID), path, BOOL_TRUE, &pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetAddressHC: %08x", rv);
        errorCode = static_cast<int>(rv);
        return NULL;
    }
    jstring address = env->NewStringUTF(pAddress);
    JUB_FreeMemory(pAddress);
    return address;
}

JNIEXPORT jstring native_HCGetMainHDNode(JNIEnv *env, jobject obj, jlong contextID) {
    jclass clazz = env->FindClass("java/lang/String");
    if (clazz == NULL) {
        LOG_ERR("clazz == NULL");
        return NULL;
    }

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeHC(contextID, &xpub);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_GetMainHDNodeHC: %08x", rv);
        errorCode = rv;
        return NULL;
    }
    jstring mainPub = env->NewStringUTF(xpub);
    JUB_FreeMemory(xpub);
    return mainPub;
}

JNIEXPORT jstring JNICALL native_HCTransaction(JNIEnv *env, jobject obj, jlong contextID, jstring jJSON) {
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

// Lcom/jubiter/jubnative/utils/JniResult;

/**
 * JNINativeMethod由三部分组成:
 * (1)Java中的函数名;
 * (2)函数签名,格式为(输入参数类型)返回值类型;
 * (3)native函数名
 */
static JNINativeMethod gMethods[] = {
//    {
//        "nativeInitDevice",
//        "()I",
//        (void *) native_initDevice
//    },
//    {
//        "nativeStartScan",
////        "(Lcom/legendwd/hyperpay/main/hardwarewallet/jubnative/InnerScanCallback;)I",
//        "(Lcom/jubiter/jubnative/InnerScanCallback;)I",
//        (void *) native_startScan
//    },
//    {
//        "nativeStopScan",
//        "()I",
//        (void *) native_stopScan
//    },
//    {
//        "nativeConnectDevice",
////        "(Ljava/lang/String;I[JILcom/legendwd/hyperpay/main/hardwarewallet/jubnative/InnerDiscCallback;)I",
//        "(Ljava/lang/String;I[JILcom/jubiter/jubnative/InnerDiscCallback;)I",
//        (void *) native_connectDevice
//    },
//    {
//        "nativeDisconnect",
//        "(J)I",
//        (void *) native_disconnectDevice
//    },
//    {
//        "nativeIsConnected",
//        "(J)I",
//        (void *) native_isConnectDevice
//    },
//    {
//        "nativeShowVirtualPwd",
//        "(J)I",
//        (void *) native_show
//    },
//    {
//        "nativeVerifyPIN",
//        "(J[B)I",
//        (void *) native_verifyPIN
//    },
//    {
//        "nativeGetDeviceInfo",
////        "(Lcom/legendwd/hyperpay/main/hardwarewallet/jubnative/utils/JUB_DEVICE_INFO;J)I",
//        "(Lcom/jubiter/jubnative/utils/JUB_DEVICE_INFO;J)I",
//        (void *) native_GetDeviceInfo
//    },
//    {
//        "nativeSendApdu",
//        "(JLjava/lang/String;)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_sendAPDU
//    },
//    {
//        "nativeGetDeviceCert",
//        "(J)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_GetDeviceCert
//    },
//    {
//        "nativeEnumApplets",
//        "(J)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_EnumApplets
//    },
//    {
//        "nativeGetAppletVersion",
//        "(JLjava/lang/String;)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_GetAppletVersion
//    },
//    {
//        "nativeBTCCreateContext",
//        "([IZLjava/lang/String;J)I",
//        (void *) native_BTCCreateContext
//    },
//    {
//        "nativeBTCGetAddress",
//        "(JLjava/lang/String;)[Ljava/lang/String;",
//        (void *) native_BTCGetAddress
//    },
//    {
//        "nativeBTCGetMainHDNode",
//        "(J)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_BTCGetMainHDNode
//    },
//    {
//        "nativeBTCShowAddress",
//        "(JII)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_BTC_ShowAddress
//    },
//    {
//        "nativeBTCSetMyAddress",
//        "(JII)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_BTC_SetMyAddress
//    },
//    {
//        "nativeSetTimeOut",
//        "(JI)I",
//        (void *) native_SetTimeOut
//    },
//    {
//        "nativeBTCTransaction",
//        "(JLjava/lang/String;)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_BTCTransaction
//    },
////    {
////        "nativeParseTransaction",
////        "(JLjava/lang/String;)Ljava/lang/String;",
////        (void *) native_ParseTransactionRaw
////    },
//    {
//        "nativeETHCreateContext",
//        "([ILjava/lang/String;J)I",
//        (void *) native_ETHCreateContext
//    },
//    {
//        "nativeETHTransaction",
//        "(JLjava/lang/String;)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_ETH_Transaction
//    },
//    {
//        "nativeETHERC20Transaction",
//        "(JLjava/lang/String;)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_ETH_ERC20_Transaction
//    },
//    {
//        "nativeETHSetERC20ETHToken",
//        "(JLjava/lang/String;)I",
//        (void *) native_ETH_Set_ERC20_Token
//    },
//    {
//        "nativeETHShowAddress",
//        "(JI)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_ETHShowAddress
//    },
//    {
//        "nativeETHGetAddress",
//        "(JLjava/lang/String;)[Ljava/lang/String;",
//        (void *) native_ETHGetAddress
//    },
//    {
//        "nativeETHGetMainHDNode",
//        "(J)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_ETHGetMainHDNode
//    },
//    {
//        "nativeEnumSupportCoins",
//        "(J)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_EnumSupportCoins
//    },
//    {
//        "nativeCancelVirtualPwd",
//        "(J)I",
//        (void *) native_CancelVirtualPwd
//    },
//    {
//        "nativeClearContext",
//        "(J)I",
//        (void *) native_ClearContext
//    },
//    {
//        "nativeQueryBattery",
//        "(J[I)I",
//        (void *) native_QueryBattery
//    },
//    {
//        "nativeIsInitialize",
//        "(J)I",
//        (void *) native_IsInitialize
//    },
//    {
//        "nativeIsBootLoader",
//        "(J)I",
//        (void *) native_IsBootLoader
//    },
//    {
//        "nativeGetVersion",
//        "()Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_GetVersion
//    },
//    {
//        "nativeETHSetAddress",
//        "(JII)Lcom/jubiter/jubnative/utils/JniResult;",
//        (void *) native_ETHSetAddress
//    },
    {
        "nativeGenerateMnemonic",
        "(Ljava/lang/String;)Ljava/lang/String;",
        (void *) native_GenerateMnemonic
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
    jint ret = JNI_FALSE;

    // 获取 env 指针
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOG_ERR(">>> get env fail");
        return ret;
    }

    jint version = env->GetVersion();
    LOG_ERR(">>> jni verion: %d", version);

    // 保存全局 JVM 以便在动态注册的皆空中使用 env 环境
    ret = env->GetJavaVM(&g_vm);
    if (ret != JNI_OK) {
        LOG_ERR(">>> GetJavaVM fail");
        return ret;
    }
    LOG_ERR(">>> GetJavaVM success");

    // 获取类引用
    jclass clazz = env->FindClass(NATIVE_API_CLASS);
    if (clazz == NULL) {
        LOG_ERR(">>> clazz == NULL");
        return ret;
    }
    LOG_ERR(">>> FindClass success");

    // 注册 JNI 方法
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives fail");
        return ret;
    }
    LOG_ERR(">>> jni onload success");

    // 成功
    return JNI_VERSION_1_6;
}