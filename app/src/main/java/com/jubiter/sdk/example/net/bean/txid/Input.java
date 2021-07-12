
package com.jubiter.sdk.example.net.bean.txid;

import com.google.gson.annotations.SerializedName;

public class Input {

    @SerializedName("preOutHash")
    private String mPreOutHash;
    @SerializedName("preOutIndex")
    private Long mPreOutIndex;
    @SerializedName("value")
    private String mValue;

    public String getPreOutHash() {
        return mPreOutHash;
    }

    public void setPreOutHash(String preOutHash) {
        mPreOutHash = preOutHash;
    }

    public Long getPreOutIndex() {
        return mPreOutIndex;
    }

    public void setPreOutIndex(Long preOutIndex) {
        mPreOutIndex = preOutIndex;
    }

    public String getValue() {
        return mValue;
    }

    public void setValue(String value) {
        mValue = value;
    }

}
