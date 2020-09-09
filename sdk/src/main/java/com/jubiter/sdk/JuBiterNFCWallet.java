package com.jubiter.sdk;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.proto.CommonProtos;

/**
 * NFC
 * @author fengshuo
 * @date 2020/9/8
 * @time 14:20
 */
public class JuBiterNFCWallet {

    /**
     * 初始化 NFC
     * @return
     */
    public static int nfcInitDevice() {
        return NativeApi.nativeNFCInitDevice();
    }

    /**
     * NFC 连接
     * @param deviceUUID
     * @return
     */
    public static CommonProtos.ResultInt nfcConnectDevice(String deviceUUID) {
        try {
            byte[] result =  NativeApi.nativeNFCConnectDevice(deviceUUID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * NFC 断开连接
     * @param deviceID
     * @return
     */
    public static int nfcDisconnectDevice(int deviceID) {
        return NativeApi.nativeNFCDisconnectDevice(deviceID);
    }

    /**
     * NFC 是否连接
     * @param deviceID
     * @return
     */
    public static int nfcIsConnect(int deviceID) {
        return NativeApi.nativeNFCIsConnect(deviceID);
    }

    /**
     * 重置  NFC
     * @param deviceID
     * @return
     */
    public static int nfcReset(int deviceID) {
        return NativeApi.nativeNFCReset(deviceID);
    }

    /**
     * NFC 生成种子
     * @param deviceID
     * @param PIN
     * @param curve
     * @return
     */
    public static int nfcGenerateSeed(int deviceID, String PIN, CommonProtos.CURVES curve) {
        return NativeApi.nativeNFCGenerateSeed(deviceID, PIN, curve.toString().getBytes());
    }

    /**
     * NFC 导入助记词
     * @param deviceID
     * @param PIN
     * @param mnemonic
     * @return
     */
    public static int nfcImportMnemonic(int deviceID, String PIN, String mnemonic) {
        return NativeApi.nativeNFCImportMnemonic(deviceID, PIN, mnemonic);
    }

    /**
     * NFC 导出助记词
     * @param deviceID
     * @param PIN
     * @param mnemonic
     * @return
     */
    public static byte[] nfcExportMnemonic(int deviceID, String PIN, String mnemonic) {
        return NativeApi.nativeNFCExportMnemonic(deviceID, PIN, mnemonic);
    }

    /**
     * NFC 修改密码
     * @param deviceID
     * @param originPIN
     * @param newPIN
     * @return
     */
    public static int nfcChangePIN(int deviceID, String originPIN, String newPIN) {
        return  NativeApi.nativeNFCChangePIN(deviceID, originPIN, newPIN);
    }


}
