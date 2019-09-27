package com.jubiter.sdk;

/**
 * @author fengshuo
 * @date 2019/9/25
 * @time 15:51
 */
public interface ConnectionStateCallback {

    /**
     *
     * @param name
     */
    public void onConnected(String name, int handle);

    /**
     * 连接断开回调
     *
     * @param mac 设备MAC
     */
    public void onDisconnected(String mac);

    /**
     * 错误码
     *
     * @param error
     */
    public void onError(int error);

}
