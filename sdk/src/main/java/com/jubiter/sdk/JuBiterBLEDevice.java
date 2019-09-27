package com.jubiter.sdk;

/**
 * @author fengshuo
 * @date 2019/9/25
 * @time 15:59
 */
public class JuBiterBLEDevice {

    private String mac;
    private String name;
    private int deviceType;

    public JuBiterBLEDevice(String mac, String name, int deviceType) {
        this.mac = mac;
        this.name = name;
        this.deviceType = deviceType;
    }

    public String getMac() {
        return mac;
    }

    public String getName() {
        return name;
    }

    public int getDeviceType() {
        return deviceType;
    }
}
