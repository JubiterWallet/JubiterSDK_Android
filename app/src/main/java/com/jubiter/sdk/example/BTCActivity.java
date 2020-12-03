package com.jubiter.sdk.example;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.Bundle;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;


import com.jubiter.sdk.example.dialog.InputDialog;
import com.jubiter.sdk.example.dialog.PathDialog;
import com.jubiter.sdk.example.dialog.SelectDialog;
import com.jubiter.sdk.example.utils.DecimalDigitsInputFilter;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.Utils;
import com.jubiter.sdk.proto.BitcoinProtos;

import java.util.ArrayList;

public class BTCActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;
    private Spinner mSpinner;

    private Context mContext;
    private JubiterImpl mJubiter;
    private Integer mBtcContextID = -1;
    private SelectDialog mSelectDialog;
    private SelectDialog mSelectUnitDialog;
    private JubiterImpl.BTC_TransType mTransType;
    private String transferInputValue;
    private int unitIndx;
    private ArrayList<String> mUnits;

    private ProgressDialog mDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_btc);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);
        mSpinner = findViewById(R.id.spinner_btc);
        mDialog = new ProgressDialog(this);
        mSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                switch (i) {
                    case 0:
                        mTransType = JubiterImpl.BTC_TransType.BTC_P2PKH;
                        break;
                    case 1:
                        mTransType = JubiterImpl.BTC_TransType.BTC_P2WPKH;
                        break;
                    case 2:
                        mTransType = JubiterImpl.BTC_TransType.LTC;
                        break;
                    case 3:
                        mTransType = JubiterImpl.BTC_TransType.DASH;
                        break;
                    case 4:
                        mTransType = JubiterImpl.BTC_TransType.BCH;
                        break;
                    case 5:
                        mTransType = JubiterImpl.BTC_TransType.QTUM;
                        break;
                    case 6:
                        mTransType = JubiterImpl.BTC_TransType.USDT;
                        break;
                }
                rebuildContext();
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        ArrayList<String> strings = new ArrayList<>();
        strings.add("Use virtual keyboard");
        strings.add("Use fingerprint");
        mSelectDialog = new SelectDialog().init(mContext, strings)
                .setCancelable(false)
                .setTitle("Please select Verify PIN mode")
                .setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                        showVirtualPwd(i);
                        mSelectDialog.dismiss();
                    }
                });

        mUnits = new ArrayList<>();
        mUnits.add("BTC");
        mUnits.add("cBTC");
        mUnits.add("mBTC");
        mUnits.add("uBTC");
        mUnits.add("Satoshi");
        mSelectUnitDialog = new SelectDialog().init(mContext, mUnits)
                .setCancelable(false)
                .setTitle("Please select the Unit on the device")
                .setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                        unitIndx = i;
                        mSelectUnitDialog.dismiss();
                    }
                });
    }

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.delete:
                mTxtLog.setText("");
                break;
            case R.id.btc_trans:
                transfer();
                break;
            case R.id.btc_get_address:
                getAddress();
                break;
            case R.id.btc_show_address:
                showAddress();
                break;
            case R.id.btc_set_my_address:
                setMyAddress();
                break;
            case R.id.btc_set_timeout:
                setTimeout();
                break;
            case R.id.txt_set_unit:
                setUnit();
                break;
            default:
                break;
        }
    }

    private void setUnit() {
        mSelectUnitDialog.show();
    }

    private void rebuildContext() {
        mDialog.setMessage("Create context in progress....");
        mDialog.show();
        if (mBtcContextID != -1) {
            mJubiter.clearContext(mBtcContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("clearContext success");
                    createContext();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("clearContext " + errorCode);
                }
            });
        } else {
            createContext();
        }
    }

    private void createContext() {
        mJubiter.btcCreateContext(mTransType, new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("btcCreateContext success " + integer);
                mBtcContextID = integer;
                mDialog.dismiss();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("btcCreateContext error " + errorCode);
                mDialog.dismiss();
            }
        });
    }

    private void transfer() {
        setUnitExecute();
        //showSelectVerifyTypeDialog();
        String title = "Please input value";
        if (mTransType != JubiterImpl.BTC_TransType.USDT) {
            title = "Please input value (unit: " + mUnits.get(unitIndx) + ")";
        }
        InputDialog inputDialog = new InputDialog(mContext, title, new InputDialog.callback() {
            @Override
            public void onClickListener(String value) {
                if (TextUtils.isEmpty(value)) {
                    return;
                }
                transferInputValue = value;
                showSelectVerifyTypeDialog();
            }

            @Override
            public void onCancel() {

            }
        }).setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                .setFilters(new InputFilter[]{new DecimalDigitsInputFilter(8)});
        if (mTransType != JubiterImpl.BTC_TransType.USDT) {
            switch (unitIndx) {
                case 0://BTC
                    break;
                case 1://cBTC
                    inputDialog.setFilters(new InputFilter[]{new DecimalDigitsInputFilter(6)});
                    break;
                case 2://mBTC
                    inputDialog.setFilters(new InputFilter[]{new DecimalDigitsInputFilter(5)});
                    break;
                case 3://uBTC
                    inputDialog.setFilters(new InputFilter[]{new DecimalDigitsInputFilter(2)});
                    break;
                case 4://sat
                    inputDialog.setInputType(InputType.TYPE_CLASS_NUMBER);
                    break;
            }
        }
        inputDialog.show();
    }

    private void setUnitExecute() {
        mJubiter.setBTCUnit(mBtcContextID, BitcoinProtos.BTC_UNIT_TYPE.forNumber(unitIndx), new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("setBTCUnit success " + mUnits.get(unitIndx));
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("setBTCUnit error" + errorCode);
            }
        });
    }

    private void showSelectVerifyTypeDialog() {
//        DeviceType deviceType = mJubiter.getDeviceType();
//        if (deviceType.getDEVICE() == 0) {
            showVirtualPwd(0);
//            return;
//        }
//        mSelectDialog.show();
    }

    private void showVirtualPwd(int type) {
        if (type == 0) {
            mJubiter.showVirtualPwd(mBtcContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("showVirtualPwd success");
                    new InputDialog(mContext, new InputDialog.callback() {
                        @Override
                        public void onClickListener(String value) {
                            if (TextUtils.isEmpty(value)) {
                                return;
                            }
                            mJubiter.verifyPIN(mBtcContextID, value, new JubCallback<Void>() {
                                @Override
                                public void onSuccess(Void aVoid) {
                                    showLog("verifyPIN success");
                                    executeTrans();
                                }

                                @Override
                                public void onFailed(long errorCode) {
                                    showLog("verifyPIN " + errorCode);
                                }
                            });
                        }

                        @Override
                        public void onCancel() {
                            mJubiter.cancelVirtualPwd(mBtcContextID, null);
                        }
                    }).show();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("showVirtualPwd " + errorCode);
                }
            });
        } else {
            mJubiter.verifyFingerprint(mBtcContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("verifyFingerprint success");
                    executeTrans();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("verifyFingerprint " + errorCode);
                }
            });
        }
    }

    private void executeTrans() {
        mDialog.setMessage("BTC Transaction in progress....");
        mDialog.show();
        int decimal = getDecimal();
        mJubiter.btcTransaction(mBtcContextID, mTransType, decimal, transferInputValue, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                mDialog.dismiss();
                showLog(s);
            }

            @Override
            public void onFailed(long errorCode) {
                mDialog.dismiss();
                showLog("" + errorCode);
            }
        });
    }

    private int getDecimal() {
        int decimal = 8;
        if (mTransType != JubiterImpl.BTC_TransType.USDT) {
            switch (unitIndx) {
                case 0://BTC
                    break;
                case 1://cBTC
                    decimal = 6;
                    break;
                case 2://mBTC
                    decimal = 5;
                    break;
                case 3://uBTC
                    decimal = 2;
                    break;
                case 4://sat
                    decimal = 0;
                    break;
            }
        }
        return decimal;
    }

    private void setMyAddress() {
        mJubiter.showVirtualPwd(mBtcContextID, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("showVirtualPwd success");
                new InputDialog(mContext, new InputDialog.callback() {
                    @Override
                    public void onClickListener(String value) {
                        if (TextUtils.isEmpty(value)) {
                            return;
                        }
                        mJubiter.verifyPIN(mBtcContextID, value, new JubCallback<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                showLog("verifyPIN success");
                                mJubiter.btcSetMyAddress(mBtcContextID, new JubCallback<String>() {
                                    @Override
                                    public void onSuccess(String s) {
                                        showLog("btcSetMyAddress " + s);
                                    }

                                    @Override
                                    public void onFailed(long errorCode) {
                                        showLog("btcSetMyAddress " + errorCode);
                                    }
                                });
                            }

                            @Override
                            public void onFailed(long errorCode) {
                                showLog("verifyPIN " + errorCode);
                            }
                        });
                    }

                    @Override
                    public void onCancel() {
                        mJubiter.cancelVirtualPwd(mBtcContextID, null);
                    }
                }).show();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("showVirtualPwd " + errorCode);
            }
        });
    }

    private void showAddress() {
        mJubiter.btcShowAddress(mBtcContextID, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("btcShowAddress " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("btcShowAddress " + errorCode);
            }
        });
    }

    private void getAddress() {
        new PathDialog(mContext, "Input a path address_index", new PathDialog.callback() {
            @Override
            public void onClickListener(String value) {
                if (TextUtils.isEmpty(value)) {
                    return;
                }
                mJubiter.btcGetAddress(mBtcContextID, Integer.parseInt(value), new JubCallback<String>() {
                    @Override
                    public void onSuccess(String s) {
                        showLog(s);
                    }

                    @Override
                    public void onFailed(long errorCode) {
                        showLog(errorCode + "");
                    }
                });
            }

            @Override
            public void onCancel() {

            }
        }).show();
    }

    private void setTimeout() {
        new InputDialog(mContext, "Must be a number and less than 600.", new InputDialog.callback() {
            @Override
            public void onClickListener(String value) {
                if (TextUtils.isEmpty(value)) {
                    return;
                }
                int valueInt = Integer.parseInt(value);
                if (valueInt < 600 && valueInt > 0) {
                    mJubiter.setTimeout(valueInt, new JubCallback<Void>() {
                        @Override
                        public void onSuccess(Void aVoid) {
                            showLog("setTimeout success");
                        }

                        @Override
                        public void onFailed(long errorCode) {
                            showLog("setTimeout " + errorCode);
                        }
                    });
                }
            }

            @Override
            public void onCancel() {

            }
        }).show();
    }


    void showLog(final String tip) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Utils.parseLog(tip, mScrollView, mTxtLog);
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mBtcContextID != -1) {
            mJubiter.clearContext(mBtcContextID, null);
        }
    }
}