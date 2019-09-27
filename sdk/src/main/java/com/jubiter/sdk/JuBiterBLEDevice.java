package com.jubiter.sdk;

/**
 * @author fengshuo
 * @date 2019/9/25
 * @time 15:59
 */
public class JuBiterBLEDevice {

    private String name;
    private String mac;
    private int deviceType;

    public JuBiterBLEDevice(String name, String mac, int deviceType) {
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
