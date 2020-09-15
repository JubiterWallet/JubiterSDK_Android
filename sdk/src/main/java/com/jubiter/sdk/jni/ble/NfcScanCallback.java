package com.jubiter.sdk.jni.ble;

/**
 * @author fengshuo
 * @date 2020/9/15
 * @time 16:37
 */
public interface NfcScanCallback {
    /**
     * 搜索结果回调
     *
     * @param errorCode
     * @param uuid 设备 MAC 地址
     * @param devType 设备类型：音频、蓝牙、OTG、NFC
     */
    public void onScanResult(int errorCode, String uuid, int devType);

}
