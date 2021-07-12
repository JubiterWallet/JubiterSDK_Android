
package com.jubiter.sdk.example.net.bean.txid;

import java.util.List;
import com.google.gson.annotations.SerializedName;

public class TxStatusData {

    @SerializedName("blkHash")
    private String mBlkHash;
    @SerializedName("blkIndex")
    private Long mBlkIndex;
    @SerializedName("blkTime")
    private Long mBlkTime;
    @SerializedName("blockIndex")
    private Long mBlockIndex;
    @SerializedName("inputs")
    private List<Input> mInputs;
    @SerializedName("raw")
    private String mRaw;
    @SerializedName("txid")
    private String mTxid;

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

    public Long getBlockIndex() {
        return mBlockIndex;
    }

    public void setBlockIndex(Long blockIndex) {
        mBlockIndex = blockIndex;
    }

    public List<Input> getInputs() {
        return mInputs;
    }

    public void setInputs(List<Input> inputs) {
        mInputs = inputs;
    }

    public String getRaw() {
        return mRaw;
    }

    public void setRaw(String raw) {
        mRaw = raw;
    }

    public String getTxid() {
        return mTxid;
    }

    public void setTxid(String txid) {
        mTxid = txid;
    }

}
