
package com.jubiter.sdk.example.net.bean.btctransaction;

import com.google.gson.annotations.SerializedName;

public class Output {

    @SerializedName("address")
    private String mAddress;
    @SerializedName("amount")
    private String mAmount;
    @SerializedName("change")
    private Long mChange;
    @SerializedName("index")
    private Long mIndex;
    @SerializedName("isChange")
    private Boolean mIsChange;

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

    public Long getChange() {
        return mChange;
    }

    public void setChange(Long change) {
        mChange = change;
    }

    public Long getIndex() {
        return mIndex;
    }

    public void setIndex(Long index) {
        mIndex = index;
    }

    public Boolean getIsChange() {
        return mIsChange;
    }

    public void setIsChange(Boolean isChange) {
        mIsChange = isChange;
    }

}
