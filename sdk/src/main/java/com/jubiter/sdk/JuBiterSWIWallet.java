package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NfcNativeApi;
import com.jubiter.sdk.jni.SwiNativeApi;
import com.jubiter.sdk.jni.nfc.NFCInitParam;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.CommonProtos.ResultAny;
import com.jubiter.sdk.proto.CommonProtos.ResultInt;
import com.jubiter.sdk.proto.CommonProtos.ResultString;

/**
 * SWI 钱包操作
 *
 */
public class JuBiterSWIWallet {

    /**
     * SWI 连接
     *
     * @return
     */
    public static ResultInt swiConnectDevice() {
        try {
            byte[] result = SwiNativeApi.nativeSWIConnectDevice();
            return ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * SWI 断开连接
     *
     * @param deviceID
     * @return
     */
    public static int swiDisconnectDevice(int deviceID) {
        return SwiNativeApi.nativeSWIDisconnectDevice(deviceID);
    }

    /**
     * BuildFromMasterPrivateKey
     *
     * @param deviceID
     * @param masterPrivateKey
     * @param curve
     * @return
     */
    public static int swiBuildFromMasterPrivateKey(int deviceID, CommonProtos.CURVES curve, String masterPrivateKey) {
        return SwiNativeApi.nativeSWIBuildFromMasterPrivateKey(deviceID, curve.toString().getBytes(),masterPrivateKey);
    }

    /**
     * BuildFromMnemonic
     *
     * @param deviceID
     * @param mnemonic
     * @param passphrase
     * @return
     */
    public static int swiBuildFromMnemonic(int deviceID, String mnemonic, String passphrase) {
        return SwiNativeApi.nativeSWIBuildFromMnemonic(deviceID, passphrase, mnemonic);
    }
}
