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

    public static CommonProtos.ResultInt createContext_Software(BitcoinProtos.ContextCfgBTC config, String xPrikey) {
        try {
            byte[] result = NativeApi.nativeBTCCreateContext_Software(config.toByteArray(), xPrikey);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultInt createContext(BitcoinProtos.ContextCfgBTC config, int deviceID) {
        try {
            byte[] result = NativeApi.nativeBTCCreateContext(config.toByteArray(), deviceID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString getMainHDNode(int contextID) {
        try {
            byte[] result = NativeApi.nativeBTCGetMainHDNode(contextID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString getHDNode(int contextID, CommonProtos.Bip32Path bip32) {
        try {
            byte[] result = NativeApi.nativeBTCGetHDNode(contextID, bip32.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString getAddress(int contextID, CommonProtos.Bip32Path bip32, boolean isShow) {
        try {
            byte[] result = NativeApi.nativeBTCGetAddress(contextID, bip32.toByteArray(), isShow);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString setAddress(int contextID, CommonProtos.Bip32Path bip32) {
        try {
            byte[] result = NativeApi.nativeBTCSetAddress(contextID, bip32.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString signTransaction(int contextID, BitcoinProtos.TransactionBTC txInfo) {
        try {
            byte[] result = NativeApi.nativeBTCSignTransaction(contextID, txInfo.toByteArray());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString setUint(int contextID, BitcoinProtos.BTC_UNIT_TYPE uintType) {
        try {
            byte[] result = NativeApi.nativeBTCSetUnit(contextID, uintType.toString().getBytes());
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static CommonProtos.ResultString buildUSDTOutput(int contextID, String usdtTo, long amount) {
        try {
            byte[] result = NativeApi.nativeBuildUSDTOutput(contextID, usdtTo, amount);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }
}
