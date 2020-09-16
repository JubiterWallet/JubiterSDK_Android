package com.jubiter.sdk.jni.nfc;

/**
 * @author fengshuo
 * @date 2020/9/15
 * @time 16:38
 */
public interface NfcDiscCallback {
    /**
     * 连接断开回调
     *
     * @param mac 设备蓝牙MAC
     */
    public void onDisconnect(String mac);
}
