//
// Created by Journey on 2021/4/12.
//
#include "jni_xrp.h"


JNIEXPORT jbyteArray JNICALL
native_CreateContextXRP(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {
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
native_GetAddressXRP(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_GetAddressXRP(contextID,
                                      bip32Path,
                                      (JUB_ENUM_BOOL) bShow,
                                      &address);
        LOG_DEBUG("JUB_GetAddressXRP : %s", address);
        return buildPbRvString("JUB_GetAddressXRP 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeXRP(JNIEnv *env,
                    jclass clz,
                    jint contextID,
                    jbyteArray format,
                    jbyteArray bip32) {

    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub;
        JUB_RV rv = JUB_GetHDNodeXRP(contextID,
                                     (JUB_ENUM_PUB_FORMAT) _format,
                                     bip44Path,
                                     &xpub);
        return buildPbRvString("JUB_GetHDNodeXRP 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeXRP 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeXRP(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeXRP(contextID,
                                     (JUB_ENUM_PUB_FORMAT) _format,
                                     &xpub);
    return buildPbRvString("JUB_GetMainHDNodeXRP", env, rv, xpub);
}

//JNIEXPORT jbyteArray JNICALL
//native_SetMyAddressXRP(JNIEnv *env, jclass clz, jint contextID, jbyteArray bip32) {
//    BIP44_Path bip32Path;
//    if (parseBip44Path(env, bip32, &bip32Path)) {
//        JUB_CHAR_PTR address = nullptr;
//        JUB_RV rv = JUB_SetMyAddressXRP(contextID, bip32Path, &address);
//        return buildPbRvString("JUB_SetMyAddressXRP 1", env, rv, address);
//    }
//    return buildPbRvString("JUB_SetMyAddressXRP 2", env, JUBR_ARGUMENTS_BAD, "");
//}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionXRP(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray bip32,
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
                        case JUB::Proto::Ripple::ENUM_XRP_PYMT_TYPE ::DXRP: {
                            tx.pymt.type =  JUB_ENUM_XRP_PYMT_TYPE::DXRP;
                            tx.pymt.amount;
                            tx.pymt.amount.value =(JUB_CHAR_PTR)  pbTx.pymt().amount().value().c_str();
                            tx.pymt.destination = (JUB_CHAR_PTR)  pbTx.pymt().destination().c_str();
                            tx.pymt.destinationTag = (JUB_CHAR_PTR)  pbTx.pymt().destination_tag().c_str();
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
native_CheckAddressXRP(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jstring address) {
    JUB::Proto::Common::ResultAny resultAddrParse;
    auto strAddress = jstring2stdString(env, address);
    JUB_CHAR_PTR addr = nullptr;
    JUB_CHAR_PTR tag = nullptr;
    JUB_RV rv = JUB_CheckAddressXRP(contextID,
                                    strAddress.c_str(),
                                    &addr,
                                    &tag);
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


JNINativeMethod xrpNativeMethods[] = {
        {
                "nativeXRPCreateContext",
                "([BI)[B",
                (void *) native_CreateContextXRP
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
//        {
//                "nativeXRPSetAddress",
//                "(I[B)[B",
//                (void *) native_SetMyAddressXRP
//        },
        {
                "nativeXRPSignTransaction",
                "(I[B[B)[B",
                (void *) native_SignTransactionXRP
        },
};


#define XRP_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getXrpClass(JNIEnv *env) {
    return env->FindClass(XRP_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getXrpNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(xrpNativeMethods) / sizeof(xrpNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(xrpNativeMethods[i]);
    }
    return methodList;
}