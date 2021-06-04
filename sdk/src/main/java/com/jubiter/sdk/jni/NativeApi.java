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


    //********************************* 硬件钱包 ************************************

    public static native byte[] nativeGetDeviceInfo(int deviceID);

    public static native byte[] nativeGetDeviceCert(int deviceID);

    public static native byte[] nativeSendAPDU(int deviceID, String apdu);

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

    public static native byte[] nativeBuildQRC20Output(int contextID, String contractAddr, int decimal,
                                                       String symbol, long gasLimit, long gasPrice,
                                                       String to, String amount);

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

    public static native byte[] nativeETHBuildERC721Abi(int contextID,
                                                        String tokenName,
                                                        String contractAddress,
                                                        String tokenFrom,
                                                        String tokenTo,
                                                        String tokenID);

    public static native byte[] nativeETHSignTransaction(int contextID, byte[] txInfo);

    public static native byte[] nativeETHSignBytestring(int contextID, byte[] bip32, String data);

    public static native byte[] nativeETHSignContract(int contextID, byte[] txInfo);

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


    //********************************* SDK XRP ************************************

    public static native byte[] nativeXRPCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeXRPCreateContext_Software(byte[] config, String xPriKey);

    public static native byte[] nativeXRPGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeXRPGetHDNode(int contextID, byte[] format, byte[] bip32);

    public static native byte[] nativeXRPGetMainHDNode(int contextID, byte[] format);

    public static native byte[] nativeXRPSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeXRPSignTransaction(int contextID, byte[] bip32, byte[] txInfo);

    public static native byte[] nativeXRPCheckAddress(int contextID, String address);

    //********************************* SDK TRX ************************************

    public static native byte[] nativeTRXCreateContext(byte[] config, int deviceID);

    public static native byte[] nativeTRXCreateContext_Software(byte[] config, String xPriKey);

    public static native byte[] nativeTRXGetAddress(int contextID, byte[] bip32, boolean isShow);

    public static native byte[] nativeTRXCheckAddress(int contextID, String address);

    public static native byte[] nativeTRXGetHDNode(int contextID, byte[] format, byte[] bip32);

    public static native byte[] nativeTRXGetMainHDNode(int contextID, byte[] format);

    public static native byte[] nativeTRXSetAddress(int contextID, byte[] bip32);

    public static native byte[] nativeTRXSignTransaction(int contextID, byte[] bip32,
                                                         String jPackedContractInPb);

    public static native int nativeTRXSetTRC10Asset(int contextID, byte[] assetName,
                                                    int unitDP, byte[] assetId);

    public static native byte[] nativeTRXBuildTRC20Abi(int contextID, String tokenName,
                                                       int unitDP,
                                                       String contractAddress,
                                                       String address,
                                                       String amount);

    public static native byte[] nativeTRXBuildTRC721Abi(int contextID,
                                                        String tokenName,
                                                        String contractAddress,
                                                        String tokenFrom,
                                                        String tokenTo,
                                                        String tokenID);

    public static native byte[] nativeTRXPackContract(int contextID, byte[] tx);

}
