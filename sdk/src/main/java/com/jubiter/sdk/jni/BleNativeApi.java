package com.jubiter.sdk.jni;

import com.jubiter.sdk.jni.ble.InnerDiscCallback;
import com.jubiter.sdk.jni.ble.InnerScanCallback;

/**
 * @author fengshuo
 * @date 2020/9/16
 * @time 13:15
 */
public class BleNativeApi extends NativeApi {

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

}
