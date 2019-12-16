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

#include <JUB_CORE.h>

#endif


#ifndef HC
// 标准钱包的 class path
#define NATIVE_API_CLASS "com/jubiter/sdk/jni/NativeApi"
#else
// HC Wallet钱包的class path
    //#define NATIVE_API_CLASS "com/legendwd/hyperpay/main/hardwarewallet/jubnative/NativeApi"
#endif


// 保存 JavaVM
JavaVM *g_vm = NULL;

//==================helper=========================

std::string jbyteArray2stdString(JNIEnv *env, jbyteArray jbytes) {
    jbyte *data = (jbyte *) env->GetByteArrayElements(jbytes, 0);
    jsize size = env->GetArrayLength(jbytes);
    return std::string(data, data + size);
}

jbyteArray stdString2jbyteArray(JNIEnv *env, std::string str) {
    jbyteArray jarray = env->NewByteArray(str.size());
    jsize size = str.size();
    env->SetByteArrayRegion(jarray, 0, size, (const jbyte *) &str[0]);
    return jarray;
}


jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, JUB_CHAR_PTR str) {
    JUB::Proto::Common::ResultString resultString;
    resultString.set_statecode(rv);
    if (JUBR_OK == rv) { resultString.set_value(str); }

    std::string result;
    resultString.SerializeToString(&result);
    if (nullptr != str) {
        JUB_FreeMemory(str);
    }
    return stdString2jbyteArray(env, result);
}

jbyteArray buildPbRvString(JNIEnv *env, JUB_RV rv, std::string str) {
    JUB::Proto::Common::ResultString resultString;
    resultString.set_statecode(rv);
    if (JUBR_OK == rv) { resultString.set_value(str); }

    std::string result;
    resultString.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
}

jbyteArray buildPbRvUInt(JNIEnv *env, JUB_RV rv, uint32_t res) {
    JUB::Proto::Common::ResultInt resultInt;
    resultInt.set_statecode(rv);
    if (JUBR_OK == rv) { resultInt.set_value(res); }

    std::string result;
    resultInt.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
}

std::string jstring2stdString(JNIEnv *env, jstring jstr) {
    char *pStr = const_cast<char *>(env->GetStringUTFChars(jstr, NULL));
    std::string stdString = pStr;
    env->ReleaseStringUTFChars(jstr, pStr);
    return stdString;
}


template<typename T>
bool parseFromJbyteArray(JNIEnv *env, jbyteArray jbytes, T *pb) {
    auto pStr = jbyteArray2stdString(env, jbytes);
    return pb->ParseFromString(pStr);
}

bool parseBip44Path(JNIEnv *env, jbyteArray jbytes, BIP44_Path *bip44Path) {
    JUB::Proto::Common::Bip32Path pbBip44Path;
    bool rv = parseFromJbyteArray(env, jbytes, &pbBip44Path);
    if (rv) {
        bip44Path->addressIndex = pbBip44Path.addressindex();
        bip44Path->change = (JUB_ENUM_BOOL) pbBip44Path.change();
    }
    return rv;
}



//================================== 软件钱包 ===========================================

JNIEXPORT jbyteArray JNICALL native_GenerateMnemonic(JNIEnv *env, jobject obj, jbyteArray param) {
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

    JUB_CHAR_PTR pMnemonic;
    JUB_RV rv = JUB_GenerateMnemonic(jubStrength, &pMnemonic);
    return buildPbRvString(env, rv, pMnemonic);
}


JNIEXPORT jint JNICALL native_CheckMnemonic(JNIEnv *env, jobject obj, jstring mnemonic) {
    std::string strMnemonic = jstring2stdString(env, mnemonic);
    JUB_RV rv = JUB_CheckMnemonic(strMnemonic.c_str());
    if (JUBR_OK != rv) {
        LOG_ERR("JUB_CheckMnemonic rv: %08lx", rv);
        return rv;
    }
    return rv;
}


