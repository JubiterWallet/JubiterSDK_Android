package com.jubiter.sdk.example;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.jubiter.sdk.JuBiterEthereum;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EthereumProtos;

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
public class ETHTest {

    private static final String TAG = "ETHInstrumentedTest";

    @Before
    public void setUp() throws Exception {
        Log.d(TAG, "ETH 测试");
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
        EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                .setMainPath("m/44\'/0\'/0\'")
                .build();
        CommonProtos.ResultInt result = JuBiterEthereum.createContext_Software(config,
                "xprv9s21ZrQH143K2Qpcfq2KcJ2XNc3NRuTiUHNxgN7xhNHwS9wQjN8F4e5pwVdwodTzh7NFoY714xztHdrJboGzhLL2yGjuXD2oXc69SGRynr");
        Log.d(TAG, ">>> rv: " + result.getStateCode() + " contextID: " + result.getValue());
        assertEquals(0, result.getStateCode());
    }
}
