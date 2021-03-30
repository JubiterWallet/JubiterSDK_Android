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

    private static final String TAG = "ETHTest";

    private static final String MAIN_PATH = "m/44\'/60\'/0\'";

    @Before
    public void setUp() throws Exception {
        System.loadLibrary("coreNDK");
    }

    @Test
    public void step1_createContext_Software() {
        EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                .setMainPath(MAIN_PATH)
                .build();
        CommonProtos.ResultInt result = JuBiterEthereum.createContext_Software(config, BaseTest.ROOT_KEY);
        assertEquals(0, result.getStateCode());
        Log.d(TAG, ">>> createContext_Software value : " + result.getValue());
    }
}
