package com.jubiter.sdk.example;

import static org.junit.Assert.assertEquals;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.jubiter.sdk.JuBiterEthereum;
import com.jubiter.sdk.JuBiterSWIWallet;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EthereumProtos;

import org.junit.Before;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.MethodSorters;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class ETHTest extends BaseTest {

    private static final String TAG = "ETHTest";

    private static final String MAIN_PATH = "m/44\'/60\'/0\'";

    private static int contextID;

    @Before
    public void setUp() throws Exception {
        System.loadLibrary("coreNDK");
    }

//    void init() {
//        System.loadLibrary("coreNDK");
//    }

    @Test
    public void step1_createContext() {
//        init();
        CommonProtos.ResultInt device = JuBiterSWIWallet.swiConnectDevice();
        int deviceID = device.getValue();
        JuBiterSWIWallet.swiBuildFromMasterPrivateKey(
                deviceID,
                CommonProtos.CURVES.SECP256K1,
                BaseTest.ROOT_KEY
        );
        EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                .setMainPath(MAIN_PATH)
                .setChainId(1)
                .build();
        CommonProtos.ResultInt result = JuBiterEthereum.createContext(config,deviceID);
        assertEquals(0, result.getStateCode());

        contextID = result.getValue();
        Log.d(TAG, ">>> createContext_Software value: " + result.getValue());
    }

    @Test
    public void step2_getMainHDNode() {
        CommonProtos.ResultString result = JuBiterEthereum.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getMainHDNode value: " + result.getValue());
    }

    @Test
    public void step3_getHDNode() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterEthereum.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB, bip32Path);

        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getHDNode value: " + result.getValue());
    }

    @Test
    public void step4_getAddress() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        CommonProtos.ResultString result = JuBiterEthereum.getAddress(contextID, bip32Path, false);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> getAddress value: " + result.getValue());
    }

    @Test
    public void step5_setAddress() {

    }


    @Test
    public void step6_buildERC20Abi() {
//        JuBiterEthereum.setERC20Token(contextID, "USDT", 6, "TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG");
//
//        CommonProtos.ResultString result = JuBiterEthereum.buildERC20TransferAbi(
//                contextID,
//                "TX8K7GXogXRPZnUsxmQrM3ZyHKXQwd93ZQ",
//                "156000000");
//        assertEquals(0, result.getStateCode());
//        Log.d(TAG, ">>> buildERC20TransferAbi value: " + result.getValue());
    }

    @Test
    public void step8_signTransaction() {

//        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
//                .setChange(false)
//                .setAddressIndex(0)
//                .build();
//        EthereumProtos.TransactionETH transactionETH = EthereumProtos.TransactionETH.newBuilder()
//                .setPath(bip32Path)
//                .setNonce(13)
//                .setGasLimit(310000)
//                .setGasPriceInWei("10000000000")
//                .setTo("0xef31DEc147DCDcd64F6a0ABFA7D441B62A216BC9")
//                .setValueInWei("500000000000000")
//                .setInput("4A75626974657257616C6C6574")
//                .build();
//        CommonProtos.ResultString result = JuBiterEthereum.signTransaction(contextID, transactionETH);
//
//        assertEquals(0, result.getStateCode());
//        Log.d(TAG, ">>> signTransaction value : " + result.getValue());
    }

    @Test
    public void step9_singTypedData() {
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        String typedData = "{\n" +
                "    \"types\": {\n" +
                "        \"EIP712Domain\": [\n" +
                "            {\"name\": \"name\", \"type\": \"string\"},\n" +
                "            {\"name\": \"version\", \"type\": \"string\"},\n" +
                "            {\"name\": \"chainId\", \"type\": \"uint256\"},\n" +
                "            {\"name\": \"verifyingContract\", \"type\": \"address\"}\n" +
                "        ],\n" +
                "        \"Person\": [\n" +
                "            {\"name\": \"name\", \"type\": \"string\"},\n" +
                "            {\"name\": \"wallet\", \"type\": \"address\"}\n" +
                "        ],\n" +
                "        \"Mail\": [\n" +
                "            {\"name\": \"from\", \"type\": \"Person\"},\n" +
                "            {\"name\": \"to\", \"type\": \"Person\"},\n" +
                "            {\"name\": \"contents\", \"type\": \"string\"}\n" +
                "        ]\n" +
                "    },\n" +
                "    \"primaryType\": \"Mail\",\n" +
                "    \"domain\": {\n" +
                "        \"name\": \"Ether Mail\",\n" +
                "        \"version\": \"1\",\n" +
                "        \"chainId\": 1,\n" +
                "        \"verifyingContract\": \"0x1e0Ae8205e9726E6F296ab8869160A6423E2337E\"\n" +
                "    },\n" +
                "    \"message\": {\n" +
                "        \"from\": {\"name\": \"Cow\", \"wallet\": \"0xc0004B62C5A39a728e4Af5bee0c6B4a4E54b15ad\"},\n" +
                "        \"to\": {\"name\": \"Bob\", \"wallet\": \"0x54B0Fa66A065748C40dCA2C7Fe125A2028CF9982\"},\n" +
                "        \"contents\": \"Hello, Bob!\"\n" +
                "    }\n" +
                "}\n";

        CommonProtos.ResultString result = JuBiterEthereum.signTypedData(contextID, bip32Path, typedData, true);

        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> signTypedData value : " + result.getValue());

    }
}
