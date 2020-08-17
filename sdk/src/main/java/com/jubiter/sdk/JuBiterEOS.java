package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EOSProtos;
import com.jubiter.sdk.proto.EthereumProtos;

/**
 * @author jifeng
 * @date 2020/7/22
 * @time 09:22
 */
public final class JuBiterEOS {

    /**
     * 创建硬件钱包上下文
     *
     * @param config   上下文配置信息
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext(CommonProtos.ContextCfg config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeEOSCreateContext(config.toByteArray(), deviceID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 创建软件钱包上下文
     *
     * @param config  上下文配置信息
     * @param xPrikey 待使用钱包对应的主私钥
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext_Software(CommonProtos.ContextCfg config, String xPrikey) {
        try {
            byte[] result = NativeApi.nativeEOSCreateContext_Software(config.toByteArray(), xPrikey);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取主公钥
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param format    主公钥的显示格式，HEX or XPUB
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getMainHDNode(int contextID, CommonProtos.ENUM_PUB_FORMAT format) {
        try {
            byte[] result = NativeApi.nativeEOSGetMainHDNode(contextID, format.toString().getBytes());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取 bip32 指定路径的公钥
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param format    公钥的显示格式，HEX or XPUB
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getHDNode(int contextID, CommonProtos.ENUM_PUB_FORMAT format,
                                                      CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeEOSGetHDNode(contextID, format.toString().getBytes(), bip44.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取指定 bip32 地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @param isShow    是否在硬件屏幕显示地址；true：显示，false：不显示
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getAddress(int contextID, CommonProtos.Bip44Path bip44, boolean isShow) {
        try {
            byte[] result = NativeApi.nativeEOSGetAddress(contextID, bip44.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置EOS快捷收款地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString setAddress(int contextID,
                                                       CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeEOSSetAddress(contextID, bip44.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 构建action
     *
     * @param contextID       上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param actionListEOS    action列表
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildAction(int contextID,
                                                          EOSProtos.ActionListEOS actionListEOS) {
        try {
            byte[] result = NativeApi.nativeEOSBuildAction(contextID,actionListEOS.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * EOS 交易签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param txInfo
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signTransaction(int contextID, EOSProtos.TransactionEOS txInfo) {
        try {
            byte[] result = NativeApi.nativeEOSSignTransaction(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }
}
