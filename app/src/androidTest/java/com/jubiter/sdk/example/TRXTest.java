package com.jubiter.sdk.example;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.google.protobuf.Any;
import com.google.protobuf.ByteString;
import com.jubiter.sdk.JuBiterSWIWallet;
import com.jubiter.sdk.JuBiterTRX;
import com.jubiter.sdk.example.utils.HexUtils;
import com.jubiter.sdk.proto.CommonProtos;

import org.junit.Before;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.MethodSorters;
import org.tron.protos.Protocol;
import org.tron.protos.contract.Common;

import static org.junit.Assert.assertEquals;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class TRXTest extends BaseTest {

    private static final String TAG = "TRXTest";

    private static final String MAIN_PATH = "m/44\'/195\'/0\'";

    private static int contextID;

    @Before
    public void setUp() throws Exception {
        System.loadLibrary("coreNDK");
    }

    @Test
    public void step01_createContext_Software() {
        CommonProtos.ResultInt device = JuBiterSWIWallet.swiConnectDevice();
        int deviceID = device.getValue();
        JuBiterSWIWallet.swiBuildFromMasterPrivateKey(
                deviceID,
                CommonProtos.CURVES.SECP256K1,
                BaseTest.ROOT_KEY
        );


        CommonProtos.ContextCfg config = CommonProtos.ContextCfg.newBuilder()
                .setMainPath(MAIN_PATH)
                .build();
        CommonProtos.ResultInt result = JuBiterTRX.createContext(config,deviceID);
        assertEquals(0, result.getStateCode());

        contextID = result.getValue();
        Log.d(TAG, ">>> createContext_Software value: " + result.getValue());
    }

    @Test
    public void step02_getMainHDNode() {
        CommonProtos.ResultString result = JuBiterTRX.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getMainHDNode value: " + result.getValue());
    }

    @Test
    public void step03_getHDNode() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterTRX.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, bip32Path);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getHDNode value: " + result.getValue());
    }

    @Test
    public void step04_getAddress() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterTRX.getAddress(contextID, bip32Path, false);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getAddress value: " + result.getValue());
    }

    @Test
    public void step05_setAddress() {

    }


    @Test
    public void step06_buildTRC20Abi() {
        JuBiterTRX.setTRC20Token(contextID, "USDT", 6, "TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG");

        CommonProtos.ResultString result = JuBiterTRX.buildTRC20TransferAbi(
                contextID,
                "TX8K7GXogXRPZnUsxmQrM3ZyHKXQwd93ZQ",
                "156000000");
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> buildTRC20TransferAbi value: " + result.getValue());
    }

    @Test
    public void step07_packContract() {
        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
                                        .setToAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
                                        .setAmount(1)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(0)
                        .build())
                .build();

        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> packContract value : " + result.getValue());
    }

    @Test
    public void step08_signTransaction() {

        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
                                        .setToAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
                                        .setAmount(1)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom(HexUtils.fromString("8610")))
                        .setRefBlockHash(ByteString.copyFrom(HexUtils.fromString("6a630e523f995e67")))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(0)
                        .build())
                .build();
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        // 无数据缓存，跳过 verifyPIN

        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> packContract value : " + result.getValue());

        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, result.getValue());

        assertEquals(0, signRes.getStateCode());
        Log.d(TAG, ">>> signTransaction value : " + signRes.getValue());
    }

    @Test
    public void step09_freezeBalanceSign() {
        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                // 资源冻结
                                .setType(Protocol.Transaction.Contract.ContractType.FreezeBalanceContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.FreezeBalanceContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
                                        .setReceiverAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
                                        // 带宽、能量
                                        .setResource(Common.ResourceCode.BANDWIDTH)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(0)
                        .build())
                .build();
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        // 无数据缓存，跳过 verifyPIN

        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> packContract value : " + result.getValue());

        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, result.getValue());

        assertEquals(0, signRes.getStateCode());
        Log.d(TAG, ">>> signTransaction value : " + signRes.getValue());
    }

    @Test
    public void step10_unfreezeBalanceSign() {
        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                // 资源解冻
                                .setType(Protocol.Transaction.Contract.ContractType.UnfreezeBalanceContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.UnfreezeBalanceContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
                                        .setReceiverAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
                                        // 带宽、能量
                                        .setResource(Common.ResourceCode.BANDWIDTH)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(0)
                        .build())
                .build();
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        // 无数据缓存，跳过 verifyPIN

        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> packContract value : " + result.getValue());

        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, result.getValue());

        assertEquals(0, signRes.getStateCode());
        Log.d(TAG, ">>> signTransaction value : " + signRes.getValue());
    }

}
