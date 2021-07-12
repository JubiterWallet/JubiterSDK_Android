
package com.jubiter.sdk.example.net.bean.ethinfo;

import com.google.gson.annotations.SerializedName;


public class EthAccountInfo {

    @SerializedName("data")
    private EthAccountInfoData mEthAccountInfoData;
    @SerializedName("msg")
    private String mErrmsg;
    @SerializedName("code")
    private Long mStatusCode;

    public EthAccountInfoData getData() {
        return mEthAccountInfoData;
    }

    public void setData(EthAccountInfoData ethAccountInfoData) {
        mEthAccountInfoData = ethAccountInfoData;
    }

    public String getErrmsg() {
        return mErrmsg;
    }

    public void setErrmsg(String errmsg) {
        mErrmsg = errmsg;
    }

    public Long getStatusCode() {
        return mStatusCode;
    }

    public void setStatusCode(Long statusCode) {
        mStatusCode = statusCode;
    }

}
