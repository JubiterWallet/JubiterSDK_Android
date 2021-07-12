
package com.jubiter.sdk.example.net.bean.ethhistory;

import com.google.gson.annotations.SerializedName;

public class TokenInfo {

    @SerializedName("coin_type")
    private String mCoinType;
    @SerializedName("token_addr")
    private String mTokenAddr;
    @SerializedName("token_decimal")
    private Long mTokenDecimal;
    @SerializedName("token_ename")
    private String mTokenEname;
    @SerializedName("token_gasused_max")
    private String mTokenGasusedMax;
    @SerializedName("token_icon_name")
    private String mTokenIconName;
    @SerializedName("token_icon_url")
    private String mTokenIconUrl;
    @SerializedName("token_symbol")
    private String mTokenSymbol;
    @SerializedName("token_type")
    private String mTokenType;
    @SerializedName("token_weight")
    private Long mTokenWeight;

    public String getCoinType() {
        return mCoinType;
    }

    public void setCoinType(String coinType) {
        mCoinType = coinType;
    }

    public String getTokenAddr() {
        return mTokenAddr;
    }

    public void setTokenAddr(String tokenAddr) {
        mTokenAddr = tokenAddr;
    }

    public Long getTokenDecimal() {
        return mTokenDecimal;
    }

    public void setTokenDecimal(Long tokenDecimal) {
        mTokenDecimal = tokenDecimal;
    }

    public String getTokenEname() {
        return mTokenEname;
    }

    public void setTokenEname(String tokenEname) {
        mTokenEname = tokenEname;
    }

    public String getTokenGasusedMax() {
        return mTokenGasusedMax;
    }

    public void setTokenGasusedMax(String tokenGasusedMax) {
        mTokenGasusedMax = tokenGasusedMax;
    }

    public String getTokenIconName() {
        return mTokenIconName;
    }

    public void setTokenIconName(String tokenIconName) {
        mTokenIconName = tokenIconName;
    }

    public String getTokenIconUrl() {
        return mTokenIconUrl;
    }

    public void setTokenIconUrl(String tokenIconUrl) {
        mTokenIconUrl = tokenIconUrl;
    }

    public String getTokenSymbol() {
        return mTokenSymbol;
    }

    public void setTokenSymbol(String tokenSymbol) {
        mTokenSymbol = tokenSymbol;
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
