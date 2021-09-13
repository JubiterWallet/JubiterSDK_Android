package com.jubiter.sdk;

import android.util.Log;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.BioNativeApi;
import com.jubiter.sdk.jni.BleNativeApi;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.jni.ble.InnerDiscCallback;
import com.jubiter.sdk.jni.ble.InnerScanCallback;
import com.jubiter.sdk.proto.CommonProtos;

/**
 * @author jifeng
 * @date 2021/9/10
 * @time 14:57
 */
public final class JuBiterBIOWallet {


    /**
     *
     * @param deviceID  设备ID
     * @param mode      验证模式
     * @return
     */
    public static CommonProtos.ResultInt identityVerify(int deviceID, int mode) {
        try {
            byte[] result = BioNativeApi.nativeBIOIdentityVerify(deviceID, mode);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param deviceID  设备ID
     * @param mode      验证模式
     * @param pin
     * @return
     */
    public static CommonProtos.ResultInt identityVerifyPIN(int deviceID, int mode, String pin) {
        try {
            byte[] result = BioNativeApi.nativeBIOIdentityVerifyPIN(deviceID, mode, pin);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param deviceID  设备ID
     * @return
     */
    public static int identityShowNineGrids(int deviceID) {
        return BioNativeApi.nativeBIOIdentityShowNineGrids(deviceID);
    }

    /**
     *
     * @param deviceID  设备ID
     * @return
     */
    public static int identityCancelNineGrids(int deviceID) {
        return BioNativeApi.nativeBIOIdentityCancelNineGrids(deviceID);
    }

    /**
     *
     * @param deviceID  设备ID
     * @param fpTimeout 超时
     * @param fgptIndex 轮换次序
     * @return
     */
    public static CommonProtos.ResultAny enrollFingerprint(int deviceID, int fpTimeout, int fgptIndex) {
        try {
            byte[] index = new byte[1];
            index[0] = (byte)fgptIndex;
            byte[] result = BioNativeApi.nativeBIOEnrollFingerprint(deviceID, fpTimeout, index);
            return CommonProtos.ResultAny.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param deviceID  设备ID
     * @return
     */
    public static CommonProtos.ResultString enumFingerprint(int deviceID) {
        try {
            byte[] result = BioNativeApi.nativeBIOEnumFingerprint(deviceID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param deviceID  设备ID
     * @param fpTimeout
     * @return
     */
    public static int eraseFingerprint(int deviceID, int fpTimeout) {
        return BioNativeApi.nativeBIOEraseFingerprint(deviceID, fpTimeout);
    }

    /**
     *
     * @param deviceID  设备ID
     * @param fpTimeout
     * @param fgptID  指纹ID
     * @return
     */
    public static int deleteFingerprint(int deviceID, int fpTimeout, int fgptID) {
        return BioNativeApi.nativeBIODeleteFingerprint(deviceID, fpTimeout, (byte)fgptID);
    }

    /**
     *
     * @param deviceID  设备ID
     * @return
     */
    public static CommonProtos.ResultInt verifyFgptForIntl(int deviceID) {
        try {
            byte[] result = BioNativeApi.nativeBIOVerifyFgptForIntl(deviceID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     *
     * @param contextID  币种上下文
     * @return
     */
    public static CommonProtos.ResultInt verifyFingerprint(int contextID) {
        try {
            byte[] result = BioNativeApi.nativeBIOVerifyFingerprint(contextID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }
}
