
package com.jubiter.sdk.example.net.bean.txid;

import com.google.gson.annotations.SerializedName;

public class TxStatus {

    @SerializedName("data")
    private TxStatusData mTxStatusData;
    @SerializedName("errmsg")
    private String mErrmsg;
    @SerializedName("statusCode")
    private Long mStatusCode;

    public TxStatusData getData() {
        return mTxStatusData;
    }

    public void setData(TxStatusData txStatusData) {
        mTxStatusData = txStatusData;
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
