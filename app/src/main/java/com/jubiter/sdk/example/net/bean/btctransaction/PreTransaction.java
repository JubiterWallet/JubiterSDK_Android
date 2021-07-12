
package com.jubiter.sdk.example.net.bean.btctransaction;

import java.util.List;
import com.google.gson.annotations.SerializedName;

public class PreTransaction {

    @SerializedName("fee")
    private String mFee;
    @SerializedName("inputs")
    private List<Input> mInputs;
    @SerializedName("outputs")
    private List<Output> mOutputs;

    public String getFee() {
        return mFee;
    }

    public void setFee(String fee) {
        mFee = fee;
    }

    public List<Input> getInputs() {
        return mInputs;
    }

    public void setInputs(List<Input> inputs) {
        mInputs = inputs;
    }

    public List<Output> getOutputs() {
        return mOutputs;
    }

    public void setOutputs(List<Output> outputs) {
        mOutputs = outputs;
    }

}
