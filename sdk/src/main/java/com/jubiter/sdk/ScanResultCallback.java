package com.jubiter.sdk;

/**
 * @author fengshuo
 * @date 2019/9/25
 * @time 15:59
 */
public interface ScanResultCallback {

    /**
     * 搜索结果回调
     *
     * @param device 搜索到的BLE设备
     */
    public void onScanResult(JuBiterBLEDevice device);

    /**
     * 扫描停止
     */
    public void onStop();

    /**
     *
     * @param errorCode
     */
    public void onError(int errorCode);
}
