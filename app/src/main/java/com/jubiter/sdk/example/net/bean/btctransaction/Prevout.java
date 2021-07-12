
package com.jubiter.sdk.example.net.bean.btctransaction;

import com.google.gson.annotations.SerializedName;

public class Prevout {

    @SerializedName("hash")
    private String mHash;
    @SerializedName("index")
    private int mIndex;

    public String getHash() {
        return mHash;
    }

    public void setHash(String hash) {
        mHash = hash;
    }

    public int getIndex() {
        return mIndex;
    }

    public void setIndex(int index) {
        mIndex = index;
    }

}
