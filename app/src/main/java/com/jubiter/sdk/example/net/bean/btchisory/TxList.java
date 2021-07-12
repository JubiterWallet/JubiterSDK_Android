
package com.jubiter.sdk.example.net.bean.btchisory;

import java.util.List;
import com.google.gson.annotations.SerializedName;

public class TxList {

    @SerializedName("blkHash")
    private String mBlkHash;
    @SerializedName("blkIndex")
    private Long mBlkIndex;
    @SerializedName("blkTime")
    private Long mBlkTime;
    @SerializedName("decimal")
    private Long mDecimal;
    @SerializedName("fee")
    private String mFee;
    @SerializedName("status")
    private Long mStatus;
    @SerializedName("symbol")
    private String mSymbol;
    @SerializedName("txid")
    private String mTxid;
    @SerializedName("type")
    private Long mType;
    @SerializedName("value")
    private String mValue;
    @SerializedName("vin")
    private List<Vin> mVin;
    @SerializedName("vout")
    private List<Vout> mVout;

    public String getBlkHash() {
        return mBlkHash;
    }

    public void setBlkHash(String blkHash) {
        mBlkHash = blkHash;
    }

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

    public Long getDecimal() {
        return mDecimal;
    }

    public void setDecimal(Long decimal) {
        mDecimal = decimal;
    }

    public String getFee() {
        return mFee;
    }

    public void setFee(String fee) {
        mFee = fee;
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

    public String getTxid() {
        return mTxid;
    }

    public void setTxid(String txid) {
        mTxid = txid;
    }

    public Long getType() {
        return mType;
    }

    public void setType(Long type) {
        mType = type;
    }

    public String getValue() {
        return mValue;
    }

    public void setValue(String value) {
        mValue = value;
    }

    public List<Vin> getVin() {
        return mVin;
    }

    public void setVin(List<Vin> vin) {
        mVin = vin;
    }

    public List<Vout> getVout() {
        return mVout;
    }

    public void setVout(List<Vout> vout) {
        mVout = vout;
    }

    @Override
    public String toString() {
        return "TxList{" +
                "blkHash='" + mBlkHash + '\'' +
                ", blkIndex=" + mBlkIndex +
                ", blkTime=" + mBlkTime +
                ", decimal=" + mDecimal +
                ", fee='" + mFee + '\'' +
                ", status=" + mStatus +
                ", symbol='" + mSymbol + '\'' +
                ", txid='" + mTxid + '\'' +
                ", type=" + mType +
                ", value='" + mValue + '\'' +
                ", vin=" + mVin +
                ", vout=" + mVout +
                '}';
    }
}
