package com.jubiter.sdk.example;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.jubiter.sdk.JuBiterBitcoin;
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
public class BTCTest {

    private static final String TAG = "BTCInstrumentedTest";

    @Before
    public void setUp() throws Exception {
        System.loadLibrary("coreNDK");
    }

//    @Test
//    public void useAppContext() {
//        // Context of the app under test.
//        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
//
//        assertEquals("com.jubiter.sdk.example", appContext.getPackageName());
//    }

    @Test
    public void createContext_Software() {
        BitcoinProtos.ContextCfgBTC config = BitcoinProtos.ContextCfgBTC.newBuilder()
                .setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                .setMainPath("m/44'/0'/0'")
                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH)
                .build();
        CommonProtos.ResultInt result = JuBiterBitcoin.createContext_Software(config, "xpub6CAxrkiSbwkn4LayKD6qBcZg4tQvhHBH7TofQjNV9Lb3cB5u8owxdLGfc2bKoz2McoviAMXzWHwSaqc5Sm8C9SWMsnvuBw1bjEwtWsMZ");
        Log.d(TAG, ">>> createContext_Software rv: " + result.getStateCode() + ", value: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }
}
