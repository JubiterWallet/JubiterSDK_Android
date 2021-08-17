//
// Created by Journey on 2021/3/19.
//

#include "jni_trx.h"


JNIEXPORT jbyteArray JNICALL
native_CreateContextTRX(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_TRX cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextTRX(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextTRX 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextTRX 2", env, JUBR_ARGUMENTS_BAD, 0);
    };
}

JNIEXPORT jbyteArray JNICALL
native_CreateContextTRX_soft(JNIEnv *env,
                             jclass clz,
                             jbyteArray jcfg,
                             jstring xprv) {
    auto strXPRV = jstring2stdString(env, xprv);
    JUB::Proto::Common::ContextCfg pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_TRX cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextTRX_soft(cfg, (JUB_CHAR_PTR) strXPRV.c_str(), &contextID);
        return buildPbRvUInt("JUB_CreateContextTRX_soft 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextTRX_soft 2", env, JUBR_ARGUMENTS_BAD, 0);
    }
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressTRX(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address;
        JUB_RV rv = JUB_GetAddressTRX(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        LOG_DEBUG("JUB_GetAddressTRX : %s", address);
        return buildPbRvString("JUB_GetAddressTRX 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressTRX 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_CheckAddressTRX(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jstring address) {
    auto strAddress = jstring2stdString(env, address);
    JUB_CHAR_PTR addrInHex;
    JUB_RV rv = JUB_CheckAddressTRX(contextID, strAddress.c_str(), &addrInHex);
    LOG_DEBUG("JUB_CheckAddressTRX : %s", addrInHex);
    return buildPbRvString("JUB_CheckAddressTRX 1", env, rv, addrInHex);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeTRX(JNIEnv *env,
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
        JUB_RV rv = JUB_GetHDNodeTRX(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeTRX 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeTRX 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeTRX(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub;
    JUB_RV rv = JUB_GetMainHDNodeTRX(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeTRX", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressTRX(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressTRX(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressTRX 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressTRX 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionTRX(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray bip32,
                          jstring jPackedContractInPb) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        auto strContract = jstring2stdString(env, jPackedContractInPb);
        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionTRX(static_cast<JUB_UINT16>(contextID),
                                           bip32Path,
                                           strContract.c_str(),
                                           &raw);
        return buildPbRvString("JUB_SignTransactionTRX 1", env, rv, raw);
    }
    return buildPbRvString("JUB_SignTransactionTRX 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_TRXSetTRC10Asset(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jbyteArray assetName,
                        jint unitDP,
                        jbyteArray assetId) {
    auto assetNameStr = jbyteArray2stdString(env, assetName);
    auto assetIdStr = jbyteArray2stdString(env, assetId);
    JUB_RV rv = JUB_SetTRC10Asset(contextID, assetNameStr.c_str(), unitDP, assetIdStr.c_str());
    if (JUBR_OK != rv) {
        LOG_ERR("JUB_SetTRC10Asset rv: %08lx", rv);
    }
    return rv;
}

JNIEXPORT int JNICALL
native_TRXSetTRC20Token(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jstring tokenName,
                        jint unitDP,
                        jstring contractAddress) {
    auto strTokenName = jstring2stdString(env, tokenName);
    auto strContractAddress = jstring2stdString(env, contractAddress);
    JUB_RV rv = JUB_SetTRC20Token(contextID,
                                  (JUB_CHAR_PTR) strTokenName.c_str(),
                                  unitDP,
                                  (JUB_CHAR_PTR) strContractAddress.c_str());
    LOG_DEBUG("JUB_SetTRC20Token rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_TRXBuildTRC20TransferAbi(JNIEnv *env,
                                jclass clz,
                                jint contextID,
                                jstring address,
                                jstring amount) {
    auto strAddress = jstring2stdString(env, address);
    auto strAmount = jstring2stdString(env, amount);
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildTRC20TransferAbi(contextID,
                                          (JUB_CHAR_PTR) strAddress.c_str(),
                                          (JUB_CHAR_PTR) strAmount.c_str(),
                                          &abi);

    return buildPbRvString("JUB_BuildTRC20Abi", env, rv, abi);
}

JNIEXPORT jbyteArray JNICALL
native_TRXPackContract(JNIEnv *env,
                       jclass obj,
                       jlong contextID,
                       jbyteArray tx) {
    protocol::Transaction pbTx;
    if (!parseFromJbyteArray(env, tx, &pbTx)) {
        return buildPbRvString("JUB_PackContractTRX 1", env, JUBR_ARGUMENTS_BAD, "");
    }

    JUB_CHAR_PTR raw = nullptr;
    JUB_TX_TRX trxTx;
    trxTx.ref_block_bytes = (JUB_CHAR_PTR) pbTx.raw_data().ref_block_bytes().c_str();
    trxTx.ref_block_hash = (JUB_CHAR_PTR) pbTx.raw_data().ref_block_hash().c_str();
    trxTx.ref_block_num = nullptr;
    trxTx.data = nullptr;
    trxTx.expiration = (JUB_CHAR_PTR) std::to_string(pbTx.raw_data().expiration()).c_str();
    trxTx.timestamp = (JUB_CHAR_PTR) std::to_string(pbTx.raw_data().timestamp()).c_str();
    trxTx.fee_limit = (JUB_CHAR_PTR) std::to_string(pbTx.raw_data().fee_limit()).c_str();

    std::vector <JUB_CONTRACT_TRX> contractTrxs;
    JUB_CONTRACT_TRX contractTrx;
    contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::NS_ITEM_TRX_CONTRACT;
    switch (pbTx.raw_data().contract(0).type()) {
        case protocol::Transaction::Contract::TransferContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT;
            protocol::TransferContract pbTrxCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbTrxCrt);
            JUB_XFER_CONTRACT_TRX transfer;
            transfer.owner_address = (JUB_CHAR_PTR) pbTrxCrt.owner_address().c_str();
            transfer.to_address = (JUB_CHAR_PTR) pbTrxCrt.to_address().c_str();
            transfer.amount = pbTrxCrt.amount();
            contractTrx.transfer = transfer;
            break;
        }
        case protocol::Transaction::Contract::TransferAssetContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT;
            protocol::TransferAssetContract pbAssetCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbAssetCrt);
            JUB_XFER_ASSET_CONTRACT_TRX transferAsset;
            transferAsset.owner_address = (JUB_CHAR_PTR) pbAssetCrt.owner_address().c_str();
            transferAsset.asset_name = (JUB_CHAR_PTR) pbAssetCrt.asset_name().c_str();
            transferAsset.to_address = (JUB_CHAR_PTR) pbAssetCrt.to_address().c_str();
            transferAsset.amount = pbAssetCrt.amount();
            contractTrx.transferAsset = transferAsset;
            break;
        }
        case protocol::Transaction::Contract::CreateSmartContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::CREATE_SMART_CONTRACT;
            protocol::CreateSmartContract pbCreateCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbCreateCrt);
            JUB_CREATE_SMART_CONTRACT_TRX createSmart;
            createSmart.owner_address = (JUB_CHAR_PTR) pbCreateCrt.owner_address().c_str();
            createSmart.call_token_value = pbCreateCrt.call_token_value();
            createSmart.token_id = pbCreateCrt.token_id();
            createSmart.bytecode = (JUB_CHAR_PTR) pbCreateCrt.new_contract().bytecode().c_str();
            contractTrx.createSmart = createSmart;
            break;
        }
        case protocol::Transaction::Contract::TriggerSmartContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
            protocol::TriggerSmartContract pbTriggerCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbTriggerCrt);
            JUB_TRIG_SMART_CONTRACT_TRX triggerSmart;
            triggerSmart.owner_address = (JUB_CHAR_PTR) pbTriggerCrt.owner_address().c_str();
            triggerSmart.contract_address = (JUB_CHAR_PTR) pbTriggerCrt.contract_address().c_str();
            triggerSmart.call_value = pbTriggerCrt.call_value();
            triggerSmart.data = (JUB_CHAR_PTR) pbTriggerCrt.data().c_str();
            triggerSmart.call_token_value = pbTriggerCrt.call_token_value();
            triggerSmart.token_id = pbTriggerCrt.token_id();
            contractTrx.triggerSmart = triggerSmart;
            break;
        }
        case protocol::Transaction::Contract::FreezeBalanceContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT;
            protocol::FreezeBalanceContract pbFreezeBalanceCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbFreezeBalanceCrt);
            JUB_FRZ_BLA_CONTRACT_TRX freezeBalance;
            freezeBalance.owner_address = (JUB_CHAR_PTR) pbFreezeBalanceCrt.owner_address().c_str();
            freezeBalance.receiver_address = (JUB_CHAR_PTR) pbFreezeBalanceCrt.receiver_address().c_str();
            freezeBalance.frozen_balance = pbFreezeBalanceCrt.frozen_balance();
            freezeBalance.frozen_duration = pbFreezeBalanceCrt.frozen_duration();
            contractTrx.freezeBalance = freezeBalance;
            break;
        }
        case protocol::Transaction::Contract::UnfreezeBalanceContract: {
            contractTrx.type = JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT;
            protocol::UnfreezeBalanceContract pbUnfreezeBalanceCrt;
            pbTx.raw_data().contract(0).parameter().UnpackTo(&pbUnfreezeBalanceCrt);
            JUB_UNFRZ_BLA_CONTRACT_TRX unfreezeBalance;
            unfreezeBalance.owner_address = (JUB_CHAR_PTR) pbUnfreezeBalanceCrt.owner_address().c_str();
            unfreezeBalance.receiver_address = (JUB_CHAR_PTR) pbUnfreezeBalanceCrt.receiver_address().c_str();
            contractTrx.unfreezeBalance = unfreezeBalance;
            break;
        }
        default:
            break;
    }
    contractTrxs.push_back(contractTrx);
    trxTx.contracts = &contractTrxs[0];
    trxTx.contractCount = 1;

    JUB_CHAR_PTR packedContractInPB = nullptr;
    JUB_RV rv = JUB_PackContractTRX(static_cast<JUB_UINT16>(contextID), trxTx,
                                    &packedContractInPB);

    return buildPbRvString("JUB_PackContractTRX 2", env, rv, packedContractInPB);
}

