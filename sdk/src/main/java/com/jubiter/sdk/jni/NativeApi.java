package com.jubiter.sdk.jni;

/**
 * @author fengshuo
 * @date 2019/9/23
 * @time 11:27
 */
public class NativeApi {

    static {
        System.loadLibrary("coreNDK");
    }

    //********************************* 软件钱包 ************************************

    public static native byte[] nativeGenerateMnemonic(byte[] param);

    public static native int nativeCheckMnemonic(String param);

    public static native byte[] nativeGenerateSeed(String mnemonic, String passphrase);

    public static native byte[] nativeSeedToMasterPrivateKey(String seed, byte[] curve);

    //********************************* 蓝牙 ************************************

    public static native int nativeBLEInitDevice();

    public static native int nativeBLEStartScan(InnerScanCallback scanCallback);

    public static native int nativeBLEStopScan();

    public static native int nativeBLEConnectDevice(String deviceName,
                                                    String deviceMAC,
                                                    int[] deviceHandle,
                                                    int timeout,
                                                    InnerDiscCallback discCallback);

    public static native int nativeBLECancelConnect(String deviceName, String deviceMAC);

    public static native int nativeBLEDisconnectDevice(int deviceHandle);

    public static native int nativeBLEIsConnected(int deviceHandle);

    //********************************* 硬件钱包 ************************************

    public static native byte[] nativeGetDeviceInfo(int deviceID);

    public static native byte[] nativeGetDeviceCert(int deviceID);

    public static native byte[] nativeSendAPDU(int deviceID, String apdu);

    public static native boolean nativeIsInitialize(int deviceID);

    public static native boolean nativeIsBootLoader(int deviceID);

    /**
     * @param contextID
     * @param timeout
     * @return
     */
    public static native int nativeSetTimeout(int contextID, int timeout);

    public static native byte[] nativeEnumApplets(int deviceID);

    public static native byte[] nativeEnumSupportCoins(int deviceID);

    public static native byte[] nativeGetAppletVersion(int deviceID, String appID);

    public static native byte[] nativeQueryBattery(int deviceID);

    //********************************* SDK Common ************************************

    public static native int nativeClearContext(int contextID);

    public static native int nativeShowVirtualPWD(int contextID);

    public static native int nativeCancelVirtualPWD(int contextID);

    public static native byte[] nativeVerifyPIN(int contextID, String PIN);

    //********************************* SDK BTC ************************************

    public static native byte[] nativeBTCCreateContext_Software(byte[] config, String xPriKey);

    public static native byte[] nativeBTCCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeBTCGetMainHDNode(int contextID);

    public static native byte[] nativeBTCGetHDNode(int contextID, byte[] bip32);

    public static native byte[] nativeBTCGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeBTCSetAddress(int contextID, byte[] bip32);

    public static native int nativeBTCCheckAddress(int contextID, String address);

    public static native byte[] nativeBTCSignTransaction(int contextID, byte[] txArray);

    public static native int nativeBTCSetUnit(int contextID, byte[] unit);

    public static native byte[] nativeBuildUSDTOutput(int contextID, String usdtTo, long amount);

    //********************************* SDK ETH ************************************

    public static native byte[] nativeETHCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeETHCreateContext_Software(byte[] config, String xPriKey);

    public static native byte[] nativeETHGetMainHDNode(int contextID, byte[] format);

    public static native byte[] nativeETHGetHDNode(int contextID, byte[] format, byte[] bip32);

    public static native byte[] nativeETHGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeETHSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeETHBuildERC20Abi(int contextID,
                                                       String tokenName,
                                                       int unitDP,
                                                       String contractAddress,
                                                       String address,
                                                       String amountInWei);

    public static native byte[] nativeETHSignTransaction(int contextID, byte[] txInfo);

    //********************************* SDK EOS ************************************

    public static native byte[] nativeEOSCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeEOSCreateContext_Software(byte[] config, String xPriKey);

    public static native byte[] nativeEOSGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeEOSGetHDNode(int contextID, byte[] format, byte[] bip32);

    public static native byte[] nativeEOSGetMainHDNode(int contextID, byte[] format);

    public static native byte[] nativeEOSSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeEOSSignTransaction(int contextID, byte[] txInfo);

    public static native byte[] nativeEOSBuildAction(int contextID, byte[] txInfo);

    public static native byte[] nativeEOSCalculateMemoHash(String memo);

    //********************************* SDK NFC ************************************

    public static native int nativeNFCInitDevice(NFCInitParam param);

    public static native byte[] nativeNFCConnectDevice(String deviceUUID);

    public static native int nativeNFCDisconnectDevice(int deviceID);

    public static native int nativeNFCIsConnect(int deviceID);

    public static native int nativeNFCReset(int deviceID);

    public static native int nativeNFCGenerateSeed(int deviceID, String PIN, byte[] curve);

    public static native int nativeNFCImportMnemonic(int deviceID, String PIN, String mnemonic);

    public static native byte[] nativeNFCExportMnemonic(int deviceID, String PIN, String mnemonic);

    public static native int nativeNFCChangePIN(int deviceID, String originPIN, String newPIN);

    //***************************************************************************

}
