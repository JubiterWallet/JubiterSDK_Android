
package com.jubiter.sdk.example.net.bean.broadcast;

import com.google.gson.annotations.SerializedName;

public class Broadcast {

    @SerializedName("code")
    private Long mCode;
    @SerializedName("data")
    private String mTxid;
    @SerializedName("msg")
    private String mMsg;

    public Long getCode() {
        return mCode;
    }

    public void setCode(Long code) {
        mCode = code;
    }

    public String getData() {
        return mTxid;
    }

    public void setData(String broadcastData) {
        mTxid = broadcastData;
    }

    public String getMsg() {
        return mMsg;
    }

    public void setMsg(String msg) {
        mMsg = msg;
    }

}
