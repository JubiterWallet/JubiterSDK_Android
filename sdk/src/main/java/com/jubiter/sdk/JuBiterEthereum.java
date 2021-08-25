package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EthereumProtos;

/**
 * @author fengshuo
 * @date 2019/9/24
 * @time 17:22
 */
public final class JuBiterEthereum {

    /**
     * 创建硬件钱包上下文
     *
     * @param config   上下文配置信息
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultInt createContext(EthereumProtos.ContextCfgETH config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeETHCreateContext(config.toByteArray(), deviceID);
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
            byte[] result = NativeApi.nativeETHGetMainHDNode(contextID, format.toString().getBytes());
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
            byte[] result = NativeApi.nativeETHGetHDNode(contextID, format.toString().getBytes(), bip44.toByteArray());
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
            byte[] result = NativeApi.nativeETHGetAddress(contextID, bip44.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置ETH快捷收款地址
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44     符合bip44格式的分层路径
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString setAddress(int contextID,
                                                       CommonProtos.Bip44Path bip44) {
        try {
            byte[] result = NativeApi.nativeETHSetAddress(contextID, bip44.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 设置 ERC20 token
     *
     * @param contextID       上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenName       币种名称
     * @param unitDP          允许的小数点后位数
     * @param contractAddress 合约地址
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static int setERC20Token(int contextID,
                                    String tokenName,
                                    int unitDP,
                                    String contractAddress) {
        return NativeApi.nativeETHSetERC20Token(contextID, tokenName, unitDP, contractAddress);
    }

    /**
     * 构建ERC20交易体
     *
     * @param contextID       上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param address         接收地址
     * @param amountInWei     转账金额，以 wei 为单位
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildERC20TransferAbi(int contextID,
                                                                  String address,
                                                                  String amountInWei) {
        try {
            byte[] result = NativeApi.nativeETHBuildERC20TransferAbi(contextID, address, amountInWei);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 构建 ERC721 交易体
     *
     * @param contextID       上下文 ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenName       币种名称
     * @param contractAddress 合约地址
     * @return 若 stateCode 为 0, 则表示执行成功，value 即为执行结果，否则表示执行失败
     */
    public static int setERC721Token(int contextID,
                                     String tokenName,
                                     String contractAddress) {
        return NativeApi.nativeETHSetERC721Token(contextID, tokenName, contractAddress);
    }

    /**
     * 构建 ERC721 交易体
     *
     * @param contextID       上下文 ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenFrom       转出地址
     * @param tokenTo         接收地址
     * @param tokenID         资源 ID
     * @return 若 stateCode 为 0, 则表示执行成功，value 即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildERC721TransferAbi(int contextID,
                                                                   String tokenFrom,
                                                                   String tokenTo,
                                                                   String tokenID) {
        try {
            byte[] result = NativeApi.nativeETHBuildERC721TransferAbi(
                    contextID,
                    tokenFrom,
                    tokenTo,
                    tokenID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 构建 ERC1155 交易体
     *
     * @param contextID       上下文 ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenFrom       转出地址
     * @param tokenTo         接收地址
     * @param tokenID         资源 ID
     * @param tokenValue      资源值
     * @param data            个人 data
     * @return 若 stateCode 为 0, 则表示执行成功，value 即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildERC1155TransferAbi(int contextID,
                                                                    String tokenFrom,
                                                                    String tokenTo,
                                                                    String tokenID,
                                                                    String tokenValue,
                                                                    String data) {
        try {
            byte[] result = NativeApi.nativeETHBuildERC1155TransferAbi(
                    contextID,
                    tokenFrom,
                    tokenTo,
                    tokenID,
                    tokenValue,
                    data);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * 构建 ERC1155 交易体
     *
     * @param contextID       上下文 ID，该值由 createContext_Software 或 createContext 方法返回
     * @param tokenFrom       转出地址
     * @param tokenTo         接收地址
     * @param tokenIDArray    资源 ID 数组。长度必须与 tokenValueArray 一致。
     * @param tokenValueArray 资源值数组。长度必须与 tokenIDArray 一致。
     * @param data            个人 data
     * @return 若 stateCode 为 0, 则表示执行成功，value 即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString buildERC1155BatchTransferAbi(int contextID,
                                                                         String tokenFrom,
                                                                         String tokenTo,
                                                                         String[] tokenIDArray,
                                                                         String[] tokenValueArray,
                                                                         String data) {
        try {
            byte[] result = NativeApi.nativeETHBuildERC1155BatchTransferAbi(
                    contextID,
                    tokenFrom,
                    tokenTo,
                    tokenIDArray,
                    tokenValueArray,
                    data);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * ETH 交易签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param txInfo
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signTransaction(int contextID, EthereumProtos.TransactionETH txInfo) {
        try {
            byte[] result = NativeApi.nativeETHSignTransaction(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param bip44
     * @param data
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signBytestring(int contextID, CommonProtos.Bip44Path bip44, String data) {
        try {
            byte[] result = NativeApi.nativeETHSignBytestring(contextID, bip44.toByteArray(),data);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * ETH 协议签名
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param txInfo
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString signContract(int contextID, EthereumProtos.TransactionETH txInfo) {
        try {
            byte[] result = NativeApi.nativeETHSignContract(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }
}
