package com.jubiter.sdk.example;

import android.content.Context;
import android.os.Bundle;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.jubiter.sdk.example.dialog.InputDialog;
import com.jubiter.sdk.example.dialog.PathDialog;
import com.jubiter.sdk.example.dialog.SelectDialog;
import com.jubiter.sdk.example.utils.DecimalDigitsInputFilter;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.Utils;

import java.util.ArrayList;

public class TRXActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;

    private Context mContext;
    private JubiterImpl mJubiter;
    private Integer mTRXhContextID = -1;
    private String transferInputValue;
    private SelectDialog mSelectDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_trx);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
        createTRXContext();
    }

    private void createTRXContext() {
        mJubiter.trxCreateContext(new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("trxCreateContext success " + integer);
                mTRXhContextID = integer;
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("trxCreateContext error" + errorCode);
            }
        });
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);

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
            case R.id.trx_trans:
                transfer();
                break;
            case R.id.trx_get_address:
                getAddress();
                break;
            case R.id.trx_show_address:
                showAddress();
                break;
            case R.id.trx_set_my_address:
                setMyAddress();
                break;
            case R.id.trx_set_timeout:
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
        })
                .setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL)
                .setFilters(new InputFilter[]{new DecimalDigitsInputFilter(6)});

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
            mJubiter.showVirtualPwd(mTRXhContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("showVirtualPwd success");
                    new InputDialog(mContext, new InputDialog.callback() {
                        @Override
                        public void onClickListener(String value) {
                            if (TextUtils.isEmpty(value)) {
                                return;
                            }
                            mJubiter.verifyPIN(mTRXhContextID, value, new JubCallback<Void>() {
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
                            mJubiter.cancelVirtualPwd(mTRXhContextID, null);
                        }
                    }).show();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("showVirtualPwd " + errorCode);
                }
            });
        } else {
            mJubiter.verifyFingerprint(mTRXhContextID, new JubCallback<Void>() {
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
        mJubiter.trxTransaction(mTRXhContextID, transferInputValue, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("trxTransaction " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("trxTransaction " + errorCode);
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
        mJubiter.showVirtualPwd(mTRXhContextID, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("showVirtualPwd success");
                new InputDialog(mContext, new InputDialog.callback() {
                    @Override
                    public void onClickListener(String value) {
                        if (TextUtils.isEmpty(value)) {
                            return;
                        }
                        mJubiter.verifyPIN(mTRXhContextID, value, new JubCallback<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                showLog("verifyPIN success");
                                mJubiter.trxSetMyAddress(mTRXhContextID, new JubCallback<String>() {
                                    @Override
                                    public void onSuccess(String s) {
                                        showLog("trxSetMyAddress " + s);
                                    }

                                    @Override
                                    public void onFailed(long errorCode) {
                                        showLog("trxSetMyAddress " + errorCode);
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
                        mJubiter.cancelVirtualPwd(mTRXhContextID, null);
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
        mJubiter.trxShowAddress(mTRXhContextID, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("trxShowAddress " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("trxShowAddress " + errorCode);
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
                mJubiter.trxGetAddress(mTRXhContextID, Integer.parseInt(value), new JubCallback<String>() {
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
        if (mTRXhContextID != -1) {
            mJubiter.clearContext(mTRXhContextID, null);
        }
    }
}