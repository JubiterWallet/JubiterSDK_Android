
package com.jubiter.sdk.example.net.bean.ethinfo;

import java.util.List;
import com.google.gson.annotations.SerializedName;

public class EthAccountInfoData {

    @SerializedName("balance")
    private String mBalance;
    @SerializedName("localNonce")
    private String mLocalNonce;
    @SerializedName("nonce")
    private String mNonce;
    @SerializedName("pending")
    private Boolean mPending;
    @SerializedName("pendingTxs")
    private List<Object> mPendingTxs;
    @SerializedName("tokens")
    private List<Token> mTokens;

    public String getBalance() {
        return mBalance;
    }

    public void setBalance(String balance) {
        mBalance = balance;
    }

    public String getLocalNonce() {
        return mLocalNonce;
    }

    public void setLocalNonce(String localNonce) {
        mLocalNonce = localNonce;
    }

    public String getNonce() {
        return mNonce;
    }

    public void setNonce(String nonce) {
        mNonce = nonce;
    }

    public Boolean getPending() {
        return mPending;
    }

    public void setPending(Boolean pending) {
        mPending = pending;
    }

    public List<Object> getPendingTxs() {
        return mPendingTxs;
    }

    public void setPendingTxs(List<Object> pendingTxs) {
        mPendingTxs = pendingTxs;
    }

    public List<Token> getTokens() {
        return mTokens;
    }

    public void setTokens(List<Token> tokens) {
        mTokens = tokens;
    }

}
