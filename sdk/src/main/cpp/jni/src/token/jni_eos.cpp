//
// Created by Journey on 2021/4/12.
//
#include "jni_eos.h"

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

//JNIEXPORT jbyteArray JNICALL
//native_SetMyAddressEOS(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
//    BIP44_Path bip32Path;
//    if (parseBip44Path(env, bip32, &bip32Path)) {
//        JUB_CHAR_PTR address = nullptr;
//        JUB_RV rv = JUB_SetMyAddressEOS(contextID, bip32Path, &address);
//        return buildPbRvString("JUB_SetMyAddressEOS 1", env, rv, address);
//    }
//    return buildPbRvString("JUB_SetMyAddressEOS 2", env, JUBR_ARGUMENTS_BAD, "");
//}

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


JNINativeMethod eosNativeMethods[] = {
        {
                "nativeEOSCreateContext",
                "([BI)[B",
                (void *) native_CreateContextEOS
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
//        {
//                "nativeEOSSetAddress",
//                "(I[B)[B",
//                (void *) native_SetMyAddressEOS
//        },
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
};


#define EOS_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getEosClass(JNIEnv *env) {
    return env->FindClass(EOS_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getEosNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(eosNativeMethods) / sizeof(eosNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(eosNativeMethods[i]);
    }
    return methodList;
}