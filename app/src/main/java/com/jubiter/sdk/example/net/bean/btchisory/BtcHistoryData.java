
package com.jubiter.sdk.example.net.bean.btchisory;

import com.google.gson.annotations.SerializedName;

import java.util.List;

@SuppressWarnings("unused")
public class BtcHistoryData {
    @SerializedName("page")
    private Long mPage;
    @SerializedName("pageSize")
    private Long mPageSize;
    @SerializedName("total")
    private Long mTotal;
    @SerializedName("totalPage")
    private Long mTotalPage;
    @SerializedName("TxLists")
    private List<TxList> mTxLists;

    public Long getPage() {
        return mPage;
    }

    public void setPage(Long page) {
        mPage = page;
    }

    public Long getPageSize() {
        return mPageSize;
    }

    public void setPageSize(Long pageSize) {
        mPageSize = pageSize;
    }

    public Long getTotal() {
        return mTotal;
    }

    public void setTotal(Long total) {
        mTotal = total;
    }

    public Long getTotalPage() {
        return mTotalPage;
    }

    public void setTotalPage(Long totalPage) {
        mTotalPage = totalPage;
    }

    public List<TxList> getTxLists() {
        return mTxLists;
    }

    public void setTxLists(List<TxList> txLists) {
        mTxLists = txLists;
    }

}
