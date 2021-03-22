package com.jubiter.sdk.example;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.google.protobuf.Any;
import com.google.protobuf.ByteString;
import com.jubiter.sdk.JuBiterTRX;
import com.jubiter.sdk.proto.CommonProtos;

import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.MethodSorters;

import static org.junit.Assert.assertEquals;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class TRXTest {

    private static final String TAG = "TRXTest";

    private static int contextID;

//    @Before
//    public void setUp() throws Exception {
//        System.loadLibrary("coreNDK");
//    }

//    @Test
//    public void useAppContext() {
//        // Context of the app under test.
//        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
//
//        assertEquals("com.jubiter.sdk.example", appContext.getPackageName());
//    }

    public void init() {
        System.loadLibrary("coreNDK");
    }

    @Test
    public void stage1_createContext_Software() {
        init();

        CommonProtos.ContextCfg config = CommonProtos.ContextCfg.newBuilder()
                .setMainPath("m/44'/195'/0'")
                .build();
        CommonProtos.ResultInt result = JuBiterTRX.createContext_Software(config,
                "xprv9s21ZrQH143K2Qpcfq2KcJ2XNc3NRuTiUHNxgN7xhNHwS9wQjN8F4e5pwVdwodTzh7NFoY714xztHdrJboGzhLL2yGjuXD2oXc69SGRynrz");
        Log.d(TAG, ">>> createContext_Software rv: " + result.getStateCode() + " contextID: " + result.getValue());
        assertEquals(0, result.getStateCode());

        contextID = result.getValue();
    }

    @Test
    public void stage2_getMainHDNode() {
        CommonProtos.ResultString result = JuBiterTRX.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX);
        Log.d(TAG, ">>> getMainHDNode rv: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }

    @Test
    public void stage3_getHDNode() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterTRX.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB, bip32Path);
        Log.d(TAG, ">>> getHDNode rv: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }

    @Test
    public void stage4_getAddress() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterTRX.getAddress(contextID, bip32Path, false);
        Log.d(TAG, ">>> getAddress rv: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }

    @Test
    public void stage5_setAddress() {

    }


    @Test
    public void stage6_buildTRC20Abi() {
        CommonProtos.ResultString result = JuBiterTRX.buildTRC20Abi(
                contextID,
                "USDT",
                6,
                "TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t",
                "TX8K7GXogXRPZnUsxmQrM3ZyHKXQwd93ZQ",
                "156000000");
        Log.d(TAG, ">>> buildERC20Abi rv: " + result);
        assertEquals(0, result.getStateCode());
    }

    @Test
    public void stage7_packContract() {
        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWqFgX6kK4WN4bzq2CySTCmP9UTYfSHRrE".getBytes()))
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
        Log.d(TAG, ">>> packContract - rv : " + result.getStateCode() + " value: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }

    @Test
    public void stage8_signTransaction() {

        org.tron.protos.Protocol.Transaction transactionTrx = org.tron.protos.Protocol.Transaction.newBuilder()
                .setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
                                        .setOwnerAddress(ByteString.copyFrom("TWqFgX6kK4WN4bzq2CySTCmP9UTYfSHRrE".getBytes()))
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
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        // 无数据缓存，跳过 verifyPIN

        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);
        Log.d(TAG, ">>> packContract - rv : " + result.getStateCode() + " value: " + result.getValue());
        assertEquals(0, result.getStateCode());

        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, result.getValue());
        Log.d(TAG, ">>> signTransaction - rv : " + signRes.getStateCode() + " value: " + signRes.getValue());

        assertEquals(0, signRes.getStateCode());
    }

}
