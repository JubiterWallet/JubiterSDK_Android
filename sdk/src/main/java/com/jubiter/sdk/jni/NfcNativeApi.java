package com.jubiter.sdk.jni;

import com.jubiter.sdk.jni.nfc.NFCInitParam;

/**
 * @author fengshuo
 * @date 2020/9/16
 * @time 11:24
 */
public class NfcNativeApi extends NativeApi {

    public static native int nativeNFCInitDevice(NFCInitParam param);

    public static native byte[] nativeNFCConnectDevice(String deviceUUID);

    public static native int nativeNFCDisconnectDevice(int deviceID);

    public static native int nativeNFCIsConnect(int deviceID);

    public static native int nativeNFCReset(int deviceID);

    public static native int nativeNFCGenerateSeed(int deviceID, String PIN, byte[] curve);

    public static native int nativeNFCImportMnemonic(int deviceID, String PIN, String mnemonic);

    public static native byte[] nativeNFCExportMnemonic(int deviceID, String PIN, String mnemonic);

    public static native int nativeNFCChangePIN(int deviceID, String originPIN, String newPIN);
}
