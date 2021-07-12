
package com.jubiter.sdk.example.net.bean.btchisory;

import com.google.gson.annotations.SerializedName;

public class Vout {

    @SerializedName("address")
    private String mAddress;
    @SerializedName("amount")
    private String mAmount;
    @SerializedName("self")
    private Boolean mSelf;

    public String getAddress() {
        return mAddress;
    }

    public void setAddress(String address) {
        mAddress = address;
    }

    public String getAmount() {
        return mAmount;
    }

    public void setAmount(String amount) {
        mAmount = amount;
    }

    public Boolean getSelf() {
        return mSelf;
    }

    public void setSelf(Boolean self) {
        mSelf = self;
    }

    @Override
    public String toString() {
        return "Vout{" +
                "address='" + mAddress + '\'' +
                ", amount='" + mAmount + '\'' +
                ", self=" + mSelf +
                '}';
    }
}
