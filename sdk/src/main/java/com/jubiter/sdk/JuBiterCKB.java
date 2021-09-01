package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.NervosCKBProtos;

/**
 * @author jifeng
 * @date 2021/8/31
 * @time 09:58
 */
public final class JuBiterCKB {

    /**
     * 创建硬件钱包上下文
     *
     * @param config   上下文配置信息
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext(NervosCKBProtos.ContextCfgCKB config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeCKBCreateContext(config.toByteArray(), deviceID);
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
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getMainHDNode(int contextID) {
        try {
            byte[] result = NativeApi.nativeCKBGetMainHDNode(contextID);
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
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getHDNode(int contextID,
                                                      CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeCKBGetHDNode(contextID,  bip44.toByteArray());
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
    public static CommonProtos.ResultString getAddress(int contextID,
                                                       CommonProtos.Bip44Path bip44,
                                                       boolean isShow) {
        try {
            byte[] result = NativeApi.nativeCKBGetAddress(contextID, bip44.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 检测收款地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param address   待检测地址
     * @return 若 stateCode 为0,则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static int checkAddress(int contextID, String address) {
        int result = NativeApi.nativeCKBCheckAddress(contextID, address);
        return result;
    }

    /**
     * CKB 交易签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param txInfo
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signTransaction(int contextID, NervosCKBProtos.TransactionCKB txInfo) {
        try {
            byte[] result = NativeApi.nativeCKBSignTransaction(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

}
