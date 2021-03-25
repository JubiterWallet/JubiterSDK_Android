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

//===================================== JUB_SDK_BTC ============================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC_soft(JNIEnv *env, jclass clz, jbyteArray jcfg, jstring xprv) {
    JUB::Proto::Bitcoin::ContextCfgBTC pbCfg;
    auto strXprv = jstring2stdString(env, xprv);
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.coinType = (JUB_ENUM_COINTYPE_BTC) pbCfg.coin_type();
        cfg.transType = (JUB_ENUM_BTC_TRANS_TYPE) pbCfg.trans_type();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextBTC_soft(cfg, (JUB_CHAR_PTR) strXprv.c_str(), &contextID);
        return buildPbRvUInt("JUB_CreateContextBTC_soft 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextBTC_soft 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}


JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID) {
    JUB::Proto::Bitcoin::ContextCfgBTC pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.coinType = (JUB_ENUM_COINTYPE_BTC) pbCfg.coin_type();
        cfg.transType = (JUB_ENUM_BTC_TRANS_TYPE) pbCfg.trans_type();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextBTC 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextBTC 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL native_GetMainHDNodeBTC(JNIEnv *env, jclass clz, jint contextID) {
    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeBTC", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeBTC(contextID, bip32Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeBTC 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressBTC(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressBTC 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_CheckAddressBTC(JNIEnv *env, jclass clz, jint contextID, jstring address) {
    auto strAddress = jstring2stdString(env, address);
    JUB_RV rv = JUB_CheckAddressBTC(contextID, strAddress.c_str());
    LOG_DEBUG("JUB_CheckAddressBTC rv: %d", rv);
    return rv;
}


JNIEXPORT jbyteArray JNICALL
native_SetMyAddressBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressBTC(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressBTC 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray jTX) {

    JUB::Proto::Bitcoin::TransactionBTC tx;
    if (parseFromJbyteArray(env, jTX, &tx)) {
        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        JUB_UINT32 lockTime = tx.locktime();
        JUB_UINT32 version = tx.version();

        LOG_DEBUG("lockTime： %d", lockTime);
        LOG_DEBUG("version： %d", version);

        for (auto i = 0; i < tx.inputs_size(); i++) {
            INPUT_BTC input;
            input.amount = tx.inputs(i).amount();
            input.preIndex = tx.inputs(i).pre_index();
            input.preHash = (JUB_CHAR_PTR) tx.inputs(i).pre_hash().c_str();
            input.path.change = (JUB_ENUM_BOOL) tx.inputs(i).path().change();
            input.path.addressIndex = tx.inputs(i).path().address_index();
            input.nSequence = 0xFFFFFFFF;
            inputs.emplace_back(input);
        }

        for (auto i = 0; i < tx.outputs_size(); i++) {
            OUTPUT_BTC output;
            output.type = (JUB_ENUM_SCRIPT_BTC_TYPE) tx.outputs(i).type();
            switch (output.type) {
                case P2PKH:
                    if (!tx.outputs(i).has_std_output())
                        return buildPbRvString("JUB_SignTransactionBTC 1", env, JUBR_ARGUMENTS_BAD,
                                               "");
                    output.stdOutput.amount = tx.outputs(i).std_output().amount();
                    output.stdOutput.address = (JUB_CHAR_PTR) tx.outputs(
                            i).std_output().address().c_str();
                    output.stdOutput.changeAddress = (JUB_ENUM_BOOL) tx.outputs(
                            i).std_output().change_address();
                    output.stdOutput.path.addressIndex = tx.outputs(
                            i).std_output().path().address_index();
                    output.stdOutput.path.change = (JUB_ENUM_BOOL) tx.outputs(
                            i).std_output().path().change();
                    break;
                case RETURN0:
                    if (!tx.outputs(i).has_return0_output())
                        return buildPbRvString("JUB_SignTransactionBTC 2", env, JUBR_ARGUMENTS_BAD,
                                               "");
                    if (tx.outputs(i).return0_output().data().size() > 40)
                        return buildPbRvString("JUB_SignTransactionBTC 3", env, JUBR_ARGUMENTS_BAD,
                                               "");
                    output.return0.amount = tx.outputs(i).return0_output().amount();
                    {
                        auto byteData = HexStr2CharPtr(tx.outputs(i).return0_output().data());
                        output.return0.dataLen = byteData.size();
                        memcpy(output.return0.data, &byteData[0], byteData.size());
                    }
                    break;
                default:
                    return buildPbRvString("JUB_SignTransactionBTC 4", env, JUBR_ARGUMENTS_BAD, "");
            }
            outputs.emplace_back(output);
        }

        JUB_CHAR_PTR raw = nullptr;
        // todo fix version 需要调整 PB，暂时使用固定值
        // 1: 2009 年创世引入
        // 2： BIP34
        // 3： BIP66
        // 4： BIP65
        JUB_RV rv = JUB_SignTransactionBTC(contextID,
                                           version,
                                           &inputs[0], inputs.size(),
                                           &outputs[0], outputs.size(),
                                           lockTime,
                                           &raw);
        return buildPbRvString("JUB_SignTransactionBTC 5", env, rv, raw);
    }

    return buildPbRvString("JUB_SignTransactionBTC 6", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_SetUnitBTC(JNIEnv *env, jclass clz, jint contextID, jbyteArray junit) {

    auto strUnit = jbyteArray2stdString(env, junit);

    JUB::Proto::Bitcoin::BTC_UNIT_TYPE unit;
    JUB::Proto::Bitcoin::BTC_UNIT_TYPE_Parse(strUnit, &unit);
    JUB_RV rv = JUB_SetUnitBTC(contextID, (JUB_ENUM_BTC_UNIT_TYPE) unit);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BuildUSDTOutputs(JNIEnv *env, jclass clz, jint contextID, jstring USDTTO, jlong amount) {

    auto strUSDTTO = jstring2stdString(env, USDTTO);

    JUB::Proto::Common::ResultAny resultOutputs;
    OUTPUT_BTC USDT_outputs[2] = {};
    JUB_RV rv = JUB_BuildUSDTOutputs(contextID, (JUB_CHAR_PTR) strUSDTTO.c_str(), amount,
                                     USDT_outputs);
    resultOutputs.set_state_code(rv);
    if (rv == JUBR_OK) {
        //first output is return0,second is standard
        JUB::Proto::Bitcoin::OutputBTC output1;
        JUB::Proto::Bitcoin::Return0Output *return0 = new JUB::Proto::Bitcoin::Return0Output();
        return0->set_amount(USDT_outputs[0].return0.amount);
        return0->set_data(CharPtr2HexStr(USDT_outputs[0].return0.data,
                                         USDT_outputs[0].return0.dataLen));
        output1.set_type((JUB::Proto::Bitcoin::ENUM_SCRIPT_TYPE_BTC) USDT_outputs[0].type);
        output1.set_allocated_return0_output(return0);

        JUB::Proto::Bitcoin::OutputBTC output2;
        JUB::Proto::Bitcoin::StandardOutput *standard = new JUB::Proto::Bitcoin::StandardOutput();
        standard->set_amount(USDT_outputs[1].stdOutput.amount);
        standard->set_change_address(USDT_outputs[1].stdOutput.changeAddress);
        standard->set_address(USDT_outputs[1].stdOutput.address);
        JUB::Proto::Common::Bip44Path *path = new JUB::Proto::Common::Bip44Path();
        path->set_change(false);
        path->set_address_index(0);
        standard->set_allocated_path(path);
        output2.set_type((JUB::Proto::Bitcoin::ENUM_SCRIPT_TYPE_BTC) USDT_outputs[1].type);
        output2.set_allocated_std_output(standard);

        resultOutputs.add_value()->PackFrom(output1);
        resultOutputs.add_value()->PackFrom(output2);
    }

    std::string result;
    resultOutputs.SerializeToString(&result);
    return stdString2jbyteArray("JUB_BuildUSDTOutputs", env, result);
}

JNIEXPORT jbyteArray JNICALL
native_BuildQRC20Output(JNIEnv *env, jclass clz, jint contextID,
                         jstring contractAddr, jint decimal,
                         jstring symbol, jlong gasLimit,
                         jlong gasPrice, jstring to,
                         jstring amount) {

    auto strContractAddr = jstring2stdString(env, contractAddr);
    auto strSymbol = jstring2stdString(env, symbol);
    auto strTo = jstring2stdString(env, to);
    auto strAmount = jstring2stdString(env, amount);

    JUB::Proto::Common::ResultAny resultOutputs;
    OUTPUT_BTC QRC20_outputs[1] = {};
    JUB_RV rv = JUB_BuildQRC20Outputs(contextID, (JUB_CHAR_PTR) strContractAddr.c_str(), decimal,
                                      (JUB_CHAR_PTR) strSymbol.c_str(), gasLimit, gasPrice,
                                      (JUB_CHAR_PTR) strTo.c_str(), (JUB_CHAR_PTR) strAmount.c_str(),
                                      QRC20_outputs);
    resultOutputs.set_state_code(rv);
    if (rv == JUBR_OK) {
        JUB::Proto::Bitcoin::OutputBTC output;
        JUB::Proto::Bitcoin::QRC20Output *qrc20 = new JUB::Proto::Bitcoin::QRC20Output();
        qrc20->set_data(CharPtr2HexStr(QRC20_outputs[0].qrc20.data,
                                       QRC20_outputs[0].qrc20.dataLen));
        output.set_type((JUB::Proto::Bitcoin::ENUM_SCRIPT_TYPE_BTC) QRC20_outputs[0].type);

        output.set_allocated_qrc20_output(qrc20);
        resultOutputs.add_value()->PackFrom(output);
    }

    std::string result;
    resultOutputs.SerializeToString(&result);
    return stdString2jbyteArray("JUB_BuildQRC20Outputs", env, result);
}


//==================================== JUB_SDK_ETH_H ==========================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID) {

    JUB::Proto::Ethereum::ContextCfgETH pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.chainID = pbCfg.chain_id();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextETH(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextETH 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextETH 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH_soft(JNIEnv *env, jclass clz, jbyteArray jcfg, jstring xprv) {

    auto strXPRV = jstring2stdString(env, xprv);
    JUB::Proto::Ethereum::ContextCfgETH pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.chainID = pbCfg.chain_id();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextETH_soft(cfg, (JUB_CHAR_PTR) strXPRV.c_str(), &contextID);
        return buildPbRvUInt("JUB_CreateContextETH_soft 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextETH_soft 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}


JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeETH", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeETH(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeETH 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressETH(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressETH 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressETH 2", env, JUBR_ARGUMENTS_BAD, "");
}


JNIEXPORT jbyteArray JNICALL
native_SetMyAddressETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressETH(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressETH 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressETH 2", env, JUBR_ARGUMENTS_BAD, "");
}


JNIEXPORT jbyteArray JNICALL
native_BuildERC20AbiETH(JNIEnv *env, jclass clz, jint contextID, jstring tokenName,
                        jint unitDP,
                        jstring contractAddress,
                        jstring address,
                        jstring amountInWei) {
    auto strAddress = jstring2stdString(env, address);
    auto strAmount = jstring2stdString(env, amountInWei);
    auto strTokenName = jstring2stdString(env, tokenName);
    auto strContractAddress = jstring2stdString(env, contractAddress);
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID, (JUB_CHAR_PTR) strTokenName.c_str(), unitDP,
                                     (JUB_CHAR_PTR) strContractAddress.c_str(),
                                     (JUB_CHAR_PTR) strAddress.c_str(),
                                     (JUB_CHAR_PTR) strAmount.c_str(), &abi);

    return buildPbRvString("JUB_BuildERC20AbiETH 1", env, rv, abi);
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx) {
    JUB::Proto::Ethereum::TransactionETH pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        BIP44_Path bip32Path;
        bip32Path.change = (JUB_ENUM_BOOL) pbTx.path().change();
        bip32Path.addressIndex = pbTx.path().address_index();

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionETH(contextID, bip32Path, pbTx.nonce(), pbTx.gas_limit(),
                                           (JUB_CHAR_PTR) pbTx.gas_price_in_wei().c_str(),
                                           (JUB_CHAR_PTR) pbTx.to().c_str(),
                                           (JUB_CHAR_PTR) pbTx.value_in_wei().c_str(),
                                           (JUB_CHAR_PTR) pbTx.input().c_str(), &raw);

        return buildPbRvString("JUB_SignTransactionETH", env, rv, raw);
    }
    return buildPbRvString("JUB_SignTransactionETH", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignBytestringETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jstring data) {
    BIP44_Path bip32Path;
    auto strData = jstring2stdString(env, data);
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR signature = nullptr;
        JUB_RV rv = JUB_SignBytestringETH(contextID, bip32Path, (JUB_CHAR_PTR) strData.c_str(),
                                          &signature);
        return buildPbRvString("JUB_SignBytestringETH 1", env, rv, signature);
    }
    return buildPbRvString("JUB_SignBytestringETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignContractETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx) {
    JUB::Proto::Ethereum::TransactionETH pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        BIP44_Path bip32Path;
        bip32Path.change = (JUB_ENUM_BOOL) pbTx.path().change();
        bip32Path.addressIndex = pbTx.path().address_index();

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignContractETH(contextID, bip32Path, pbTx.nonce(), pbTx.gas_limit(),
                                        (JUB_CHAR_PTR) pbTx.gas_price_in_wei().c_str(),
                                        (JUB_CHAR_PTR) pbTx.to().c_str(),
                                        (JUB_CHAR_PTR) pbTx.value_in_wei().c_str(),
                                        (JUB_CHAR_PTR) pbTx.input().c_str(), &raw);

        return buildPbRvString("JUB_SignContractETH", env, rv, raw);
    }
    return buildPbRvString("JUB_SignContractETH", env, JUBR_ARGUMENTS_BAD, "");
}
//=================================== EOS Wallet =========================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextEOS(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID) {
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_EOS cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextEOS(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextEOS 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextEOS 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL
native_CreateContextEOS_soft(JNIEnv *env, jclass clz, jbyteArray jcfg, jstring xprv) {
    auto strXPRV = jstring2stdString(env, xprv);
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_EOS cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextEOS_soft(cfg, (JUB_CHAR_PTR) strXPRV.c_str(), &contextID);
        return buildPbRvUInt("JUB_CreateContextEOS_soft 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextEOS_soft 2", env, JUBR_ARGUMENTS_BAD, 0);
    }
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressEOS(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        LOG_DEBUG("JUB_GetAddressEOS : %s", address);
        return buildPbRvString("JUB_GetAddressEOS 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressEOS 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeEOS(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeEOS 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeEOS 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeEOS(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeEOS", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressEOS(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressEOS 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressEOS 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx) {
    JUB::Proto::EOS::TransactionEOS pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        BIP44_Path bip32Path;
        bip32Path.change = (JUB_ENUM_BOOL) pbTx.path().change();
        bip32Path.addressIndex = pbTx.path().address_index();

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionEOS(contextID, bip32Path,
                                           (JUB_CHAR_PTR) pbTx.chainid().c_str(),
                                           (JUB_CHAR_PTR) pbTx.expiration().c_str(),
                                           (JUB_CHAR_PTR) pbTx.referenceblockid().c_str(),
                                           (JUB_CHAR_PTR) pbTx.referenceblocktime().c_str(),
                                           (JUB_CHAR_PTR) pbTx.actionsinjson().c_str(), &raw);

        return buildPbRvString("JUB_SignTransactionEOS 1", env, rv, raw);
    }
    return buildPbRvString("JUB_SignTransactionEOS 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_BuildActionEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray tx) {
    JUB::Proto::EOS::ActionListEOS pbAcList;
    if (parseFromJbyteArray(env, tx, &pbAcList)) {

        std::vector<JUB_ACTION_EOS> actionArray;

        for (int i = 0; i < pbAcList.actions_size(); i++) {
            const JUB::Proto::EOS::ActionEOS &pdAc = pbAcList.actions(i);
            JUB_ACTION_EOS action;
            switch (pdAc.type()) {
                case JUB::Proto::EOS::ENUM_EOS_ACTION_TYPE::XFER:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::XFER;
                    action.transfer.from = (JUB_CHAR_PTR) pdAc.xfer_action().from().c_str();
                    action.transfer.to = (JUB_CHAR_PTR) pdAc.xfer_action().to().c_str();
                    action.transfer.asset = (JUB_CHAR_PTR) pdAc.xfer_action().asset().c_str();
                    action.transfer.memo = (JUB_CHAR_PTR) pdAc.xfer_action().memo().c_str();
                    break;
                case JUB::Proto::EOS::ENUM_EOS_ACTION_TYPE::DELE:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::DELE;
                    action.delegate.from = (JUB_CHAR_PTR) pdAc.dele_action().from().c_str();
                    action.delegate.receiver = (JUB_CHAR_PTR) pdAc.dele_action().receiver().c_str();
                    action.delegate.netQty = (JUB_CHAR_PTR) pdAc.dele_action().net_qty().c_str();
                    action.delegate.cpuQty = (JUB_CHAR_PTR) pdAc.dele_action().cpu_qty().c_str();
                    action.delegate.transfer = pdAc.dele_action().transfer();
                    action.delegate.bStake = pdAc.dele_action().stake();
                    break;
                case JUB::Proto::EOS::ENUM_EOS_ACTION_TYPE::UNDELE:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::UNDELE;
                    action.delegate.from = (JUB_CHAR_PTR) pdAc.dele_action().from().c_str();
                    action.delegate.receiver = (JUB_CHAR_PTR) pdAc.dele_action().receiver().c_str();
                    action.delegate.netQty = (JUB_CHAR_PTR) pdAc.dele_action().net_qty().c_str();
                    action.delegate.cpuQty = (JUB_CHAR_PTR) pdAc.dele_action().cpu_qty().c_str();
                    action.delegate.bStake = pdAc.dele_action().stake();
                    break;
                case JUB::Proto::EOS::ENUM_EOS_ACTION_TYPE::BUYRAM:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::BUYRAM;
                    action.buyRam.payer = (JUB_CHAR_PTR) pdAc.buy_ram_action().payer().c_str();
                    action.buyRam.quant = (JUB_CHAR_PTR) pdAc.buy_ram_action().quant().c_str();
                    action.buyRam.receiver = (JUB_CHAR_PTR) pdAc.buy_ram_action().receiver().c_str();
                    break;
                case JUB::Proto::EOS::ENUM_EOS_ACTION_TYPE::SELLRAM:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::SELLRAM;
                    action.sellRam.account = (JUB_CHAR_PTR) pdAc.sell_ram_action().account().c_str();
                    action.sellRam.bytes = (JUB_CHAR_PTR) pdAc.sell_ram_action().byte().c_str();
                    break;
                default:
                    action.type = JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_EOS_ACTION_TYPE;
                    break;
            }
            action.currency = (JUB_CHAR_PTR) pdAc.currency().c_str();
            action.name = (JUB_CHAR_PTR) pdAc.name().c_str();

            actionArray.push_back(action);
        }
        JUB_CHAR_PTR actions = nullptr;
        JUB_RV rv = JUB_BuildActionEOS(contextID, &actionArray[0],
                                       pbAcList.actions_size(), &actions);

        return buildPbRvString("JUB_BuildActionEOS 1", env, rv, actions);
    }
    return buildPbRvString("JUB_BuildActionEOS 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_CalculateMemoHashEOS(JNIEnv *env, jclass clz, jstring memo) {
    auto strMemo = jstring2stdString(env, memo);
    JUB_CHAR_PTR memoHash = nullptr;
    JUB_RV rv = JUB_CalculateMemoHash((JUB_CHAR_PTR) strMemo.c_str(), &memoHash);
    return buildPbRvString("JUB_CalculateMemoHash", env, rv, memoHash);
}

//=================================== XRP Wallet =========================================

JNIEXPORT jbyteArray JNICALL
native_CreateContextXRP(JNIEnv *env, jclass clz, jbyteArray jcfg, jint deviceID) {
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_XRP cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextXRP(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextXRP 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextXRP 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL
native_CreateContextXRP_soft(JNIEnv *env, jclass clz, jbyteArray jcfg, jstring xprv) {
    auto strXPRV = jstring2stdString(env, xprv);
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_XRP cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextXRP_soft(cfg, (JUB_CHAR_PTR) strXPRV.c_str(), &contextID);
        return buildPbRvUInt("JUB_CreateContextXRP_soft 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextXRP_soft 2", env, JUBR_ARGUMENTS_BAD, 0);
    }
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32, jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_GetAddressXRP(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        LOG_DEBUG("JUB_GetAddressXRP : %s", address);
        return buildPbRvString("JUB_GetAddressXRP 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray format, jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub;
        JUB_RV rv = JUB_GetHDNodeXRP(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeXRP 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeXRP(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeXRP", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressXRP(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressXRP 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32,
                          jbyteArray tx) {
    JUB::Proto::Ripple::TransactionXRP pbTx;
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        if (parseFromJbyteArray(env, tx, &pbTx)) {
            JUB_CHAR_PTR raw = nullptr;
            JUB_TX_XRP tx;
            switch (pbTx.type()) {
                case JUB::Proto::Ripple::ENUM_XRP_TX_TYPE::PYMT: {
                    tx.type = JUB_ENUM_XRP_TX_TYPE::PYMT;
                    tx.account = (JUB_CHAR_PTR) pbTx.account().c_str();
                    tx.fee = (JUB_CHAR_PTR) pbTx.fee().c_str();
                    tx.sequence = (JUB_CHAR_PTR) pbTx.sequence().c_str();
                    tx.flags = (JUB_CHAR_PTR) pbTx.flags().c_str();
                    tx.lastLedgerSequence = (JUB_CHAR_PTR) pbTx.last_ledger_sequence().c_str();
                    tx.pymt;
                    switch (pbTx.pymt().type()) {
                        case JUB::Proto::Ripple::ENUM_XRP_PYMT_TYPE::DXRP: {
                            tx.pymt.type = JUB_ENUM_XRP_PYMT_TYPE::DXRP;
                            tx.pymt.amount;
                            tx.pymt.amount.value = (JUB_CHAR_PTR) pbTx.pymt().amount().value().c_str();
                            tx.pymt.destination = (JUB_CHAR_PTR) pbTx.pymt().destination().c_str();
                            tx.pymt.destinationTag = (JUB_CHAR_PTR) pbTx.pymt().destination_tag().c_str();
                            break;
                        }
                        default:
                            tx.pymt.type = JUB_ENUM_XRP_PYMT_TYPE::NS_ITEM_PYMT_TYPE;
                            break;
                    }
                    break;
                }
                default: {
                    tx.accountTxnID = (JUB_CHAR_PTR) pbTx.account_txn_id().c_str();
                    tx.sourceTag = (JUB_CHAR_PTR) pbTx.source_tag().c_str();
                    break;
                }
            }
            JUB_XRP_MEMO memo;
            memo.type = (JUB_CHAR_PTR) pbTx.memo().type().c_str();
            memo.data = (JUB_CHAR_PTR) pbTx.memo().data().c_str();
            memo.format = (JUB_CHAR_PTR) pbTx.memo().format().c_str();
            tx.memo = memo;
            JUB_RV rv = JUB_SignTransactionXRP(contextID, bip32Path,
                                               tx, &raw);

            return buildPbRvString("JUB_SignTransactionXRP 1", env, rv, raw);
        }
    }
    return buildPbRvString("JUB_SignTransactionXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_CheckAddressXRP(JNIEnv *env, jclass clz, jint contextID, jstring address) {
    JUB::Proto::Common::ResultAny resultAddrParse;
    auto strAddress = jstring2stdString(env, address);
    JUB_CHAR_PTR addr = nullptr;
    JUB_CHAR_PTR tag = nullptr;
    JUB_RV rv = JUB_CheckAddressXRP(contextID, strAddress.c_str(), &addr, &tag);
    resultAddrParse.set_state_code(rv);
    if (rv == JUBR_OK) {
        JUB::Proto::Ripple::XrpAddrParse xrpAddrParse;
        xrpAddrParse.set_r_address(addr);
        xrpAddrParse.set_tag(tag);
        resultAddrParse.add_value()->PackFrom(xrpAddrParse);
    }
    std::string result;
    resultAddrParse.SerializeToString(&result);
    return stdString2jbyteArray("JUB_CheckAddressXRP", env, result);
}
//=================================== HC Wallet =========================================

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
                "nativeBTCCheckAddress",
                "(ILjava/lang/String;)I",
                (void *) native_CheckAddressBTC
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
        {
                "nativeBuildQRC20Output",
                "(ILjava/lang/String;ILjava/lang/String;JJLjava/lang/String;Ljava/lang/String;)[B",
                (void *) native_BuildQRC20Output
        },
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
                "(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)[B",
                (void *) native_BuildERC20AbiETH
        },
        {
                "nativeETHSignTransaction",
                "(I[B)[B",
                (void *) native_SignTransactionETH
        },
        {
                "nativeETHSignBytestring",
                "(I[BLjava/lang/String;)[B",
                (void *) native_SignBytestringETH
        },
        {
                "nativeETHSignContract",
                "(I[B)[B",
                (void *) native_SignContractETH
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
        // EOS
        {
                "nativeEOSCreateContext",
                "([BI)[B",
                (void *) native_CreateContextEOS
        },
        {
                "nativeEOSCreateContext_Software",
                "([BLjava/lang/String;)[B",
                (void *) native_CreateContextEOS_soft
        },
        {
                "nativeEOSGetAddress",
                "(I[BZ)[B",
                (void *) native_GetAddressEOS
        },
        {
                "nativeEOSGetHDNode",
                "(I[B[B)[B",
                (void *) native_GetHDNodeEOS
        },
        {
                "nativeEOSGetMainHDNode",
                "(I[B)[B",
                (void *) native_GetMainHDNodeEOS
        },
        {
                "nativeEOSSetAddress",
                "(I[B)[B",
                (void *) native_SetMyAddressEOS
        },
        {
                "nativeEOSSignTransaction",
                "(I[B)[B",
                (void *) native_SignTransactionEOS
        },
        {
                "nativeEOSBuildAction",
                "(I[B)[B",
                (void *) native_BuildActionEOS
        },
        {
                "nativeEOSCalculateMemoHash",
                "(Ljava/lang/String;)[B",
                (void *) native_CalculateMemoHashEOS
        },
        // XRP
        {
                "nativeXRPCreateContext",
                "([BI)[B",
                (void *) native_CreateContextXRP
        },
        {
                "nativeXRPCreateContext_Software",
                "([BLjava/lang/String;)[B",
                (void *) native_CreateContextXRP_soft
        },
        {
                "nativeXRPGetAddress",
                "(I[BZ)[B",
                (void *) native_GetAddressXRP
        },
        {
                "nativeXRPGetHDNode",
                "(I[B[B)[B",
                (void *) native_GetHDNodeXRP
        },
        {
                "nativeXRPGetMainHDNode",
                "(I[B)[B",
                (void *) native_GetMainHDNodeXRP
        },
        {
                "nativeXRPSetAddress",
                "(I[B)[B",
                (void *) native_SetMyAddressXRP
        },
        {
                "nativeXRPSignTransaction",
                "(I[B[B)[B",
                (void *) native_SignTransactionXRP
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

    // BLE 注册
    jclass bleClazz = getBleClass(env);
    std::vector<JNINativeMethod> bleMethodList = getBleNativeMethods();
    if (env->RegisterNatives(bleClazz, bleMethodList.data(), bleMethodList.size()) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives BLE fail");
        return ret;
    }
    LOG_DEBUG(">>> RegisterNatives BLE ok");

    // NFC 注册
    jclass nfcClazz = getNfcClass(env);
    std::vector<JNINativeMethod> nfcMethodList = getNfcNativeMethods();
    if (env->RegisterNatives(nfcClazz, nfcMethodList.data(), nfcMethodList.size()) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives NFC fail");
        return ret;
    }
    LOG_DEBUG(">>> RegisterNatives NFC ok");

    // TRX 注册
    jclass trxClazz = getTrxClass(env);
    std::vector<JNINativeMethod> trxMethodList = getTrxNativeMethods();
    if (env->RegisterNatives(trxClazz, trxMethodList.data(), trxMethodList.size()) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives TRX fail");
        return ret;
    }
    LOG_DEBUG(">>> RegisterNatives TRX ok");


    // 注册 JNI 方法
    if (env->RegisterNatives(clazz, methodList.data(), methodList.size()) < JNI_OK) {
        LOG_ERR(">>> RegisterNatives fail");
        return ret;
    }
    LOG_DEBUG(">>> jni onLoad success");

    // 成功
    return JNI_VERSION_1_6;
}