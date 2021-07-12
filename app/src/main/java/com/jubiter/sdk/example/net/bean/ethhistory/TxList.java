
package com.jubiter.sdk.example.net.bean.ethhistory;

import java.util.List;
import com.google.gson.annotations.SerializedName;

public class TxList {

    @SerializedName("blkIndex")
    private Long mBlkIndex;
    @SerializedName("blkTime")
    private Long mBlkTime;
    @SerializedName("contractAddress")
    private String mContractAddress;
    @SerializedName("decimal")
    private Long mDecimal;
    @SerializedName("fee")
    private Long mFee;
    @SerializedName("from")
    private String mFrom;
    @SerializedName("internal")
    private List<Object> mInternal;
    @SerializedName("nonce")
    private String mNonce;
    @SerializedName("status")
    private Long mStatus;
    @SerializedName("symbol")
    private String mSymbol;
    @SerializedName("to")
    private String mTo;
    @SerializedName("tokenInfo")
    private TokenInfo mTokenInfo;
    @SerializedName("tokenValue")
    private String mTokenValue;
    @SerializedName("txType")
    private String mTxType;
    @SerializedName("txid")
    private String mTxid;
    @SerializedName("value")
    private Long mValue;

    public Long getBlkIndex() {
        return mBlkIndex;
    }

    public void setBlkIndex(Long blkIndex) {
        mBlkIndex = blkIndex;
    }

    public Long getBlkTime() {
        return mBlkTime;
    }

    public void setBlkTime(Long blkTime) {
        mBlkTime = blkTime;
    }

    public String getContractAddress() {
        return mContractAddress;
    }

    public void setContractAddress(String contractAddress) {
        mContractAddress = contractAddress;
    }

    public Long getDecimal() {
        return mDecimal;
    }

    public void setDecimal(Long decimal) {
        mDecimal = decimal;
    }

    public Long getFee() {
        return mFee;
    }

    public void setFee(Long fee) {
        mFee = fee;
    }

    public String getFrom() {
        return mFrom;
    }

    public void setFrom(String from) {
        mFrom = from;
    }

    public List<Object> getInternal() {
        return mInternal;
    }

    public void setInternal(List<Object> internal) {
        mInternal = internal;
    }

    public String getNonce() {
        return mNonce;
    }

    public void setNonce(String nonce) {
        mNonce = nonce;
    }

    public Long getStatus() {
        return mStatus;
    }

    public void setStatus(Long status) {
        mStatus = status;
    }

    public String getSymbol() {
        return mSymbol;
    }

    public void setSymbol(String symbol) {
        mSymbol = symbol;
    }

    public String getTo() {
        return mTo;
    }

    public void setTo(String to) {
        mTo = to;
    }

    public TokenInfo getTokenInfo() {
        return mTokenInfo;
    }

    public void setTokenInfo(TokenInfo tokenInfo) {
        mTokenInfo = tokenInfo;
    }

    public String getTokenValue() {
        return mTokenValue;
    }

    public void setTokenValue(String tokenValue) {
        mTokenValue = tokenValue;
    }

    public String getTxType() {
        return mTxType;
    }

    public void setTxType(String txType) {
        mTxType = txType;
    }

    public String getTxid() {
        return mTxid;
    }

    public void setTxid(String txid) {
        mTxid = txid;
    }

    public Long getValue() {
        return mValue;
    }

    public void setValue(Long value) {
        mValue = value;
    }

}
