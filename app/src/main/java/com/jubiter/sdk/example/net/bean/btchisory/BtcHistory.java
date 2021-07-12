
package com.jubiter.sdk.example.net.bean.btchisory;

import com.google.gson.annotations.SerializedName;

public class BtcHistory {

    @SerializedName("code")
    private Long mCode;
    @SerializedName("data")
    private BtcHistoryData mBtcHistoryData;
    @SerializedName("msg")
    private String mMsg;

    public Long getCode() {
        return mCode;
    }

    public void setCode(Long code) {
        mCode = code;
    }

    public BtcHistoryData getData() {
        return mBtcHistoryData;
    }

    public void setData(BtcHistoryData btcHistoryData) {
        mBtcHistoryData = btcHistoryData;
    }

    public String getMsg() {
        return mMsg;
    }

    public void setMsg(String msg) {
        mMsg = msg;
    }

    @Override
    public String toString() {
        return "BtcHistory{" +
                "code=" + mCode +
                ", data=" + mBtcHistoryData +
                ", msg='" + mMsg + '\'' +
                '}';
    }
}
