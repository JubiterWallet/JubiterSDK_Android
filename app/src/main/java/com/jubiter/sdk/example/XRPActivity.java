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
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;


import com.jubiter.sdk.example.dialog.InputDialog;
import com.jubiter.sdk.example.dialog.PathDialog;
import com.jubiter.sdk.example.dialog.SelectDialog;
import com.jubiter.sdk.example.utils.DecimalDigitsInputFilter;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.Utils;

import java.util.ArrayList;

public class XRPActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;

    private Context mContext;
    private JubiterImpl mJubiter;
    private Integer mXRPhContextID = -1;
    private String transferInputValue;
    private SelectDialog mSelectDialog;

    private ProgressDialog mProgress;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_xrp);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
        createXRPContext();
    }

    private void createXRPContext() {
        mProgress.setMessage("Create context in progress....");
        mProgress.show();
        mJubiter.xrpCreateContext(new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("xrpCreateContext success " + integer);
                mXRPhContextID = integer;
                mProgress.dismiss();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("xrpCreateContext error" + errorCode);
                mProgress.dismiss();
            }
        });
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);

        mProgress = new ProgressDialog(mContext);
        mProgress.setCancelable(false);

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
            case R.id.xrp_trans:
                transfer();
                break;
            case R.id.xrp_get_address:
                getAddress();
                break;
            case R.id.xrp_show_address:
                showAddress();
                break;
//            case R.id.xrp_set_my_address:
//                setMyAddress();
//                break;
            case R.id.xrp_set_timeout:
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
        JubiterImpl.DeviceType deviceType = mJubiter.getDeviceType();
        if (deviceType == JubiterImpl.DeviceType.BLE) {
            showVirtualPwd(0);
        } else if(deviceType == JubiterImpl.DeviceType.SWI){
            executeTrans();
        } else if (deviceType == JubiterImpl.DeviceType.BIO) {
            showVirtualPwd(1);
        }
    }

    private void showVirtualPwd(int type) {
        if (type == 0) {
            mJubiter.showVirtualPwd(mXRPhContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("showVirtualPwd success");
                    new InputDialog(mContext, new InputDialog.callback() {
                        @Override
                        public void onClickListener(String value) {
                            if (TextUtils.isEmpty(value)) {
                                return;
                            }
                            mJubiter.verifyPIN(mXRPhContextID, value, new JubCallback<Void>() {
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
                            mJubiter.cancelVirtualPwd(mXRPhContextID, null);
                        }
                    }).show();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("showVirtualPwd " + errorCode);
                }
            });
        } else {
            mJubiter.verifyFingerprint(mXRPhContextID, new JubCallback<Void>() {
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
        mJubiter.xrpTransaction(mXRPhContextID, transferInputValue, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("xrpTransaction " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("xrpTransaction " + errorCode);
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

//    private void setMyAddress() {
//        mJubiter.showVirtualPwd(mXRPhContextID, new JubCallback<Void>() {
//            @Override
//            public void onSuccess(Void aVoid) {
//                showLog("showVirtualPwd success");
//                new InputDialog(mContext, new InputDialog.callback() {
//                    @Override
//                    public void onClickListener(String value) {
//                        if (TextUtils.isEmpty(value)) {
//                            return;
//                        }
//                        mJubiter.verifyPIN(mXRPhContextID, value, new JubCallback<Void>() {
//                            @Override
//                            public void onSuccess(Void aVoid) {
//                                showLog("verifyPIN success");
//                                mJubiter.xrpSetMyAddress(mXRPhContextID, new JubCallback<String>() {
//                                    @Override
//                                    public void onSuccess(String s) {
//                                        showLog("xrpSetMyAddress " + s);
//                                    }
//
//                                    @Override
//                                    public void onFailed(long errorCode) {
//                                        showLog("xrpSetMyAddress " + errorCode);
//                                    }
//                                });
//                            }
//
//                            @Override
//                            public void onFailed(long errorCode) {
//                                showLog("verifyPIN " + errorCode);
//                            }
//                        });
//                    }
//
//                    @Override
//                    public void onCancel() {
//                        mJubiter.cancelVirtualPwd(mXRPhContextID, null);
//                    }
//                }).show();
//            }
//
//            @Override
//            public void onFailed(long errorCode) {
//                showLog("showVirtualPwd " + errorCode);
//            }
//        });
//    }

    private void showAddress() {
        mJubiter.xrpShowAddress(mXRPhContextID, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("xrpShowAddress " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("xrpShowAddress " + errorCode);
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
                mJubiter.xrpGetAddress(mXRPhContextID, Integer.parseInt(value), new JubCallback<String>() {
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
        if (mXRPhContextID != -1) {
            mJubiter.clearContext(mXRPhContextID, null);
        }
    }
}