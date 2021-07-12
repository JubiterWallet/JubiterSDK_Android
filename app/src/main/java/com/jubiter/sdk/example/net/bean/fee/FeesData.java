
package com.jubiter.sdk.example.net.bean.fee;

import com.google.gson.annotations.SerializedName;

public class FeesData {

    @SerializedName("fastestFee")
    private Long mFastestFee;
    @SerializedName("halfHourFee")
    private Long mHalfHourFee;
    @SerializedName("hourFee")
    private Long mHourFee;

    public Long getFastestFee() {
        return mFastestFee;
    }

    public void setFastestFee(Long fastestFee) {
        mFastestFee = fastestFee;
    }

    public Long getHalfHourFee() {
        return mHalfHourFee;
    }

    public void setHalfHourFee(Long halfHourFee) {
        mHalfHourFee = halfHourFee;
    }

    public Long getHourFee() {
        return mHourFee;
    }

    public void setHourFee(Long hourFee) {
        mHourFee = hourFee;
    }

}
