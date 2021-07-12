package com.jubiter.sdk.example.net.eth;

import android.util.Log;

import com.jubiter.sdk.example.net.RetrofitManager;
import com.jubiter.sdk.example.net.bean.broadcast.Broadcast;
import com.jubiter.sdk.example.net.bean.btchisory.BtcHistory;
import com.jubiter.sdk.example.net.bean.btctransaction.PreTransactionBean;
import com.jubiter.sdk.example.net.bean.ethhistory.EthHistory;
import com.jubiter.sdk.example.net.bean.ethinfo.EthAccountInfo;
import com.jubiter.sdk.example.net.bean.fee.Fees;
import com.jubiter.sdk.example.net.bean.txid.TxStatus;
import com.jubiter.sdk.example.net.btc.BtcApiService;

import java.io.IOException;

import retrofit2.Response;
import retrofit2.http.Field;

/**
 * @Date 2021-07-08  10:21
 * @Author ZJF
 * @Version 1.0
 */
public class EthModel {
    private static final EthModel ourInstance = new EthModel();
    private final EthApiService mEthService;

    public static EthModel getInstance() {
        return ourInstance;
    }

    private EthModel() {
        mEthService = RetrofitManager.createETH(EthApiService.class);
    }

    public EthAccountInfo queryAccountInfoByAddr(String address,
                                               String contractAddress) throws IOException {
        Response<EthAccountInfo> response =
                mEthService.queryAccountInfoByAddr(address, contractAddress).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public EthHistory queryTransactionsByAddrs(String address,
                                               String contractAddress, int page) throws IOException {
        Response<EthHistory> response =
                mEthService.queryTransactionsByAddrs(address, contractAddress, "desc", page, 10).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public Fees getMinerFeeEstimations() throws IOException {
        Response<Fees> response =
                mEthService.getMinerFeeEstimations().execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }


    public Broadcast broadcastTransaction(String rawtx) throws IOException {
        Response<Broadcast> response =
                mEthService.broadcastTransaction(
                        rawtx
                ).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }

    public TxStatus queryTransactionById(String txid) throws IOException {
        Response<TxStatus> response =
                mEthService.queryTransactionById(
                        txid
                ).execute();
        if (response.isSuccessful() && response.code() == 200 && response.body() != null) {
            return response.body();
        }
        return null;
    }
}
