package com.jubiter.sdk.jni;

import com.jubiter.sdk.jni.nfc.NFCInitParam;

/**
 * @author fengshuo
 * @date 2020/9/16
 * @time 11:24
 */
public class NfcNativeApi extends NativeApi {

    public static native int nativeNFCInitDevice(NFCInitParam param);

    public static native int nativeNFCSetParam(int deviceID);

    public static native byte[] nativeNFCConnectDevice(String deviceUUID);

    public static native int nativeNFCDisconnectDevice(int deviceID);

    public static native int nativeNFCIsConnect(int deviceID);

    public static native int nativeNFCReset(int deviceID);

    public static native int nativeNFCGenerateSeed(int deviceID, String PIN, byte[] curve);

    public static native int nativeNFCImportMnemonic(int deviceID, String PIN, String mnemonic);

    public static native byte[] nativeNFCExportMnemonic(int deviceID, String PIN);

    public static native byte[] nativeNFCChangePIN(int deviceID, String originPIN, String newPIN);

    public static native byte[] nativeNFCHasRootKey(int deviceID);

    public static native int nativeNFCSetLabel(int deviceID, String label);

    public static native int nativeNFCSetAlertMessage(int deviceID, String msg);
}