JNIEXPORT int JNICALL
native_TRXSetTRC721Token(JNIEnv *env,
                         jclass clz,
                         jint contextID,
                         jstring tokenName,
                         jstring contractAddress) {
    auto strTokenName = jstring2stdString(env, tokenName);
    auto strContractAddress = jstring2stdString(env, contractAddress);
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_SetTRC721Token(contextID,
                                   (JUB_CHAR_PTR) strTokenName.c_str(),
                                   (JUB_CHAR_PTR) strContractAddress.c_str());
    LOG_DEBUG("JUB_SetTRC721Token rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_TRXBuildTRC721TransferAbi(JNIEnv *env,
                                 jclass clz,
                                 jint contextID,
                                 jstring tokenFrom,
                                 jstring tokenTo,
                                 jstring tokenID) {
    auto strTokenFrom = jstring2stdString(env, tokenFrom);
    auto strTokenTo = jstring2stdString(env, tokenTo);
    auto strTokenID = jstring2stdString(env, tokenID);
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildTRC721TransferAbi(contextID,
                                           (JUB_CHAR_PTR) strTokenFrom.c_str(),
                                           (JUB_CHAR_PTR) strTokenTo.c_str(),
                                           (JUB_CHAR_PTR) strTokenID.c_str(),
                                           &abi);

    return buildPbRvString("JUB_BuildTRC721Abi", env, rv, abi);
}


JNINativeMethod trxNativeMethods[] = {
        {
                "nativeTRXCreateContext",
                "([BI)[B",
                (void *) native_CreateContextTRX
        },
        {
                "nativeTRXCreateContext_Software",
                "([BLjava/lang/String;)[B",
                (void *) native_CreateContextTRX_soft
        },
        {
                "nativeTRXGetAddress",
                "(I[BZ)[B",
                (void *) native_GetAddressTRX
        },
        {
                "nativeTRXCheckAddress",
                "(ILjava/lang/String;)[B",
                (void *) native_CheckAddressTRX
        },
        {
                "nativeTRXGetHDNode",
                "(I[B[B)[B",
                (void *) native_GetHDNodeTRX
        },
        {
                "nativeTRXGetMainHDNode",
                "(I[B)[B",
                (void *) native_GetMainHDNodeTRX
        },
        {
                "nativeTRXSetAddress",
                "(I[B)[B",
                (void *) native_SetMyAddressTRX
        },
        {
                "nativeTRXSignTransaction",
                "(I[BLjava/lang/String;)[B",
                (void *) native_SignTransactionTRX
        },
        {
                "nativeTRXSetTRC20Token",
                "(ILjava/lang/String;ILjava/lang/String;)I",
                (void *) native_TRXSetTRC20Token
        },
        {
                "nativeTRXBuildTRC20TransferAbi",
                "(ILjava/lang/String;Ljava/lang/String;)[B",
                (void *) native_TRXBuildTRC20TransferAbi
        },
        {
                "nativeTRXSetTRC10Asset",
                "(I[BI[B)I",
                (void *) native_TRXSetTRC10Asset
        },
        {
                "nativeTRXPackContract",
                "(I[B)[B",
                (void *) native_TRXPackContract
        },
        {
                "nativeTRXSetTRC721Token",
                "(ILjava/lang/String;Ljava/lang/String;)I",
                (void *) native_TRXSetTRC721Token
        },
        {
                "nativeTRXBuildTRC721TransferAbi",
                "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)[B",
                (void *) native_TRXBuildTRC721TransferAbi
        },
};

#define TRX_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getTrxClass(JNIEnv *env) {
    return env->FindClass(TRX_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getTrxNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(trxNativeMethods) / sizeof(trxNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(trxNativeMethods[i]);
    }
    return methodList;
}