
package com.jubiter.sdk.example.net.bean.btctransaction;

import com.google.gson.annotations.SerializedName;

public class PreTransactionBean {

    @SerializedName("code")
    private Long mCode;
    @SerializedName("msg")
    private String mMsg;
    @SerializedName("data")
    private PreTransaction mPreTransaction;

    public Long getCode() {
        return mCode;
    }

    public void setCode(Long code) {
        mCode = code;
    }

    public String getMsg() {
        return mMsg;
    }

    public void setMsg(String msg) {
        mMsg = msg;
    }

    public PreTransaction getPreTransaction() {
        return mPreTransaction;
    }

    public void setPreTransaction(PreTransaction preTransaction) {
        mPreTransaction = preTransaction;
    }

}
