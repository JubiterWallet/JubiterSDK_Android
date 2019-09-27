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

    public static native int nativeInitDevice();

    public static native int nativeStartScan(InnerScanCallback scanCallback);

    public static native int nativeStopScan();

    public static native int nativeConnectDevice(String address, int[] deviceHandle, int timeout, InnerDiscCallback discCallback);

    public static native int nativeCancelConnect(String address);

    public static native int nativeDisconnectDevice(int deviceHandle);

    public static native int nativeIsConnected(int deviceHandle);

    //********************************* 硬件钱包 ************************************

    public static native byte[] nativeGetDeviceInfo(int deviceID);

    public static native byte[] nativeGetDeviceCert(int deviceID);

    public static native byte[] nativeSendApdu(int deviceID, String apdu);

    public static native boolean nativeIsInitialize(int deviceID);

    public static native boolean nativeIsBootLoader(int deviceID);

    /**
     *
     * @param contextID
     * @param timeout
     * @return
     */
    public static native byte[] nativeSetTimeout(int contextID, int timeout);

    public static native byte[] nativeEnumApplets(int deviceID);

    public static native byte[] nativeEnumSupportCoins(int deviceID);

    public static native byte[] nativeGetAppletVersion(int deviceID, String appID);

    public static native byte[] nativeQuerryBattery(int deviceID);

    //********************************* SDK Common ************************************

    public static native int nativeClearContext(int contextID);

    public static native int nativeShowVirtualPWD(int contextID);

    public static native int nativeCancelVirtualPWD(int contextID);

    public static native byte[] nativeVerifyPIN(int contextID, String PIN);

    //********************************* SDK BTC ************************************

    public static native byte[] nativeBTCCreateContext_Software(byte[] config, String xPrikey);

    public static native byte[] nativeBTCCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeBTCGetMainHDNode(int contextID);

    public static native byte[] nativeBTCGetHDNode(int contextID, byte[] bip32);

    public static native byte[] nativeBTCGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeBTCSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeBTCSignTransaction(int contextID, byte[] txArray);

    public static native byte[] nativeBTCSetUnit(int contextID, byte[] uint);

    public static native byte[] nativeBuildUSDTOutput(int contextID, String usdtTo, long amount);

    //********************************* SDK ETH ************************************

    public static native byte[] nativeETHCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeETHCreateContext_Software(byte[] config, String xPrikey);

    public static native byte[] nativeETHGetMainHDNode(int contextID, byte[] format);

    public static native byte[] nativeETHGetHDNode(int contextID, byte[] format, byte[] bip32);

    public static native byte[] nativeETHGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeETHSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeETHBuildERC20Abi(int contextID, String address, String amountInWei);

    public static native byte[] nativeETHSignTransaction(int contextID, byte[] txInfo);

}
