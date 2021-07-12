
package com.jubiter.sdk.example.net.bean.ethhistory;

import com.google.gson.annotations.SerializedName;

public class EthHistory {

    @SerializedName("code")
    private Long mCode;
    @SerializedName("data")
    private EthHistoryData mEthHistoryData;
    @SerializedName("msg")
    private String mMsg;

    public Long getCode() {
        return mCode;
    }

    public void setCode(Long code) {
        mCode = code;
    }

    public EthHistoryData getData() {
        return mEthHistoryData;
    }

    public void setData(EthHistoryData ethHistoryData) {
        mEthHistoryData = ethHistoryData;
    }

    public String getMsg() {
        return mMsg;
    }

    public void setMsg(String msg) {
        mMsg = msg;
    }

}
