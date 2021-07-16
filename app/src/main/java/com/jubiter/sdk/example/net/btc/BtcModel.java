package com.jubiter.sdk.example.net.btc;

import com.jubiter.sdk.example.net.RetrofitManager;
import com.jubiter.sdk.example.net.bean.SimpleBean;
import com.jubiter.sdk.example.net.bean.fee.Fees;
import com.jubiter.sdk.example.net.bean.btchisory.BtcHistory;
import com.jubiter.sdk.example.net.bean.btctransaction.PreTransactionBean;
import com.jubiter.sdk.example.net.bean.txid.TxStatus;

import java.io.IOException;

import retrofit2.Response;

/**
 * @Date 2021-07-07  9:35
 * @Author ZJF
 * @Version 1.0
 */
public class BtcModel {
    private static final BtcModel ourInstance = new BtcModel();
    private final BtcApiService mBtcService;

    public static BtcModel getInstance() {
        return ourInstance;
    }

    private BtcModel() {
        mBtcService = RetrofitManager.createBTC(BtcApiService.class);
    }

    public SimpleBean queryBalanceByAccount(String account) throws IOException {
        Response<SimpleBean> response =
                mBtcService.queryBalanceByAccount(account).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public BtcHistory queryTransactionByAccount(String account, int page) throws IOException {
        Response<BtcHistory> response =
                mBtcService.queryTransactionByAccount(account, "desc", page, 10).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public Fees getMinerFeeEstimations() throws IOException {
        Response<Fees> response =
                mBtcService.getMinerFeeEstimations().execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public PreTransactionBean getPreTransaction(
            String account,
            String feeRate,
            long targetValue,
            String targetAddress
    ) throws IOException {
        Response<PreTransactionBean> response =
                mBtcService.getPreTransaction(
                        account,
                        feeRate,
                        targetValue,
                        targetAddress,
                        "0",
                        "first",
                        "value"
                ).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public SimpleBean broadcastTransaction(String rawtx) throws IOException {
        Response<SimpleBean> response =
                mBtcService.broadcastTransaction(
                        rawtx
                ).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public TxStatus queryTransactionById(String txid) throws IOException {
        Response<TxStatus> response =
                mBtcService.queryTransactionById(
                        txid
                ).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }
}
