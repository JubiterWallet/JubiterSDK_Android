//
// Created by Journey on 2021/4/12.
//
#include "jni_btc.h"

JNIEXPORT jbyteArray JNICALL
native_CreateContextBTC(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {
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

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeBTC(JNIEnv *env,
                        jclass clz,
                        jint contextID) {
    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeBTC", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeBTC(JNIEnv *env,
                    jclass clz,
                    jint contextID,
                    jbyteArray bip32) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeBTC(contextID, bip32Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeBTC 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressBTC(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressBTC(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressBTC 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_CheckAddressBTC(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jstring address) {
    auto strAddress = jstring2stdString(env, address);
    JUB_RV rv = JUB_CheckAddressBTC(contextID, strAddress.c_str());
    LOG_DEBUG("JUB_CheckAddressBTC rv: %d", rv);
    return rv;
}


JNIEXPORT jbyteArray JNICALL
native_SetMyAddressBTC(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressBTC(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressBTC 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressBTC 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionBTC(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray jTX) {

    JUB::Proto::Bitcoin::TransactionBTC tx;
    if (parseFromJbyteArray(env, jTX, &tx)) {
        std::vector <INPUT_BTC> inputs;
        std::vector <OUTPUT_BTC> outputs;
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
                        return buildPbRvString("JUB_SignTransactionBTC 1",
                                               env,
                                               JUBR_ARGUMENTS_BAD,
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
                        return buildPbRvString("JUB_SignTransactionBTC 2",
                                               env,
                                               JUBR_ARGUMENTS_BAD,
                                               "");
                    if (tx.outputs(i).return0_output().data().size() > 40)
                        return buildPbRvString("JUB_SignTransactionBTC 3",
                                               env,
                                               JUBR_ARGUMENTS_BAD,
                                               "");
                    output.return0.amount = tx.outputs(i).return0_output().amount();
                    {
                        auto byteData = HexStr2CharPtr(tx.outputs(i).return0_output().data());
                        output.return0.dataLen = byteData.size();
                        memcpy(output.return0.data, &byteData[0], byteData.size());
                    }
                    break;
                default:
                    return buildPbRvString("JUB_SignTransactionBTC 4",
                                           env,
                                           JUBR_ARGUMENTS_BAD,
                                           "");
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
native_SetUnitBTC(JNIEnv *env,
                  jclass clz,
                  jint contextID,
                  jbyteArray junit) {

    auto strUnit = jbyteArray2stdString(env, junit);

    JUB::Proto::Bitcoin::BTC_UNIT_TYPE unit;
    JUB::Proto::Bitcoin::BTC_UNIT_TYPE_Parse(strUnit, &unit);
    JUB_RV rv = JUB_SetUnitBTC(contextID, (JUB_ENUM_BTC_UNIT_TYPE) unit);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BuildUSDTOutputs(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jstring USDTTO,
                        jlong amount) {

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
native_BuildQRC20Output(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jstring contractAddr,
                        jint decimal,
                        jstring symbol,
                        jlong gasLimit,
                        jlong gasPrice,
                        jstring to,
                        jstring amount) {

    auto strContractAddr = jstring2stdString(env, contractAddr);
    auto strSymbol = jstring2stdString(env, symbol);
    auto strTo = jstring2stdString(env, to);
    auto strAmount = jstring2stdString(env, amount);

    JUB::Proto::Common::ResultAny resultOutputs;
    OUTPUT_BTC QRC20_outputs[1] = {};
    JUB_RV rv = JUB_BuildQRC20Outputs(contextID,
                                      (JUB_CHAR_PTR) strContractAddr.c_str(),
                                      decimal,
                                      (JUB_CHAR_PTR) strSymbol.c_str(),
                                      gasLimit,
                                      gasPrice,
                                      (JUB_CHAR_PTR) strTo.c_str(),
                                      (JUB_CHAR_PTR) strAmount.c_str(),
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


JNINativeMethod btcNativeMethods[] = {
        {
                "nativeBTCCreateContext",
                "([BI)[B",
                (void *) native_CreateContextBTC
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
};


#define BTC_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getBtcClass(JNIEnv *env) {
    return env->FindClass(BTC_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getBtcNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(btcNativeMethods) / sizeof(btcNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(btcNativeMethods[i]);
    }
    return methodList;
}