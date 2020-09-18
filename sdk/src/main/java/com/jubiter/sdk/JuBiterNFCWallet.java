package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NfcNativeApi;
import com.jubiter.sdk.jni.nfc.NFCInitParam;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.CommonProtos.ResultInt;
import com.jubiter.sdk.proto.CommonProtos.ResultString;

/**
 * NFC
 *
 * @author fengshuo
 * @date 2020/9/8
 * @time 14:20
 */
public class JuBiterNFCWallet {

    /**
     * 初始化 NFC
     *
     * @return
     */
    public static int nfcInitDevice(NFCInitParam param) {
        return NfcNativeApi.nativeNFCInitDevice(param);
    }

    /**
     * NFC 连接
     *
     * @param deviceUUID
     * @return
     */
    public static ResultInt nfcConnectDevice(String deviceUUID) {
        try {
            byte[] result = NfcNativeApi.nativeNFCConnectDevice(deviceUUID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * NFC 断开连接
     *
     * @param deviceID
     * @return
     */
    public static int nfcDisconnectDevice(int deviceID) {
        return NfcNativeApi.nativeNFCDisconnectDevice(deviceID);
    }

    /**
     * NFC 是否连接
     *
     * @param deviceID
     * @return
     */
    public static int nfcIsConnect(int deviceID) {
        return NfcNativeApi.nativeNFCIsConnect(deviceID);
    }

    /**
     * 重置  NFC
     *
     * @param deviceID
     * @return
     */
    public static int nfcReset(int deviceID) {
        return NfcNativeApi.nativeNFCReset(deviceID);
    }

    /**
     * NFC 生成种子
     *
     * @param deviceID
     * @param PIN
     * @param curve
     * @return
     */
    public static int nfcGenerateSeed(int deviceID, String PIN, CommonProtos.CURVES curve) {
        return NfcNativeApi.nativeNFCGenerateSeed(deviceID, PIN, curve.toString().getBytes());
    }

    /**
     * NFC 导入助记词
     *
     * @param deviceID
     * @param PIN
     * @param mnemonic
     * @return
     */
    public static int nfcImportMnemonic(int deviceID, String PIN, String mnemonic) {
        return NfcNativeApi.nativeNFCImportMnemonic(deviceID, PIN, mnemonic);
    }

    /**
     * NFC 导出助记词
     *
     * @param deviceID
     * @param PIN
     * @return
     */
    public static ResultString nfcExportMnemonic(int deviceID, String PIN) {
        try {
            byte[] result = NfcNativeApi.nativeNFCExportMnemonic(deviceID, PIN);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * NFC 修改密码
     *
     * @param deviceID
     * @param originPIN
     * @param newPIN
     * @return
     */
    public static ResultInt nfcChangePIN(int deviceID, String originPIN, String newPIN) {
        try {
            byte[] result = NfcNativeApi.nativeNFCChangePIN(deviceID, originPIN, newPIN);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

}
