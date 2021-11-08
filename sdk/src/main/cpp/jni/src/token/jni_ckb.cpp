
#include "jni_ckb.h"


JNIEXPORT jbyteArray JNICALL
native_CreateContextCKB(JNIEnv *env,
                        jclass clz,
                        jbyteArray jcfg,
                        jint deviceID) {
    JUB::Proto::NervosCKB::ContextCfgCKB pbCfg;
    if (parseFromJbyteArray(env, jcfg, &pbCfg)) {
        CONTEXT_CONFIG_CKB cfg;
        cfg.mainPath = (JUB_CHAR_PTR) pbCfg.main_path().c_str();
        cfg.mainNet =  (JUB_ENUM_BOOL) pbCfg.main_net();
        JUB_UINT16 contextID;
        JUB_RV rv = JUB_CreateContextCKB(cfg, deviceID, &contextID);
        return buildPbRvUInt("JUB_CreateContextCKB 1", env, rv, contextID);
    } else {
        return buildPbRvUInt("JUB_CreateContextCKB 2", env, JUBR_ARGUMENTS_BAD, 0);
    }
}

JNIEXPORT jbyteArray JNICALL
native_GetMainHDNodeCKB(JNIEnv *env,
                        jclass clz,
                        jint contextID) {

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeCKB(contextID, &xpub);
    return buildPbRvString("JUB_GetMainHDNodeCKB", env, rv, xpub);
}