JNIEXPORT jbyteArray JNICALL
native_GenerateSeed(JNIEnv *env, jobject obj, jstring mnemonic, jstring passphrase) {
    std::string strMnemonic = jstring2stdString(env, mnemonic);
    std::string strPassphrase = jstring2stdString(env, passphrase);
    JUB_BYTE seed[64] = {0,};
    JUB_RV rv = JUB_GenerateSeed(strMnemonic.c_str(), strPassphrase.c_str(), seed, nullptr);
    std::string strSeed = CharPtr2HexStr(seed, 64);
    return buildPbRvString(env, rv, strSeed);

}

JNIEXPORT jbyteArray JNICALL
native_SeedToMasterPrivateKey(JNIEnv *env, jobject obj, jstring seed, jbyteArray curve) {
    std::string strCurve = jbyteArray2stdString(env, curve);
    std::string strSeed = jstring2stdString(env, seed);

    JUB::Proto::Common::CURVES enum_curve;
    JUB::Proto::Common::CURVES_Parse(strCurve, &enum_curve);

    std::vector<unsigned char> vSeed = HexStr2CharPtr(strSeed);

    JUB_CHAR_PTR xprv = nullptr;
    JUB_RV rv = JUB_SeedToMasterPrivateKey(&vSeed[0], vSeed.size(), (JUB_ENUM_CURVES) enum_curve, &xprv);
    return buildPbRvString(env, rv, xprv);
}


//================================= 蓝牙 ================================================

JNIEXPORT int JNICALL native_initDevice(JNIEnv *env, jobject obj) {
    LOG_ERR(">>> in native_initDevice");

    DEVICE_INIT_PARAM initParam;

    // 初始化参数转换
    jobjectToInitParam(env, g_vm, &initParam);

    JUB_RV rv = JUB_initDevice(initParam);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_initDevice ret: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_startScan(JNIEnv *env, jobject obj, jobject scanCallback) {
    jobject objParam = env->NewGlobalRef(scanCallback);
    setScanCallbackObj(g_vm, objParam);

    JUB_RV rv = JUB_enumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_enumDevices rv: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_stopScan(JNIEnv *env, jobject obj) {
    JUB_RV rv = JUB_stopEnumDevices();
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_stopEnumDevices rv: %08lx", rv);
        return rv;
    }
    return rv;
}

JNIEXPORT jint JNICALL native_connectDevice(JNIEnv *env, jobject obj, jstring address,
                                            jintArray handle, jint timeout, jobject disCallback) {
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) (env->GetStringUTFChars(address, NULL));
    JUB_UINT16 *pHandle = reinterpret_cast<JUB_UINT16 *>(env->GetIntArrayElements(handle, NULL));

    jobject objParam = env->NewGlobalRef(disCallback);
    setDiscCallbackObj(g_vm, objParam);

    JUB_RV rv = JUB_connectDevice(pAddress, 1, pHandle, timeout);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_stopEnumDevices rv: %08lx", rv);
        env->ReleaseIntArrayElements(handle, reinterpret_cast<jint *>(pHandle), 0);
        return rv;
    }
    env->ReleaseIntArrayElements(handle, reinterpret_cast<jint *>(pHandle), 0);
    return rv;
}


JNIEXPORT jint JNICALL native_cancelConnect(JNIEnv *env, jobject obj, jstring address) {
    JUB_BYTE_PTR pAddress = (JUB_BYTE_PTR) env->GetStringUTFChars(address, NULL);

    JUB_RV rv = JUB_cancelConnect(pAddress);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_cancelConnect rv: %08lx", rv);
        env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
        return rv;
    }

    env->ReleaseStringUTFChars(address, reinterpret_cast<const char *>(pAddress));
    return rv;
}


JNIEXPORT jint JNICALL native_disconnectDevice(JNIEnv *env, jobject obj, jint deviceHandle) {
    JUB_RV rv = JUB_disconnectDevice(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_disconnectDevice rv: %08lx", rv);
    }
    return rv;
}


JNIEXPORT jint JNICALL native_isConnectDevice(JNIEnv *env, jobject obj, jint deviceHandle) {
    JUB_RV rv = JUB_isDeviceConnect(deviceHandle);
    if (rv != JUBR_OK) {
        LOG_ERR("JUB_isDeviceConnect rv: %08lx", rv);
    }
    return rv;
}

