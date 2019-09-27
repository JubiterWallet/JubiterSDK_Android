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
     *
     * @param config
     * @param xPrikey
     * @return
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
     *
     * @param config
     * @param deviceID
     * @return
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
     *
     * @param contextID
     * @return
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
     *
     * @param contextID
     * @param bip32
     * @return
     */
    public static CommonProtos.ResultString getHDNode(int contextID, CommonProtos.Bip32Path bip32) {
        try {
            byte[] result = NativeApi.nativeBTCGetHDNode(contextID, bip32.toByteArray());
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
            byte[] result = NativeApi.nativeBTCGetAddress(contextID, bip32.toByteArray(), isShow);
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
            byte[] result = NativeApi.nativeBTCSetAddress(contextID, bip32.toByteArray());
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
     *
     * @param contextID
     * @param uintType
     * @return
     */
    public static int setUint(int contextID, BitcoinProtos.BTC_UNIT_TYPE uintType) {
        return NativeApi.nativeBTCSetUnit(contextID, uintType.toString().getBytes());
    }

    /**
     * 构建USDT交易输出
     *
     * @param contextID
     * @param usdtTo
     * @param amount
     * @return
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
