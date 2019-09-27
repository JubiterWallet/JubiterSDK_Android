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
     *
     * @param config
     * @param deviceID
     * @return
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
     *
     * @param config
     * @param xPrikey
     * @return
     */
    public static CommonProtos.ResultInt createContext_Software(EthereumProtos.ContextCfgETH config, String xPrikey) {
        try {
            byte[] result = NativeApi.nativeETHCreateContext_Software(config.toByteArray(), xPrikey);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param format
     * @return
     */
    public static CommonProtos.ResultString getMainHDNode(int contextID, EthereumProtos.ENUM_PUB_FORMAT format) {
        try {
            byte[] result = NativeApi.nativeETHGetMainHDNode(contextID, format.toString().getBytes());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param format
     * @param bip32
     * @return
     */
    public static CommonProtos.ResultString getHDNode(int contextID, EthereumProtos.ENUM_PUB_FORMAT format,
                                                      CommonProtos.Bip32Path bip32) {
        try {
            byte[] result = NativeApi.nativeETHGetHDNode(contextID, format.toString().getBytes(), bip32.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param bip32
     * @param isShow
     * @return
     */
    public static CommonProtos.ResultString getAddress(int contextID, CommonProtos.Bip32Path bip32, boolean isShow) {
        try {
            byte[] result = NativeApi.nativeETHGetAddress(contextID, bip32.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param bip32
     * @return
     */
    public static CommonProtos.ResultString setAddress(int contextID, CommonProtos.Bip32Path bip32) {
        try {
            byte[] result = NativeApi.nativeETHSetAddress(contextID, bip32.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param address
     * @param amountInWei
     * @return
     */
    public static CommonProtos.ResultString buildERC20Abi(int contextID, String address, String amountInWei) {
        try {
            byte[] result = NativeApi.nativeETHBuildERC20Abi(contextID, address, amountInWei);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID
     * @param txInfo
     * @return
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
}
