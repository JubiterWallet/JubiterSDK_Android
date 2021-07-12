
package com.jubiter.sdk.example.net.bean.fee;

import com.google.gson.annotations.SerializedName;

public class Fees {

    @SerializedName("code")
    private Long mCode;
    @SerializedName("data")
    private FeesData mFeesData;
    @SerializedName("msg")
    private String mMsg;

    public Long getCode() {
        return mCode;
    }

    public void setCode(Long code) {
        mCode = code;
    }

    public FeesData getData() {
        return mFeesData;
    }

    public void setData(FeesData feesData) {
        mFeesData = feesData;
    }

    public String getMsg() {
        return mMsg;
    }

    public void setMsg(String msg) {
        mMsg = msg;
    }

}
