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

import java.util.ArrayList;

public class EOSActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;
    private Spinner mSpinner;

    private Context mContext;
    private JubiterImpl mJubiter;
    private Integer mEosContextID = -1;
    private String transferInputValue;
    private JubiterImpl.EOS_TransType mTransType;
    private SelectDialog mSelectDialog;
    private ProgressDialog mDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_eos);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
        createEOSContext();
    }

    private void createEOSContext() {
        mDialog.setMessage("Create context in progress....");
        mDialog.show();
        mJubiter.eosCreateContext(new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("eosCreateContext success " + integer);
                mEosContextID = integer;
                mDialog.dismiss();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("eosCreateContext error" + errorCode);
                mDialog.dismiss();
            }
        });
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);
        mSpinner = findViewById(R.id.spinner_eos);
        mDialog = new ProgressDialog(this);
        mSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                switch (i) {
                    case 0:
                        mTransType = JubiterImpl.EOS_TransType.EOS;
                        break;
                    case 1:
                        mTransType = JubiterImpl.EOS_TransType.EOS_TOKEN;
                        break;
                    case 2:
                        mTransType = JubiterImpl.EOS_TransType.EOS_BUY_RAM;
                        break;
                    case 3:
                        mTransType = JubiterImpl.EOS_TransType.EOS_SELL_RAM;
                        break;
                    case 4:
                        mTransType = JubiterImpl.EOS_TransType.EOS_STAKE;
                        break;
                    case 5:
                        mTransType = JubiterImpl.EOS_TransType.EOS_UNSTAKE;
                        break;
                }
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
    }

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.delete:
                mTxtLog.setText("");
                break;
            case R.id.eos_trans:
                transfer();
                break;
            case R.id.eos_get_address:
                getAddress();
                break;
            case R.id.eos_show_address:
                showAddress();
                break;
            case R.id.eos_set_my_address:
                setMyAddress();
                break;
            case R.id.eos_set_timeout:
                setTimeout();
                break;
            default:
                break;
        }
    }

    private void transfer() {
        InputDialog inputDialog = new InputDialog(mContext, "Please input transaction value", new InputDialog.callback() {
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
        });
        if (mTransType == JubiterImpl.EOS_TransType.EOS_SELL_RAM) {
            inputDialog.setFilters(new InputFilter[]{new DecimalDigitsInputFilter(0)});
            inputDialog.setInputType(InputType.TYPE_CLASS_NUMBER);
        } else {
            inputDialog.setFilters(new InputFilter[]{new DecimalDigitsInputFilter(4)});
            inputDialog.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL);
        }
        inputDialog.show();
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
            mJubiter.showVirtualPwd(mEosContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("showVirtualPwd success");
                    new InputDialog(mContext, new InputDialog.callback() {
                        @Override
                        public void onClickListener(String value) {
                            if (TextUtils.isEmpty(value)) {
                                return;
                            }
                            mJubiter.verifyPIN(mEosContextID, value, new JubCallback<Void>() {
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
                            mJubiter.cancelVirtualPwd(mEosContextID, null);
                        }
                    }).show();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("showVirtualPwd " + errorCode);
                }
            });
        } else {
            mJubiter.verifyFingerprint(mEosContextID, new JubCallback<Void>() {
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
        mJubiter.eosTransaction(mEosContextID, mTransType, transferInputValue, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog(s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("" + errorCode);
            }
        });
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

    private void setMyAddress() {
        mJubiter.showVirtualPwd(mEosContextID, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("showVirtualPwd success");
                new InputDialog(mContext, new InputDialog.callback() {
                    @Override
                    public void onClickListener(String value) {
                        if (TextUtils.isEmpty(value)) {
                            return;
                        }
                        mJubiter.verifyPIN(mEosContextID, value, new JubCallback<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                showLog("verifyPIN success");
                                mJubiter.eosSetMyAddress(mEosContextID, new JubCallback<String>() {
                                    @Override
                                    public void onSuccess(String s) {
                                        showLog("eosSetMyAddress " + s);
                                    }

                                    @Override
                                    public void onFailed(long errorCode) {
                                        showLog("eosShowAddress " + errorCode);
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
                        mJubiter.cancelVirtualPwd(mEosContextID, null);
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
        mJubiter.eosShowAddress(mEosContextID, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("eosShowAddress " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("eosShowAddress " + errorCode);
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
                mJubiter.eosGetAddress(mEosContextID, Integer.parseInt(value), new JubCallback<String>() {
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
        if (mEosContextID != -1) {
            mJubiter.clearContext(mEosContextID, null);
        }
    }
}
