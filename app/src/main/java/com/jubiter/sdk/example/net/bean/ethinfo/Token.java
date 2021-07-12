
package com.jubiter.sdk.example.net.bean.ethinfo;

import com.google.gson.annotations.SerializedName;

public class Token {

    @SerializedName("address")
    private String mAddress;
    @SerializedName("balance")
    private String mBalance;
    @SerializedName("coinType")
    private String mCoinType;
    @SerializedName("iconName")
    private String mIconName;
    @SerializedName("iconUrl")
    private String mIconUrl;
    @SerializedName("symbol")
    private String mSymbol;
    @SerializedName("tokenDecimal")
    private Long mTokenDecimal;
    @SerializedName("tokenGasusedMax")
    private String mTokenGasusedMax;
    @SerializedName("tokenName")
    private String mTokenName;
    @SerializedName("tokenType")
    private String mTokenType;
    @SerializedName("tokenWeight")
    private Long mTokenWeight;

    public String getAddress() {
        return mAddress;
    }

    public void setAddress(String address) {
        mAddress = address;
    }

    public String getBalance() {
        return mBalance;
    }

    public void setBalance(String balance) {
        mBalance = balance;
    }

    public String getCoinType() {
        return mCoinType;
    }

    public void setCoinType(String coinType) {
        mCoinType = coinType;
    }

    public String getIconName() {
        return mIconName;
    }

    public void setIconName(String iconName) {
        mIconName = iconName;
    }

    public String getIconUrl() {
        return mIconUrl;
    }

    public void setIconUrl(String iconUrl) {
        mIconUrl = iconUrl;
    }

    public String getSymbol() {
        return mSymbol;
    }

    public void setSymbol(String symbol) {
        mSymbol = symbol;
    }

    public Long getTokenDecimal() {
        return mTokenDecimal;
    }

    public void setTokenDecimal(Long tokenDecimal) {
        mTokenDecimal = tokenDecimal;
    }

    public String getTokenGasusedMax() {
        return mTokenGasusedMax;
    }

    public void setTokenGasusedMax(String tokenGasusedMax) {
        mTokenGasusedMax = tokenGasusedMax;
    }

    public String getTokenName() {
        return mTokenName;
    }

    public void setTokenName(String tokenName) {
        mTokenName = tokenName;
    }

    public String getTokenType() {
        return mTokenType;
    }

    public void setTokenType(String tokenType) {
        mTokenType = tokenType;
    }

    public Long getTokenWeight() {
        return mTokenWeight;
    }

    public void setTokenWeight(Long tokenWeight) {
        mTokenWeight = tokenWeight;
    }

}
