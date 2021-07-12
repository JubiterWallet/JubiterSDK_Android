package com.jubiter.sdk.example;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.jubiter.sdk.JuBiterBitcoin;
import com.jubiter.sdk.JuBiterSWIWallet;
import com.jubiter.sdk.jni.SwiNativeApi;
import com.jubiter.sdk.proto.BitcoinProtos;
import com.jubiter.sdk.proto.CommonProtos;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertEquals;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class BTCTest extends BaseTest {

    private static final String TAG = "BTCTest";

    private static final String MAIN_PATH = "m/44\'/0\'/0\'";

    @Before
    public void setUp() throws Exception {
        System.loadLibrary("coreNDK");
    }

    @Test
    public void step1_createContext_Software() {
        CommonProtos.ResultInt device = JuBiterSWIWallet.swiConnectDevice();
        int deviceID = device.getValue();
        JuBiterSWIWallet.swiBuildFromMasterPrivateKey(
                deviceID,
                CommonProtos.CURVES.SECP256K1,
                BaseTest.ROOT_KEY
        );

        BitcoinProtos.ContextCfgBTC config = BitcoinProtos.ContextCfgBTC.newBuilder()
                .setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                .setMainPath(MAIN_PATH)
                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH)
                .build();
        CommonProtos.ResultInt result = JuBiterBitcoin.createContext(config, deviceID);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> createContext_Software value: " + result.getValue());
    }
}
