package com.jubiter.sdk.example.net.eth;

import com.jubiter.sdk.example.net.bean.SimpleBean;
import com.jubiter.sdk.example.net.bean.ethhistory.EthHistory;
import com.jubiter.sdk.example.net.bean.ethinfo.EthAccountInfo;
import com.jubiter.sdk.example.net.bean.fee.Fees;
import com.jubiter.sdk.example.net.bean.txid.TxStatus;

import retrofit2.Call;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.POST;


public interface EthApiService {

    /**
     *
     * @param address
     * @param contractAddress
     * @param upordown
     * @param udBlkHash
     * @param coinType
     * @return
     */
    @POST("api/v2/queryTransactionsByAddrs/breif")
    @FormUrlEncoded
    Call<EthHistory> queryTransactionsByAddrs(@Field("account") String address,
                                              @Field("contractAddress") String contractAddress,
                                              @Field("upordown") String upordown,
                                              @Field("page") int udBlkHash,
                                              @Field("pageSize") int coinType);

    /**
     * 获取账户信息
     * @param address
     * @param contractAddress
     * @return
     */
    @POST("api/v2/queryAccountInfoByAddr")
    @FormUrlEncoded
    Call<EthAccountInfo> queryAccountInfoByAddr(@Field("address") String address,
                                                @Field("contractAddress") String contractAddress);
    /**
     * getMinerFeeEstimations
     * @return
     */
    @POST("api/v2/getMinerFeeEstimations")
    Call<Fees> getMinerFeeEstimations();

    /**
     * queryTokenGasUsed
     * @return
     */
    @POST("api/queryTokenGasUsed")
    Call<Fees> queryTokenGasUsed();

    /**
     * 广播交易
     *
     * @param rawtx
     * @return
     */
    @POST("api/v2/broadcastTransaction")
    @FormUrlEncoded
    Call<SimpleBean> broadcastTransaction(@Field("rawtx") String rawtx);

    /**
     *
     * @param txid
     * @return
     */
    @POST("api/v2/queryTransactionById")
    @FormUrlEncoded
    Call<TxStatus> queryTransactionById(@Field("txid") String txid);


}
