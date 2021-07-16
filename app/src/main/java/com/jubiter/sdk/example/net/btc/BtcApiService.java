package com.jubiter.sdk.example.net.btc;

import com.jubiter.sdk.example.net.bean.SimpleBean;
import com.jubiter.sdk.example.net.bean.fee.Fees;
import com.jubiter.sdk.example.net.bean.btchisory.BtcHistory;
import com.jubiter.sdk.example.net.bean.btctransaction.PreTransactionBean;
import com.jubiter.sdk.example.net.bean.txid.TxStatus;

import retrofit2.Call;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.POST;


public interface BtcApiService {


    /**
     * 获取历史
     *
     * @param account
     * @return
     */
    @POST("/api/v2/queryTransactionsByAccount")
    @FormUrlEncoded
    Call<BtcHistory> queryTransactionByAccount(@Field("account") String account,
                                               @Field("upordown") String upordown,
                                               @Field("page") int page,
                                               @Field("pageSize") int pageSize);

    /**
     * getMinerFeeEstimations
     * @return
     */
    @POST("/api/v2/getMinerFeeEstimations")
    Call<Fees> getMinerFeeEstimations();

    /**
     * queryBalanceByAccount
     * @return
     */
    @POST("/api/v2/queryBalanceByAccount")
    @FormUrlEncoded
    Call<SimpleBean> queryBalanceByAccount(@Field("account") String account);


    /**
     *
     * @param account
     * @param feeRate
     * @param targetValue
     * @param targetAddress
     * @param isTranferAll
     * @param changeType
     * @param selectionType
     * @return
     */
    @POST("/api/v2/getPreTransaction")
    @FormUrlEncoded
    Call<PreTransactionBean> getPreTransaction(@Field("account") String account,
                                               @Field("fee") String feeRate,
                                               @Field("targetValue") long targetValue,
                                               @Field("targetAddress") String targetAddress,
                                               @Field("isTranferAll") String isTranferAll,
                                               @Field("changeType") String changeType,
                                               @Field("selectionType") String selectionType);

    /**
     * 广播交易
     *
     * @param rawtx
     * @return
     */
    @POST("/api/v2/broadcastTransaction")
    @FormUrlEncoded
    Call<SimpleBean> broadcastTransaction(@Field("rawtx") String rawtx);

    /**
     *
     * @param txid
     * @return
     */
    @POST("/api/queryTransactionById")
    @FormUrlEncoded
    Call<TxStatus> queryTransactionById(@Field("txid") String txid);


}
