package com.jubiter.sdk.jni;

public class SwiNativeApi extends NativeApi {

    public static native byte[] nativeSWIConnectDevice();

    public static native int nativeSWIDisconnectDevice(int deviceID);

    public static native int nativeSWIBuildFromMasterPrivateKey(int deviceID, byte[] curve, String masterPrivateKey);

    public static native int nativeSWIBuildFromMnemonic(int deviceID, String passphrase, String mnemonic);

}
