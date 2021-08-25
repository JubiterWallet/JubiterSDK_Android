//
// Created by Journey on 2021/4/12.
//
#include "jni_eth.h"

JNIEXPORT jbyteArray JNICALL
native_CreateContextETH(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {

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
native_GetMainHDNodeETH(JNIEnv *env, jclass clz, jint contextID, jbyteArray format) {
    auto strFormat = jbyteArray2stdString(env, format);
    JUB::Proto::Common::ENUM_PUB_FORMAT _format;
    JUB::Proto::Common::ENUM_PUB_FORMAT_Parse(strFormat, &_format);

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, (JUB_ENUM_PUB_FORMAT) _format, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeETH", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeETH(JNIEnv *env,
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
        JUB_RV rv = JUB_GetHDNodeETH(contextID, (JUB_ENUM_PUB_FORMAT) _format, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeETH 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressETH(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressETH(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressETH 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SetMyAddressETH(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jbyteArray bip32) {
    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_SetMyAddressETH(contextID, bip32Path, &address);
        return buildPbRvString("JUB_SetMyAddressETH 1", env, rv, address);
    }
    return buildPbRvString("JUB_SetMyAddressETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_SetERC20TokenETH(JNIEnv *env,
                        jclass clz,
                        jint contextID,
                        jstring tokenName,
                        jint unitDP,
                        jstring contractAddress) {
    auto strTokenName = jstring2stdString(env, tokenName);
    auto strContractAddress = jstring2stdString(env, contractAddress);
    JUB_RV rv = JUB_SetERC20TokenETH(contextID,
                                     (JUB_CHAR_PTR) strTokenName.c_str(),
                                     unitDP,
                                     (JUB_CHAR_PTR) strContractAddress.c_str());
    LOG_DEBUG("JUB_SetERC20TokenETH rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BuildERC20TransferAbiETH(JNIEnv *env,
                                jclass clz,
                                jint contextID,
                                jstring address,
                                jstring amountInWei) {
    auto strAddress = jstring2stdString(env, address);
    auto strAmount = jstring2stdString(env, amountInWei);
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildERC20TransferAbiETH(contextID,
                                             (JUB_CHAR_PTR) strAddress.c_str(),
                                             (JUB_CHAR_PTR) strAmount.c_str(),
                                             &abi);

    return buildPbRvString("JUB_BuildERC20TransferAbiETH", env, rv, abi);
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionETH(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray tx) {
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
native_SignBytestringETH(JNIEnv *env,
                         jclass clz,
                         jint contextID,
                         jbyteArray bip32,
                         jstring data) {
    BIP44_Path bip32Path;
    auto strData = jstring2stdString(env, data);
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR signature = nullptr;
        JUB_RV rv = JUB_SignBytestringETH(contextID, bip32Path, (JUB_CHAR_PTR)strData.c_str(), &signature);
        return buildPbRvString("JUB_SignBytestringETH 1", env, rv, signature);
    }
    return buildPbRvString("JUB_SignBytestringETH 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_SignContractETH(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jbyteArray tx) {
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

///
/// EIP-721、EIP-1155 共用 SetERC721TokenETH 接口
///
/// \param env
/// \param clz
/// \param contextID
/// \param tokenName
/// \param contractAddress
/// \return
JNIEXPORT jint JNICALL
native_SetERC721TokenETH(JNIEnv *env,
                         jclass clz,
                         jint contextID,
                         jstring tokenName,
                         jstring contractAddress) {
    auto strTokenName = jstring2stdString(env, tokenName);
    auto strContractAddress = jstring2stdString(env, contractAddress);

    JUB_RV rv = JUB_SetERC721TokenETH(contextID,
                                      (JUB_CHAR_PTR) strTokenName.c_str(),
                                      (JUB_CHAR_PTR) strContractAddress.c_str());
    LOG_DEBUG("JUB_SetERC721TokenETH rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_BuildERC721TransferAbiETH(JNIEnv *env,
                                 jclass clz,
                                 jint contextID,
                                 jstring tokenFrom,
                                 jstring tokenTo,
                                 jstring tokenID) {
    auto strTokenFrom = jstring2stdString(env, tokenFrom);
    auto strTokenTo = jstring2stdString(env, tokenTo);
    auto strTokenID = jstring2stdString(env, tokenID);
    JUB_CHAR_PTR abi = nullptr;

    JUB_RV rv = JUB_BuildERC721TransferAbiETH(contextID,
                                              (JUB_CHAR_PTR) strTokenFrom.c_str(),
                                              (JUB_CHAR_PTR) strTokenTo.c_str(),
                                              (JUB_CHAR_PTR) strTokenID.c_str(),
                                              &abi);

    return buildPbRvString("JUB_BuildERC721TransferAbiETH", env, rv, abi);
}

JNIEXPORT jbyteArray JNICALL
native_BuildERC1155TransferAbiETH(JNIEnv *env,
                                  jclass clz,
                                  jint contextID,
                                  jstring tokenFrom,
                                  jstring tokenTo,
                                  jstring tokenID,
                                  jstring tokenValue,
                                  jstring data) {
    auto strTokenFrom = jstring2stdString(env, tokenFrom);
    auto strTokenTo = jstring2stdString(env, tokenTo);
    auto strTokenID = jstring2stdString(env, tokenID);
    auto strTokenValue = jstring2stdString(env, tokenValue);
    auto strData = jstring2stdString(env, data);
    JUB_CHAR_PTR abi = nullptr;

    JUB_RV rv = JUB_BuildERC1155TransferAbiETH(contextID,
                                              (JUB_CHAR_CPTR) strTokenFrom.c_str(),
                                              (JUB_CHAR_CPTR) strTokenTo.c_str(),
                                              (JUB_CHAR_CPTR) strTokenID.c_str(),
                                              (JUB_CHAR_CPTR) strTokenValue.c_str(),
                                              (JUB_CHAR_CPTR) strData.c_str(),
                                              &abi);

    return buildPbRvString("JUB_BuildERC1155TransferAbiETH", env, rv, abi);
}


JNIEXPORT jbyteArray JNICALL
native_BuildERC1155BatchTransferAbiETH(JNIEnv *env,
                                       jclass clz,
                                       jint contextID,
                                       jstring tokenFrom,
                                       jstring tokenTo,
                                       jobjectArray tokenIDArray,
                                       jobjectArray tokenValueArray,
                                       jstring data
) {
    auto strTokenFrom = jstring2stdString(env, tokenFrom);
    auto strTokenTo = jstring2stdString(env, tokenTo);
    auto strData = jstring2stdString(env, data);

    std::vector<JUB_CHAR_CPTR> tokenIDs;
    std::vector<JUB_CHAR_CPTR> tokenValues;

    int arraySize = env->GetArrayLength(tokenIDArray);
    for (int i = 0; i < arraySize; i++) {
        jstring tokenID = (jstring) env->GetObjectArrayElement(tokenIDArray, i);
        jstring tokenValue = (jstring) env->GetObjectArrayElement(tokenValueArray, i);

        auto strTokenID = jstring2stdString(env, tokenID);
        auto strTokenValue = jstring2stdString(env, tokenValue);

        tokenIDs.push_back(strTokenID.c_str());
        tokenIDs.push_back(strTokenValue.c_str());
    }

    JUB_CHAR_PTR abi = nullptr;

    JUB_RV rv = JUB_BuildERC1155BatchTransferAbiETH(contextID,
                                                    (JUB_CHAR_CPTR) strTokenFrom.c_str(),
                                                    (JUB_CHAR_CPTR) strTokenTo.c_str(),
                                                    &tokenIDs[0], arraySize,
                                                    &tokenValues[0], arraySize,
                                                    (JUB_CHAR_CPTR) strData.c_str(),
                                                    &abi);

    return buildPbRvString("JUB_BuildERC1155BatchTransferAbiETH", env, rv, abi);
}


JNINativeMethod ethNativeMethods[] = {
        {
                "nativeETHCreateContext",
                "([BI)[B",
                (void *) native_CreateContextETH
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
                "nativeETHSetERC20Token",
                "(ILjava/lang/String;ILjava/lang/String;)I",
                (void *) native_SetERC20TokenETH
        },
        {
                "nativeETHBuildERC20TransferAbi",
                "(ILjava/lang/String;Ljava/lang/String;)[B",
                (void *) native_BuildERC20TransferAbiETH
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
            "nativeETHSetERC721Token",
            "(ILjava/lang/String;Ljava/lang/String;)I",
            (void *) native_SetERC721TokenETH
        },
        {
            "nativeETHBuildERC721TransferAbi",
            "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)[B",
            (void *) native_BuildERC721TransferAbiETH
        },
        {
            "nativeETHBuildERC1155TransferAbi",
            "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)[B",
            (void *) native_BuildERC1155TransferAbiETH
        },
        {
            "nativeETHBuildERC1155BatchTransferAbi",
            "(ILjava/lang/String;Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)[B",
            (void *) native_BuildERC1155BatchTransferAbiETH
        },
};


#define ETH_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getEthClass(JNIEnv *env) {
    return env->FindClass(ETH_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getEthNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(ethNativeMethods) / sizeof(ethNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(ethNativeMethods[i]);
    }
    return methodList;
}