//================================= JUB_SDK_DEV_h ================================================


JNIEXPORT jbyteArray JNICALL native_GetDeviceInfo(JNIEnv *env, jobject obj, jint deviceID) {

    JUB_DEVICE_INFO info;
    JUB_RV rv = JUB_GetDeviceInfo((JUB_UINT16) deviceID, &info);

    JUB::Proto::Common::ResultAny resultDeviceInfo;
    resultDeviceInfo.set_statecode(rv);
    if (rv == JUBR_OK) {
        JUB::Proto::Common::DeviceInfo deviceInfo;
        deviceInfo.set_label(info.label);
        deviceInfo.set_sn(info.sn);
        deviceInfo.set_pinretry(info.pinRetry);
        deviceInfo.set_pinmaxretry(info.pinMaxRetry);
        JUB_CHAR bleVersion[5] = {0};
        JUB_CHAR firmVersion[5] = {0};
        memcpy(bleVersion, info.bleVersion, 4);
        memcpy(firmVersion, info.firmwareVersion, 4);
        deviceInfo.set_bleversion(bleVersion);
        deviceInfo.set_firmwareversion(firmVersion);
        resultDeviceInfo.add_value()->PackFrom(deviceInfo);
    }

    std::string result;
    resultDeviceInfo.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
};

JNIEXPORT jbyteArray JNICALL native_GetDeviceCert(JNIEnv *env, jobject obj, jint deviceID) {
    JUB_CHAR_PTR cert = NULL;
    JUB_RV rv = JUB_GetDeviceCert((JUB_UINT16) deviceID, &cert);
    return buildPbRvString(env, rv, cert);
}

JNIEXPORT jbyteArray JNICALL
native_SendAPDU(JNIEnv *env, jobject obj, jint deviceID, jstring jApdu) {
    auto strAPDU = jstring2stdString(env, jApdu);
    JUB_CHAR_PTR response = nullptr;
    JUB_RV rv = JUB_SendOneApdu((JUB_UINT16) deviceID, (JUB_CHAR_PTR) strAPDU.c_str(), &response);
    return buildPbRvString(env, rv, response);
}

JNIEXPORT jboolean JNICALL native_IsInitialize(JNIEnv *env, jobject obj, jint deviceID) {
    return (jboolean) JUB_IsInitialize((JUB_UINT16) deviceID);
}

JNIEXPORT jboolean JNICALL native_IsBootLoader(JNIEnv *env, jobject obj, jint deviceID) {
    return (jboolean) JUB_IsBootLoader((JUB_UINT16) deviceID);
}

JNIEXPORT jint JNICALL native_SetTimeOut(JNIEnv *env, jobject obj, jint contextID, jint jTimeOut) {
    return static_cast<jint>(JUB_SetTimeOut(static_cast<JUB_UINT16>(contextID),
                                            static_cast<JUB_UINT16>(jTimeOut)));
}

JNIEXPORT jbyteArray JNICALL native_EnumApplets(JNIEnv *env, jobject obj, jint deviceID) {

    JUB_CHAR_PTR appList = NULL;
    JUB_RV rv = JUB_EnumApplets((JUB_UINT16) deviceID, &appList);
    return buildPbRvString(env, rv, appList);
}

JNIEXPORT jbyteArray JNICALL native_EnumSupportCoins(JNIEnv *env, jobject obj, jint deviceID) {

    JUB_CHAR_PTR appList = NULL;
    JUB_RV rv = Jub_EnumSupportCoins((JUB_UINT16) deviceID, &appList);
    return buildPbRvString(env, rv, appList);
}

JNIEXPORT jbyteArray JNICALL
native_GetAppletVersion(JNIEnv *env, jobject obj, jint deviceID, jstring appID) {
    auto strAppID = jstring2stdString(env, appID);
    JUB_CHAR_PTR appVersion = NULL;
    JUB_RV rv = JUB_GetAppletVersion((JUB_UINT16) deviceID, (JUB_CHAR_PTR) strAppID.c_str(),
                                     &appVersion);
    return buildPbRvString(env, rv, appVersion);
}

