package com.jubiter.sdk.jni;

import com.jubiter.sdk.jni.ble.InnerDiscCallback;
import com.jubiter.sdk.jni.ble.InnerScanCallback;

/**
 * @author jifeng
 * @date 2021/9/10
 * @time 11:23
 */
public class BioNativeApi extends NativeApi {

    public static native byte[] nativeBIOIdentityVerify(int deviceID, int mode);

    public static native byte[] nativeBIOIdentityVerifyPIN(int deviceID, int mode, String pin);

    public static native int nativeBIOIdentityShowNineGrids(int deviceID);

    public static native int nativeBIOIdentityCancelNineGrids(int deviceID);

    public static native byte[] nativeBIOEnrollFingerprint(int deviceID, int fpTimeout, byte[] fgptIndex);

    public static native byte[] nativeBIOEnumFingerprint(int deviceID);

    public static native int nativeBIOEraseFingerprint(int deviceID, int fpTimeout);

    public static native int nativeBIODeleteFingerprint(int deviceID, int fpTimeout, byte fgptID);

    public static native byte[] nativeBIOVerifyFgptForIntl(int deviceID);

    public static native byte[] nativeBIOVerifyFingerprint(int contextID);

}
