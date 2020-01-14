package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.BitcoinProtos;
import com.jubiter.sdk.proto.CommonProtos;

/**
 * @author fengshuo
 * @date 2019/9/24
 * @time 17:22
 */
public final class JuBiterBitcoin {

    /**
     * 创建软件钱包上下文
     *
     * @param config  上下文配置信息
     * @param xPrikey 待使用钱包对应的主私钥
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext_Software(BitcoinProtos.ContextCfgBTC config, String xPrikey) {
        try {
            byte[] result = NativeApi.nativeBTCCreateContext_Software(config.toByteArray(), xPrikey);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 创建硬件钱包上下文
     *
     * @param config   上下文配置信息
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext(BitcoinProtos.ContextCfgBTC config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeBTCCreateContext(config.toByteArray(), deviceID);
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
            byte[] result = NativeApi.nativeBTCGetMainHDNode(contextID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取 bip44 指定路径的公钥
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getHDNode(int contextID, CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeBTCGetHDNode(contextID, bip44.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取指定 bip44 地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @param isShow    是否在硬件屏幕显示地址；true：显示，false：不显示
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getAddress(int contextID, CommonProtos.Bip44Path bip44, boolean isShow) {
        try {
            byte[] result = NativeApi.nativeBTCGetAddress(contextID, bip44.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置快捷收款地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @return 若 stateCode 为0,则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString setAddress(int contextID,
                                                       CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeBTCSetAddress(contextID, bip44.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * BTC 交易签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param txInfo    交易信息
     * @return 交易结果。若 stateCode 为0,则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signTransaction(int contextID, BitcoinProtos.TransactionBTC txInfo) {
        try {
            byte[] result = NativeApi.nativeBTCSignTransaction(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置硬件显示的BTC单位
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param uintType
     * @return 若 stateCode 为0,则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static int setUint(int contextID, BitcoinProtos.BTC_UNIT_TYPE uintType) {
        return NativeApi.nativeBTCSetUnit(contextID, uintType.toString().getBytes());
    }

    /**
     * 构建USDT交易输出
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param usdtTo    USDT 接收地址
     * @param amount    USDT 转账金额
     * @return 若 stateCode 为0,则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultAny buildUSDTOutput(int contextID, String usdtTo, long amount) {
        try {
            byte[] result = NativeApi.nativeBuildUSDTOutput(contextID, usdtTo, amount);
            return CommonProtos.ResultAny.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }
}
