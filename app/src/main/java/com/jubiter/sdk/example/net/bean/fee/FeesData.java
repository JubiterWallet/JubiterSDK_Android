
package com.jubiter.sdk.example.net.bean.fee;

import com.google.gson.annotations.SerializedName;

public class FeesData {

    @SerializedName("fastestFee")
    private Double mFastestFee;
    @SerializedName("halfHourFee")
    private Double mHalfHourFee;
    @SerializedName("hourFee")
    private Double mHourFee;

    public Double getFastestFee() {
        return mFastestFee;
    }

    public void setFastestFee(Double fastestFee) {
        mFastestFee = fastestFee;
    }

    public Double getHalfHourFee() {
        return mHalfHourFee;
    }

    public void setHalfHourFee(Double halfHourFee) {
        mHalfHourFee = halfHourFee;
    }

    public Double getHourFee() {
        return mHourFee;
    }

    public void setHourFee(Double hourFee) {
        mHourFee = hourFee;
    }

}