JNIEXPORT jbyteArray JNICALL
native_GetHDNodeCKB(JNIEnv *env,
                    jclass clz,
                    jint contextID,
                    jbyteArray bip32) {

    BIP44_Path bip44Path;
    if (parseBip44Path(env, bip32, &bip44Path)) {
        JUB_CHAR_PTR xpub = nullptr;
        JUB_RV rv = JUB_GetHDNodeCKB(contextID, bip44Path, &xpub);
        return buildPbRvString("JUB_GetHDNodeCKB 1", env, rv, xpub);
    }
    return buildPbRvString("JUB_GetHDNodeCKB 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jbyteArray JNICALL
native_GetAddressCKB(JNIEnv *env,
                     jclass clz,
                     jint contextID,
                     jbyteArray bip32,
                     jboolean bShow) {

    BIP44_Path bip32Path;
    if (parseBip44Path(env, bip32, &bip32Path)) {
        JUB_CHAR_PTR address = nullptr;
        JUB_RV rv = JUB_GetAddressCKB(contextID, bip32Path, (JUB_ENUM_BOOL) bShow, &address);
        return buildPbRvString("JUB_GetAddressCKB 1", env, rv, address);
    }
    return buildPbRvString("JUB_GetAddressCKB 2", env, JUBR_ARGUMENTS_BAD, "");
}

JNIEXPORT jint JNICALL
native_CheckAddressCKB(JNIEnv *env,
                       jclass clz,
                       jint contextID,
                       jstring address) {
    auto strAddress = jstring2stdString(env, address);
    JUB_RV rv = JUB_CheckAddressCKB(contextID, strAddress.c_str());
    LOG_DEBUG("JUB_CheckAddressCKB rv: %d", rv);
    return rv;
}

JNIEXPORT jbyteArray JNICALL
native_SignTransactionCKB(JNIEnv *env,
                          jclass clz,
                          jint contextID,
                          jbyteArray tx) {
    JUB::Proto::NervosCKB::TransactionCKB pbTx;
    if (parseFromJbyteArray(env, tx, &pbTx)) {
        JUB_UINT32 version = pbTx.version();

        std::vector <CELL_DEP> deps;
        std::vector <CELL_INPUT> inputs;
        std::vector <CELL_OUTPUT> outputs;

        for (auto i = 0; i < pbTx.deps_size(); i++) {
            CELL_DEP dep;
            dep.type = pbTx.deps(i).type();
            dep.txHash = (JUB_CHAR_PTR) pbTx.deps(i).tx_hash().c_str();
            dep.index  = pbTx.deps(i).index();
            deps.push_back(dep);
        }

        for (int i = 0; i < pbTx.inputs_size(); i++) {
            CELL_INPUT input;
            input.preHash = (JUB_CHAR_PTR) pbTx.inputs(i).pre_hash().c_str();
            input.preIndex = pbTx.inputs(i).pre_index();

            input.since = pbTx.inputs(i).since();

            input.capacity = pbTx.inputs(i).capacity();

            input.lock.codeHash = (JUB_CHAR_PTR) pbTx.inputs(i).lock().code_hash().c_str();
            input.lock.hashType = pbTx.inputs(i).lock().hash_type();
            input.lock.args = (JUB_CHAR_PTR)pbTx.inputs(i).lock().args().c_str();

            input.path.change = (JUB_ENUM_BOOL)pbTx.inputs(i).path().change();
            input.path.addressIndex = pbTx.inputs(i).path().address_index();

            inputs.push_back(input);
        }

        for (int i = 0; i < pbTx.outputs_size(); i++) {
            CELL_OUTPUT output;
            output.capacity = pbTx.outputs(i).capacity();

            output.lock.codeHash = (JUB_CHAR_PTR) pbTx.outputs(i).lock().code_hash().c_str();
            output.lock.hashType = pbTx.outputs(i).lock().hash_type();
            output.lock.args = (JUB_CHAR_PTR) pbTx.outputs(i).lock().args().c_str();

            if(pbTx.outputs(i).has_type()) {
                output.type.codeHash = (JUB_CHAR_PTR) pbTx.outputs(i).type().code_hash().c_str();
                output.type.hashType = pbTx.outputs(i).type().hash_type();
                output.type.args = (JUB_CHAR_PTR) pbTx.outputs(i).type().args().c_str();
            }
            else {
                output.type.codeHash = nullptr;
                output.type.hashType = 0;
                output.type.args = nullptr;
            }

            output.data = (JUB_CHAR_PTR) pbTx.outputs(i).data().c_str();

            outputs.push_back(output);
        }

        JUB_CHAR_PTR raw = nullptr;
        JUB_RV rv = JUB_SignTransactionCKB(contextID,
                                           version,
                                           &deps[0], deps.size(),
                                           &inputs[0], inputs.size(),
                                           &outputs[0], outputs.size(),
                                           &raw);

        return buildPbRvString("JUB_SignTransactionCKB", env, rv, raw);
    }
    return buildPbRvString("JUB_SignTransactionCKB", env, JUBR_ARGUMENTS_BAD, "");
}


JNINativeMethod ckbNativeMethods[] = {
        {
                "nativeCKBCreateContext",
                "([BI)[B",
                (void *) native_CreateContextCKB
        },
        {
                "nativeCKBGetMainHDNode",
                "(I)[B",
                (void *) native_GetMainHDNodeCKB
        },
        {
                "nativeCKBGetHDNode",
                "(I[B)[B",
                (void *) native_GetHDNodeCKB
        },
        {
                "nativeCKBGetAddress",
                "(I[BZ)[B",
                (void *) native_GetAddressCKB
        },
        {
                "nativeCKBCheckAddress",
                "(ILjava/lang/String;)I",
                (void *) native_CheckAddressCKB
        },
        {
                "nativeCKBSignTransaction",
                "(I[B)[B",
                (void *) native_SignTransactionCKB
        },
};


#define CKB_NATIVE_CLASS "com/jubiter/sdk/jni/NativeApi"

jclass getCKBClass(JNIEnv *env) {
    return env->FindClass(CKB_NATIVE_CLASS);
}

std::vector <JNINativeMethod> getCKBNativeMethods() {
    std::vector <JNINativeMethod> methodList;
    for (int i = 0, count = sizeof(ckbNativeMethods) / sizeof(ckbNativeMethods[0]);
         i < count; ++i) {
        methodList.push_back(ckbNativeMethods[i]);
    }
    return methodList;
}