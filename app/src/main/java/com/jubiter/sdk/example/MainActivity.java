package com.jubiter.sdk.example;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.RadioButton;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Size;
import androidx.appcompat.app.AppCompatActivity;


import com.jubiter.sdk.ConnectionStateCallback;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.Utils;

import java.util.List;

import pub.devrel.easypermissions.AppSettingsDialog;
import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends AppCompatActivity implements EasyPermissions.PermissionCallbacks {

    private TextView mTxtScanBle, mTxtOffBle, mTxtLog;
    private RadioButton mRadioBle, mRadioNFC, mRadioBio, mRadioSoft;
    private ScrollView mScrollView;

    private Context mContext;
    private final static int REQUEST_PERMISSION = 0x1001;

    private boolean isConnect = false;
    private JubiterImpl mJubiter;
    private ProgressDialog mDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;
        initView();
    }

    private void initView() {
        mTxtScanBle = findViewById(R.id.txt_scan_ble);
        mTxtOffBle = findViewById(R.id.txt_off_ble);
        mRadioBle = findViewById(R.id.radio_ble);
        mRadioNFC = findViewById(R.id.radio_nfc);
        mRadioBio = findViewById(R.id.radio_bio);
        mRadioSoft = findViewById(R.id.radio_soft);
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);
        mDialog = new ProgressDialog(this);

        mRadioBle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRadioBle.setChecked(true);
                mRadioNFC.setChecked(false);
                mRadioBio.setChecked(false);
                mRadioSoft.setChecked(false);
                mTxtScanBle.setVisibility(View.VISIBLE);
                mTxtOffBle.setVisibility(View.VISIBLE);
                disconnectBle();
                if (!hasPermissions()) {
                    requestPermissions("Permission request", REQUEST_PERMISSION, Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION);
                } else {
                    mJubiter = JubiterImpl.getInstance(mContext);
                }
            }
        });
        mRadioBle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRadioBle.setChecked(true);
                mRadioNFC.setChecked(false);
                mRadioBio.setChecked(false);
                mRadioSoft.setChecked(false);
                mTxtScanBle.setVisibility(View.VISIBLE);
                mTxtOffBle.setVisibility(View.VISIBLE);
                disconnectBle();
                if (!hasPermissions()) {
                    requestPermissions("Permission request", REQUEST_PERMISSION, Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION);
                } else {
                    mJubiter = JubiterImpl.getInstance(mContext);
                }
            }
        });
        mRadioBio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRadioBle.setChecked(false);
                mRadioNFC.setChecked(false);
                mRadioBio.setChecked(true);
                mRadioSoft.setChecked(false);
                mTxtScanBle.setVisibility(View.VISIBLE);
                mTxtOffBle.setVisibility(View.VISIBLE);
                disconnectBle();
                if (!hasPermissions()) {
                    requestPermissions("Permission request", REQUEST_PERMISSION, Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION);
                } else {
                    mJubiter = JubiterImpl.getInstance(mContext);
                }
            }
        });
        mRadioNFC.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRadioBle.setChecked(false);
                mRadioBio.setChecked(false);
                mRadioSoft.setChecked(false);
                mRadioNFC.setChecked(true);
                mTxtScanBle.setVisibility(View.GONE);
                mTxtOffBle.setVisibility(View.GONE);
                disconnectBle();
            }
        });
        mRadioSoft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRadioBle.setChecked(false);
                mRadioNFC.setChecked(false);
                mRadioBio.setChecked(false);
                mRadioSoft.setChecked(true);
                mTxtScanBle.setVisibility(View.GONE);
                mTxtOffBle.setVisibility(View.GONE);
                disconnectBle();
            }
        });
        mRadioBle.performClick();
    }

    public void onClick(View view) {
        int id = view.getId();
        if (id == R.id.txt_scan_ble) {
            scanBle();
        } else if (id == R.id.txt_off_ble) {
            disconnectBle();
        } else if (id == R.id.delete) {
            mTxtLog.setText("");
        } else if (id == R.id.btn_btc) {
            toBTCActivity();
        } else if (id == R.id.btn_eth) {
            toETHActivity();
        } else if (id == R.id.btn_eos) {
            toEOSActivity();
        } else if (id == R.id.btn_finger) {
            toFingerActivity();
        } else if (id == R.id.btn_xrp) {
            toXRPActivity();
        } else if (id == R.id.btn_trx) {
            toTRXActivity();
        } else if (id == R.id.btn_device) {
            toDeviceActivity();
        }
    }

    private void toDeviceActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, DeviceActivity.class));
        }
    }

    private void scanBle() {
        mJubiter.connect(new ConnectionStateCallback() {
            @Override
            public void onConnected(String mac, int handle) {
                isConnect = true;
                showLog("DeviceMac:" + mac);
                getDeviceType();
            }

            @Override
            public void onDisconnected(String mac) {
                isConnect = false;
            }

            @Override
            public void onError(int error) {
                isConnect = false;
                showToast(String.format("连接失败：0x%x", error) + "\n");
            }
        });
    }

    private void getDeviceType() {
        mJubiter.getDeviceType(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("getDeviceType:" + s);
            }

            @Override
            public void onFailed(long errorCode) {

            }
        });
    }

    private void disconnectBle() {
        if (isConnect) {
            mJubiter.disConnectBt();
            isConnect = false;
            showLog("disConnectBt");
        }
    }

    private void toBTCActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, BTCActivity.class));
        }
    }

    private void toETHActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, ETHActivity.class));
        }
    }

    private void toXRPActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, XRPActivity.class));
        }
    }

    private void toTRXActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, TRXActivity.class));
        }
    }

    private void toEOSActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, EOSActivity.class));
        }
    }

    private void toFingerActivity() {
        if (isConnect) {
            startActivity(new Intent(mContext, FingerPrintManagerActivity.class));
        }
    }

    public boolean hasPermissions() {
        return EasyPermissions.hasPermissions(this, Manifest.permission.ACCESS_COARSE_LOCATION);
    }

    public void requestPermissions(@NonNull String rationale,
                                   int requestCode, @Size(min = 1) @NonNull String... perms) {
        EasyPermissions.requestPermissions(this, rationale, requestCode, perms);
    }

    @Override
    public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {
        mJubiter = JubiterImpl.getInstance(this);
    }

    @Override
    public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {
        if (EasyPermissions.somePermissionPermanentlyDenied(this, perms)) {
            new AppSettingsDialog.Builder(this).build().show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    void showToast(final String tip) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(mContext, tip, Toast.LENGTH_SHORT).show();
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