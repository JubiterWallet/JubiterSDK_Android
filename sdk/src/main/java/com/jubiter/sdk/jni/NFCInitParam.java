package com.jubiter.sdk.jni;

import android.app.Activity;

import com.jubiter.sdk.jni.ble.NfcDiscCallback;
import com.jubiter.sdk.jni.ble.NfcScanCallback;

/**
 * @author fengshuo
 * @date 2020/9/10
 * @time 14:02
 */
public class NFCInitParam {
    public Activity mContext;
    public NfcScanCallback mNfcScanCallback;
    public NfcDiscCallback mNfcDiscCallback;

    public NFCInitParam(Activity context,
                        NfcScanCallback nfcScanCallback,
                        NfcDiscCallback nfcDiscCallback) {
        mContext = context;
        mNfcScanCallback = nfcScanCallback;
        mNfcDiscCallback = nfcDiscCallback;
    }
}
