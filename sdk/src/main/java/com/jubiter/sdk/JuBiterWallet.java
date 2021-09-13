package com.jubiter.sdk;

import android.util.Log;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.jni.BleNativeApi;
import com.jubiter.sdk.jni.NativeApi;
import com.jubiter.sdk.jni.ble.InnerDiscCallback;
import com.jubiter.sdk.jni.ble.InnerScanCallback;
import com.jubiter.sdk.proto.CommonProtos;

/**
 * @author fengshuo
 * @date 2019/9/23
 * @time 16:22
 */
public final class JuBiterWallet {

    /**
     * 根据指定的强度生成对应的助记码
     *
     * @param strength 助记码强度
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString generateMnemonic(CommonProtos.ENUM_MNEMONIC_STRENGTH strength) {
        try {
            byte[] mnemonic = NativeApi.nativeGenerateMnemonic(strength.toString().getBytes());
            return CommonProtos.ResultString.parseFrom(mnemonic);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 校验助记码
     *
     * @param mnemonic 助记码
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static int checkMnemonic(String mnemonic) {
        return NativeApi.nativeCheckMnemonic(mnemonic);
    }

    /**
     * 获取硬件设备信息
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultAny getDeviceInfo(int deviceID) {
        try {
            byte[] result = NativeApi.nativeGetDeviceInfo(deviceID);
            return CommonProtos.ResultAny.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取硬件设备信息
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultAny getDeviceType(int deviceID) {
        try {
            byte[] result = NativeApi.nativeGetDeviceType(deviceID);
            return CommonProtos.ResultAny.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取硬件设备整数
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString getDeviceCert(int deviceID) {
        try {
            byte[] result = NativeApi.nativeGetDeviceCert(deviceID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 发送 apdu
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @param apdu     待发送的自定义APDU
     * @return 若stateCode为0, 则表示执行成功，value即为执行结果，否则表示执行失败
     */
    public static CommonProtos.ResultString sendApdu(int deviceID, String apdu) {
        try {
            byte[] result = NativeApi.nativeSendAPDU(deviceID, apdu);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * 硬件是否处于 bootLoader 模式
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return
     */
    public static boolean isBootLoader(int deviceID) {
        return NativeApi.nativeIsBootLoader(deviceID);
    }

    /**
     * 设置超时时间
     *
     * @param contextID 下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param timeout   超时时间
     * @return
     */
    public static int setTimeout(int contextID, int timeout) {
        return NativeApi.nativeSetTimeout(contextID, timeout);
    }

    /**
     * 枚举设备中的 applet ID
     * <p>
     * applet ID 以空格分隔
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return
     */
    public static CommonProtos.ResultString enumApplets(int deviceID) {
        try {
            byte[] result = NativeApi.nativeEnumApplets(deviceID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 枚举设备中支持的币种
     * <p>
     * applet ID 以空格分隔
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return
     */
    public static CommonProtos.ResultString enumSupportCoins(int deviceID) {
        try {
            byte[] result = NativeApi.nativeEnumSupportCoins(deviceID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 获取指定 ID 应用的版本号
     *
     * 版本号规则：XX.XX.XX
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @param appletID 硬件设备内部 applet 的 ID
     * @return
     */
    public static CommonProtos.ResultString getAppletVersion(int deviceID, String appletID) {
        try {
            byte[] result = NativeApi.nativeGetAppletVersion(deviceID, appletID);
            return CommonProtos.ResultString.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 查询蓝牙设备电量
     *
     * @param deviceID 已连接的硬件设备ID，该值由 connectDevice 方法返回
     * @return
     */
    public static CommonProtos.ResultInt queryBattery(int deviceID) {
        try {
            byte[] result = NativeApi.nativeQueryBattery(deviceID);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 清空上下文
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @return
     */
    public static int clearContext(int contextID) {
        return NativeApi.nativeClearContext(contextID);
    }

    /**
     * 显示硬件设备虚拟密码
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @return
     */
    public static int showVirtualPWD(int contextID) {
        return NativeApi.nativeShowVirtualPWD(contextID);
    }

    /**
     * 取消硬件设备虚拟密码
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @return
     */
    public static int cancelVirtualPWD(int contextID) {
        return NativeApi.nativeCancelVirtualPWD(contextID);
    }

    /**
     * 校验钱包密码
     *
     * @param contextID 上下文ID，该值由 createContext_Software 或 createContext 方法返回
     * @param PIN       钱包密码
     * @return
     */
    public static CommonProtos.ResultInt verifyPIN(int contextID, String PIN) {
        try {
            byte[] result = NativeApi.nativeVerifyPIN(contextID, PIN);
            return CommonProtos.ResultInt.parseFrom(result);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return null;
    }


    //************************************ 蓝牙接口 **************************************

    /**
     * 初始化设备接口
     * <p>
     * 需要蓝牙权限 Bluetooth
     *
     * @return 0：成功；非0：失败
     */
    public static int initDevice() {
        return BleNativeApi.nativeBLEInitDevice();
    }

    /**
     * 扫描BLE设备
     * <p>
     * 异步接口，在扫描中回调中接收设备信息，若周边存在多个设备被搜索到则会回调多次
     *
     * @param scanResultCallback 扫描回调
     * @return 0：成功；非0：失败
     */
    public static void startScan(final ScanResultCallback scanResultCallback) {
        int rv = BleNativeApi.nativeBLEStartScan(new InnerScanCallback() {
            @Override
            public void onScanResult(String name, String uuid, int devType) {
                JuBiterBLEDevice device = new JuBiterBLEDevice(name, uuid, devType);
                scanResultCallback.onScanResult(device);
            }

            @Override
            public void onStop() {
                scanResultCallback.onStop();
            }

            @Override
            public void onError(int errorCode) {
                scanResultCallback.onError(errorCode);
            }
        });
        if (0 != rv) {
            scanResultCallback.onError(rv);
        }
    }

    /**
     * 停止搜索
     * <p>
     * 注：保证有 startScan 时，一定有与之对应的 stopScan
     *
     * @return 0：成功；非0：失败
     */
    public static int stopScan() {
        return BleNativeApi.nativeBLEStopScan();
    }

    /**
     * 连接 BLE 设备
     * <p>
     * 注: 异步接口，该接口只支持单设备连接，若要连接其他设备，需断开当前连接
     * <p>
     *
     * @param deviceMAC               待连接设备的 MAC 地址
     * @param timeout                 连接超时时间，单位毫秒（ms）
     * @param connectionStateCallback 设备断开连接回调
     * @return 0：成功；非0：失败
     */
    public static void connectDeviceAsync(final String deviceName, final String deviceMAC,
                                          final int timeout,
                                          final ConnectionStateCallback connectionStateCallback) {
        Log.d("JuBiterWallet", ">>> connectDeviceAsync name: " + deviceName + ", address: " + deviceMAC + ", timeout: " + timeout);
        new Thread(new Runnable() {
            @Override
            public void run() {
                int[] deviceHandle = new int[1];
                int rv = BleNativeApi.nativeBLEConnectDevice(deviceName, deviceMAC, deviceHandle, timeout, new InnerDiscCallback() {
                    @Override
                    public void onDisconnect(String mac) {
                        connectionStateCallback.onDisconnected(mac);
                    }
                });
                if (0 != rv) {
                    connectionStateCallback.onError(rv);
                } else {
                    connectionStateCallback.onConnected(deviceMAC, deviceHandle[0]);
                }
            }
        }).start();
    }

    /**
     * 取消正在连接的设备
     * <p>
     * 该使用场景：正在连接指定设备，但尚未连接成功，此时因某些原因想要终止当前操作
     *
     * @param deviceMAC 待取消连接的设备 MAC 地址
     * @return 0：成功；非0：失败
     */
    public static int cancelConnect(String deviceName, String deviceMAC) {
        return BleNativeApi.nativeBLECancelConnect(deviceName, deviceMAC);
    }

    /**
     * 断开连接指定设备
     *
     * @param deviceID 待操作的设备连接句柄
     * @return 0：成功；非0：失败
     */
    public static int disconnectDevice(int deviceID) {
        return BleNativeApi.nativeBLEDisconnectDevice(deviceID);
    }

    /**
     * 查询指定设备是否连接
     *
     * @param deviceID 待查询的设备连接句柄
     * @return 0：成功；非0：失败
     */
    public static boolean isConnected(int deviceID) {
        int rv = BleNativeApi.nativeBLEIsConnected(deviceID);
        if (0 == rv) {
            return true;
        }
        return false;
    }

}
