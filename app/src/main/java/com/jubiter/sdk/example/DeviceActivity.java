package com.jubiter.sdk.example;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.Utils;


public class DeviceActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;

    private Context mContext;
    private JubiterImpl mJubiter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);
    }

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.delete:
                mTxtLog.setText("");
                break;
            case R.id.query_battery:
                queryBattery();
                break;
            case R.id.device_info:
                getDeviceInfo();
                break;
            case R.id.device_applets:
                getApplets();
                break;
            case R.id.device_applet_info:
                getAppletInfo();
                break;
            case R.id.device_cert:
                getCert();
                break;
            case R.id.support_coin:
                getSupportCoins();
                break;
            default:
                break;
        }
    }

    private void getCert() {
        mJubiter.getDeviceCert(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("getDeviceCert " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("getDeviceCert error " + errorCode);
            }
        });
    }

    private void getApplets() {
        mJubiter.enumApplets(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("enumApplets " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("enumApplets error " + errorCode);
            }
        });
    }

    private void getAppletInfo() {
        mJubiter.enumAppletInfo(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("enumAppletInfo " + s);
                Log.d("fs", "applet info: " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("enumAppletInfo error " + errorCode);
            }
        });
    }

    private void getSupportCoins() {
        mJubiter.enumSupportCoins(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("enumSupportCoins: " + s);
                Log.d("fs", "coins: " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("enumSupportCoins error " + errorCode);
            }
        });
    }

    private void getDeviceInfo() {
        mJubiter.getDeviceInfo(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("getDeviceInfo " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("getDeviceInfo error " + errorCode);
            }
        });
    }

    private void queryBattery() {
        mJubiter.queryBattery(new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("queryBattery " + integer);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("queryBattery error " + errorCode);
            }
        });
    }

    void showLog(final String tip) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Utils.parseLog(tip, mScrollView, mTxtLog);
            }
        });
    }
}