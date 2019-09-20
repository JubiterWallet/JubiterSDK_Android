package com.jubiter.sdk.jni;

/**
 * @author fengshuo
 * @date 2019/9/19
 * @time 15:40
 */
public class JubCommon {

    static {
        System.loadLibrary("coreNDK");
    }


    //************************************ 蓝牙接口 **************************************

    public static native int nativeInit();

    public static native int nativeStartScan(InnerScanCallback scanCallback);





    //************************************ 功能接口 **************************************

    public static native String nativeGenerateMnemonic(String param);


}