JNIEXPORT jbyteArray JNICALL native_QueryBattery(JNIEnv *env, jobject obj, jint deviceID) {

    JUB_BYTE battery = 0;
    JUB_RV rv = JUB_QueryBattery(deviceID, &battery);
    return buildPbRvUInt(env, rv, battery & 0xFF);
}
//===================================== JUB_SDK_COMM_H ============================================
JNIEXPORT jint JNICALL native_ClearContext(JNIEnv *env, jobject obj, jint contextID) {
    return (jint) JUB_ClearContext(contextID);
}

JNIEXPORT jint JNICALL native_ShowVirtualPwd(JNIEnv *env, jobject obj, jint contextID) {
    return (jint) JUB_ShowVirtualPwd(contextID);
}

JNIEXPORT jint JNICALL native_CancelVirtualPwd(JNIEnv *env, jobject obj, jint contextID) {
    return (jint) JUB_CancelVirtualPwd(contextID);
}

JNIEXPORT jbyteArray JNICALL
native_VerifyPIN(JNIEnv *env, jobject obj, jint contextID, jstring jPin) {
    auto strPin = jstring2stdString(env, jPin);
    JUB_ULONG retry;
    JUB_RV rv = JUB_VerifyPIN(contextID, (JUB_CHAR_PTR) strPin.c_str(), &retry);
    return buildPbRvUInt(env, rv, retry);
}

//===================================== JUB_SDK_BTC ============================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC_soft(JNIEnv *env, jobject obj, jbyteArray jcfg, jstring xprv) {
    JUB::Proto::Bitcoin::ContextCfgBTC pbCfg;
    auto strXprv = jstring2stdString(env, xprv);
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.coinType = (JUB_ENUM_COINTYPE_BTC) pbCfg.coin_type();
        cfg.transType = (JUB_ENUM_BTC_TRANS_TYPE) pbCfg.trans_type();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextBTC_soft(cfg, (JUB_CHAR_PTR) strXprv.c_str(), &contextID);
        return buildPbRvUInt(env, rv, contextID);
    } else {
        return buildPbRvUInt(env, JUBR_ARGUMENTS_BAD, 0);
    };
}


JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC(JNIEnv *env, jobject obj, jbyteArray jcfg, jint deviceID) {
    JUB::Proto::Bitcoin::ContextCfgBTC pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.coinType = (JUB_ENUM_COINTYPE_BTC) pbCfg.coin_type();
        cfg.transType = (JUB_ENUM_BTC_TRANS_TYPE) pbCfg.trans_type();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        return buildPbRvUInt(env, rv, contextID);
    } else {
        return buildPbRvUInt(env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL native_GetMainHDNodeBTC(JNIEnv *env, jobject obj, jint contextID) {
    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &xpub);
    return buildPbRvString(env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeBTC(JNIEnv *env, jobject obj, jint contextID, jbyteArray bip32) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR xpub;
        JUB_RV rv = JUB_GetHDNodeBTC(contextID, bip32Path, &xpub);
        return buildPbRvString(env, rv, xpub);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressBTC(JNIEnv *env, jobject obj, jint contextID, jbyteArray bip32, jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_GetAddressBTC(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString(env, rv, address);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}


JNIEXPORT jbyteArray JNICALL
native_SetMyAddressBTC(JNIEnv *env, jobject obj, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_SetMyAddressBTC(contextID, bip32Path, &address);
        return buildPbRvString(env, rv, address);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionBTC(JNIEnv *env, jobject obj, jint contextID, jbyteArray jTX) {

    JUB::Proto::Bitcoin::TransactionBTC tx;
    if (parseFromJbyteArray(env, jTX, &tx)) {
        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        JUB_UINT32 lockTime = tx.locktime();
        for (auto i = 0; i < tx.inputs_size(); i++) {
            INPUT_BTC input;
            input.amount = tx.inputs(i).amount();
            input.preIndex = tx.inputs(i).preindex();
            input.preHash = (JUB_CHAR_PTR) tx.inputs(i).prehash().c_str();
            input.path.change = (JUB_ENUM_BOOL) tx.inputs(i).path().change();
            input.path.addressIndex = tx.inputs(i).path().addressindex();
            inputs.emplace_back(input);
        }
        for (auto i = 0; i < tx.outputs_size(); i++) {
            OUTPUT_BTC output;
            output.type = (OUTPUT_ENUM_BTC_TYPE) tx.outputs(i).type();
            switch (output.type) {
                case STANDARD:
                    if (!tx.outputs(i).has_standardoputput())
                        return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
                    output.outputStandard.amount = tx.outputs(i).standardoputput().amount();
                    output.outputStandard.address = (JUB_CHAR_PTR) tx.outputs(
                        i).standardoputput().address().c_str();
                    output.outputStandard.changeAddress = (JUB_ENUM_BOOL) tx.outputs(
                        i).standardoputput().changeaddress();
                    output.outputStandard.path.addressIndex = tx.outputs(
                        i).standardoputput().path().addressindex();
                    output.outputStandard.path.change = (JUB_ENUM_BOOL) tx.outputs(
                        i).standardoputput().path().change();
                    break;
                case RETURN0:
                    if (!tx.outputs(i).has_return0output())
                        return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
                    if (tx.outputs(i).return0output().data().size() > 40)
                        return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
                    output.outputReturn0.amount = tx.outputs(i).return0output().amount();
                    {
                        auto byteData = HexStr2CharPtr(tx.outputs(i).return0output().data());
                        output.outputReturn0.dataLen = byteData.size();
                        memcpy(output.outputReturn0.data, &byteData[0], byteData.size());
                    }
                    break;
                default:
                    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
            }
            outputs.emplace_back(output);
        }

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionBTC(contextID, &inputs[0], inputs.size(), &outputs[0],
                                           outputs.size(), lockTime, &raw);
        return buildPbRvString(env, rv, raw);
    }

    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_SetUnitBTC(JNIEnv *env, jobject obj, jint contextID, jbyteArray junit) {

    auto strUnit = jbyteArray2stdString(env, junit);

    JUB::Proto::Bitcoin::BTC_UNIT_TYPE unit;
    JUB::Proto::Bitcoin::BTC_UNIT_TYPE_Parse(strUnit, &unit);
    JUB_RV rv = JUB_SetUnitBTC(contextID, (JUB_ENUM_BTC_UNIT_TYPE) unit);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BuildUSDTOutputs(JNIEnv *env, jobject obj, jint contextID, jstring USDTTO, jlong amount) {


    auto strUSDTTO = jstring2stdString(env, USDTTO);

    JUB::Proto::Common::ResultAny resultOutputs;
    OUTPUT_BTC USDT_outputs[2] = {};
    JUB_RV rv = JUB_BuildUSDTOutputs(contextID, (JUB_CHAR_PTR) strUSDTTO.c_str(), amount,
                                     USDT_outputs);
    resultOutputs.set_statecode(rv);
    if (rv == JUBR_OK) {
        //first output is return0,second is standard
        JUB::Proto::Bitcoin::OutputBTC output1;
        JUB::Proto::Bitcoin::Return0Output *return0 = new JUB::Proto::Bitcoin::Return0Output();
        return0->set_amount(USDT_outputs[0].outputReturn0.amount);
        return0->set_data(CharPtr2HexStr(USDT_outputs[0].outputReturn0.data,
                                         USDT_outputs[0].outputReturn0.dataLen));
        output1.set_type((JUB::Proto::Bitcoin::ENUM_SCRIPT_TYPE_BTC) USDT_outputs[0].type);
        output1.set_allocated_return0output(return0);

        JUB::Proto::Bitcoin::OutputBTC output2;
        JUB::Proto::Bitcoin::StandardOutput *standard = new JUB::Proto::Bitcoin::StandardOutput();
        standard->set_amount(USDT_outputs[1].outputStandard.amount);
        standard->set_changeaddress(USDT_outputs[1].outputStandard.changeAddress);
        standard->set_address(USDT_outputs[1].outputStandard.address);
        JUB::Proto::Common::Bip32Path *path = new JUB::Proto::Common::Bip32Path();
        path->set_change(false);
        path->set_addressindex(0);
        standard->set_allocated_path(path);
        output2.set_type((JUB::Proto::Bitcoin::ENUM_SCRIPT_TYPE_BTC) USDT_outputs[1].type);
        output2.set_allocated_standardoputput(standard);

        resultOutputs.add_value()->PackFrom(output1);
        resultOutputs.add_value()->PackFrom(output2);
    }

    std::string result;
    resultOutputs.SerializeToString(&result);
    return stdString2jbyteArray(env, result);
}


//==================================== JUB_SDK_ETH_H ==========================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH(JNIEnv *env, jobject obj, jbyteArray jcfg, jint deviceID) {

    JUB::Proto::Ethereum::ContextCfgETH pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.chainID = pbCfg.chainid();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextETH(cfg, deviceID, &contextID);
        return buildPbRvUInt(env, rv, contextID);
    } else {
        return buildPbRvUInt(env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH_soft(JNIEnv *env, jobject obj, jbyteArray jcfg, jstring xprv) {

    auto strXPRV = jstring2stdString(env, xprv);
    JUB::Proto::Ethereum::ContextCfgETH pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.chainID = pbCfg.chainid();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextETH_soft(cfg, (JUB_CHAR_PTR) strXPRV.c_str(), &contextID);
        return buildPbRvUInt(env, rv, contextID);
    } else {
        return buildPbRvUInt(env, JUBR_ARGUMENTS_BAD, 0);
    };
}


JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeETH(JNIEnv *env, jobject obj, jint contextID, jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Ethereum::ENUM_PUB_FORMAT _format;
    JUB::Proto::Ethereum::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, (JUB_ENUM_ETH_PUB_FORMAT) _format, &xpub);
    return buildPbRvString(env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeETH(JNIEnv *env, jobject obj, jint contextID, jbyteArray format, jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Ethereum::ENUM_PUB_FORMAT _format;
    JUB::Proto::Ethereum::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR xpub;
        JUB_RV rv = JUB_GetHDNodeETH(contextID, (JUB_ENUM_ETH_PUB_FORMAT) _format, bip32Path, &xpub);
        return buildPbRvString(env, rv, xpub);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressETH(JNIEnv *env, jobject obj, jint contextID, jbyteArray bip32, jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_GetAddressETH(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        LOG_ERR("JUB_GetAddressETH : %s", address);
        return buildPbRvString(env, rv, address);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}


JNIEXPORT jbyteArray JNICALL
native_SetMyAddressETH(JNIEnv *env, jobject obj, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_SetMyAddressETH(contextID, bip32Path, &address);
        return buildPbRvString(env, rv, address);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
}


JNIEXPORT jbyteArray JNICALL
native_BuildERC20AbiETH(JNIEnv *env, jobject obj, jint contextID, jstring address,
                        jstring amountInWei) {
    auto strAddress = jstring2stdString(env, address);
    auto strAmount = jstring2stdString(env, amountInWei);
    JUB_CHAR_PTR abi;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID, (JUB_CHAR_PTR) strAddress.c_str(),
                                     (JUB_CHAR_PTR) strAmount.c_str(), &abi);

    return buildPbRvString(env, rv, abi);
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionETH(JNIEnv *env, jobject obj, jint contextID, jbyteArray tx) {
    JUB::Proto::Ethereum::TransactionETH pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        BIP44_Path bip32Path;
        bip32Path.change = (JUB_ENUM_BOOL) pbTx.path().change();
        bip32Path.addressIndex = pbTx.path().addressindex();

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionETH(contextID, bip32Path, pbTx.nonce(), pbTx.gaslimit(),
                                           (JUB_CHAR_PTR) pbTx.gaspriceinwei().c_str(),
                                           (JUB_CHAR_PTR) pbTx.to().c_str(),
                                           (JUB_CHAR_PTR) pbTx.valueinwei().c_str(),
                                           (JUB_CHAR_PTR) pbTx.input().c_str(), &raw);

        return buildPbRvString(env, rv, raw);
    }
    return buildPbRvString(env, JUBR_ARGUMENTS_BAD, "");
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

JNIEXPORT jstring JNICALL native_HCShowAddress(JNIEnv *env, jobject obj, jlong contextID, jint change, jint index) {
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

JNIEXPORT jstring native_HCGetMainHDNode(JNIEnv *env, jobject obj, jlong contextID) {
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
    {
        "nativeInitDevice",
        "()I",
        (void *) native_initDevice
    },
    {
        "nativeStartScan",
        "(Lcom/jubiter/sdk/jni/InnerScanCallback;)I",
        (void *) native_startScan
    },
    {
        "nativeStopScan",
        "()I",
        (void *) native_stopScan
    },
    {
        "nativeConnectDevice",
        "(Ljava/lang/String;[IILcom/jubiter/sdk/jni/InnerDiscCallback;)I",
        (void *) native_connectDevice
    },
    {
        "nativeDisconnectDevice",
        "(I)I",
        (void *) native_disconnectDevice
    },
    {
        "nativeIsConnected",
        "(I)I",
        (void *) native_isConnectDevice
    },
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
        "nativeSendApdu",
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
        "nativeQuerryBattery",
        "(I)[B",
        (void *) native_QueryBattery
    },
    {
        "nativeBTCCreateContext",
        "([BI)[B",
        (void *) native_CreateContextBTC
    },
    {
        "nativeBTCCreateContext_Software",
        "([BLjava/lang/String;)[B",
        (void *) native_CreateContextBTC_soft
    },
    {
        "nativeBTCGetAddress",
        "(I[BZ)[B",
        (void *) native_GetAddressBTC
    },
    {
        "nativeBTCGetMainHDNode",
        "(I)[B",
        (void *) native_GetMainHDNodeBTC
    },
    {
        "nativeBTCGetHDNode",
        "(I[B)[B",
        (void *) native_GetHDNodeBTC
    },
    {
        "nativeBTCSetAddress",
        "(I[B)[B",
        (void *) native_SetMyAddressBTC
    },
    {
        "nativeBTCSignTransaction",
        "(I[B)[B",
        (void *) native_SignTransactionBTC
    },
    {
        "nativeBTCSetUnit",
        "(I[B)I",
        (void *) native_SetUnitBTC
    },
    {
        "nativeBuildUSDTOutput",
        "(ILjava/lang/String;J)[B",
        (void *) native_BuildUSDTOutputs
    },
////    {
////        "nativeParseTransaction",
////        "(JLjava/lang/String;)Ljava/lang/String;",
////        (void *) native_ParseTransactionRaw
////    },
    {
        "nativeETHCreateContext",
        "([BI)[B",
        (void *) native_CreateContextETH
    },
    {
        "nativeETHCreateContext_Software",
        "([BLjava/lang/String;)[B",
        (void *) native_CreateContextETH_soft
    },
    {
        "nativeETHGetMainHDNode",
        "(I[B)[B",
        (void *) native_GetMainHDNodeETH
    },
    {
        "nativeETHGetHDNode",
        "(I[B[B)[B",
        (void *) native_GetHDNodeETH
    },
    {
        "nativeETHGetAddress",
        "(I[BZ)[B",
        (void *) native_GetAddressETH
    },
    {
        "nativeETHSetAddress",
        "(I[B)[B",
        (void *) native_SetMyAddressETH
    },
    {
        "nativeETHBuildERC20Abi",
        "(ILjava/lang/String;Ljava/lang/String;)[B",
        (void *) native_BuildERC20AbiETH
    },
    {
        "nativeETHSignTransaction",
        "(I[B)[B",
        (void *) native_SignTransactionETH
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
        "nativeIsInitialize",
        "(I)Z",
        (void *) native_IsInitialize
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