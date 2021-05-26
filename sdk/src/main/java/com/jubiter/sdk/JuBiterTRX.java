package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.CommonProtos;

/**
 * @author jifeng
 * @date 2021/03/17
 * @time 10:46
 */
public final class JuBiterTRX {

    /**
     * 创建硬件钱包上下文
     *
     * @param config   上下文配置信息
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext(CommonProtos.ContextCfg config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeTRXCreateContext(config.toByteArray(), deviceID);
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
            byte[] result = NativeApi.nativeTRXCreateContext_Software(config.toByteArray(), xPrikey);
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
            byte[] result = NativeApi.nativeTRXGetMainHDNode(contextID, format.toString().getBytes());
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
            byte[] result = NativeApi.nativeTRXGetHDNode(contextID, format.toString().getBytes(), bip44.toByteArray());
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
            byte[] result = NativeApi.nativeTRXGetAddress(contextID, bip44.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置TRX快捷收款地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString setAddress(int contextID,
                                                       CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeTRXSetAddress(contextID, bip44.toByteArray());
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
    public static CommonProtos.ResultString checkAddress(int contextID, String address) {
        try {
            byte[] result = NativeApi.nativeTRXCheckAddress(contextID, address);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * TRX 交易签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param packedContractInPb
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signTransaction(int contextID, CommonProtos.Bip44Path bip44,
                                                            String packedContractInPb) {
        try {
            byte[] result = NativeApi.nativeTRXSignTransaction(contextID, bip44.toByteArray(), packedContractInPb);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 构建TRC20交易体
     *
     * @param contextID       上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenName       token名称
     * @param unitDP          允许的小数点后位数
     * @param contractAddress 合约地址
     * @param address         接收地址
     * @param amount     转账金额，
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildTRC20Abi(int contextID, String tokenName,
                                                          int unitDP,
                                                          String contractAddress,
                                                          String address,
                                                          String amount) {
        try {
            byte[] result = NativeApi.nativeTRXBuildTRC20Abi(contextID, tokenName, unitDP,
                    contractAddress, address, amount);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置TRC10 Asset
     *
     * @param contextID       上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param assetName       asset名称
     * @param unitDP          允许的小数点后位数
     * @param assetId asset ID
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static int SetTRC10Asset(int contextID, byte[] assetName,
                                                          int unitDP, byte[] assetId) {
        return NativeApi.nativeTRXSetTRC10Asset(contextID, assetName, unitDP,
                assetId);
    }

    /**
     * TRX 打包协议
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param transaction
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString packContract(int contextID, org.tron.protos.Protocol.Transaction transaction) {
        try {
            byte[] result = NativeApi.nativeTRXPackContract(contextID, transaction.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

}
