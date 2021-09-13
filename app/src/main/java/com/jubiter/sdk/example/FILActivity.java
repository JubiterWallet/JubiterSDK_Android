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

public class FILActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;
    private Spinner mSpinner;

    private Context mContext;
    private JubiterImpl mJubiter;
    private Integer mFilContextID = -1;
    private String transferInputValue;
    private JubiterImpl.FIL_TransType mTransType;
    private SelectDialog mSelectDialog;
    private ProgressDialog mProgress;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fil);
        mContext = this;
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
        createFILContext();
    }

    private void createFILContext() {
        mProgress.setMessage("Create context in progress....");
        mProgress.show();
        mJubiter.filCreateContext(new JubCallback<Integer>() {
            @Override
            public void onSuccess(Integer integer) {
                showLog("filCreateContext success " + integer);
                mFilContextID = integer;
                mProgress.dismiss();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("filCreateContext error" + errorCode);
                mProgress.dismiss();
            }
        });
    }

    private void initView() {
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);
        mSpinner = findViewById(R.id.spinner_fil);
        mSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                switch (i) {
                    case 0:
                        mTransType = JubiterImpl.FIL_TransType.FIL;
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
        mProgress = new ProgressDialog(mContext);
        mProgress.setCancelable(false);
    }

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.delete:
                mTxtLog.setText("");
                break;
            case R.id.fil_trans:
                transfer();
                break;
            case R.id.fil_get_address:
                getAddress();
                break;
            case R.id.fil_show_address:
                showAddress();
                break;
//            case R.id.fil_set_my_address:
//                setMyAddress();
//                break;
            case R.id.fil_set_timeout:
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
                .setFilters(new InputFilter[]{new DecimalDigitsInputFilter(18)});

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
            mJubiter.showVirtualPwd(mFilContextID, new JubCallback<Void>() {
                @Override
                public void onSuccess(Void aVoid) {
                    showLog("showVirtualPwd success");
                    new InputDialog(mContext, new InputDialog.callback() {
                        @Override
                        public void onClickListener(String value) {
                            if (TextUtils.isEmpty(value)) {
                                return;
                            }
                            mJubiter.verifyPIN(mFilContextID, value, new JubCallback<Void>() {
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
                            mJubiter.cancelVirtualPwd(mFilContextID, null);
                        }
                    }).show();
                }

                @Override
                public void onFailed(long errorCode) {
                    showLog("showVirtualPwd " + errorCode);
                }
            });
        } else {
            mJubiter.verifyFingerprint(mFilContextID, new JubCallback<Void>() {
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
        mJubiter.filTransaction(mFilContextID, mTransType, transferInputValue, new JubCallback<String>() {
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

//    private void setMyAddress() {
//        mJubiter.showVirtualPwd(mFilContextID, new JubCallback<Void>() {
//            @Override
//            public void onSuccess(Void aVoid) {
//                showLog("showVirtualPwd success");
//                new InputDialog(mContext, new InputDialog.callback() {
//                    @Override
//                    public void onClickListener(String value) {
//                        if (TextUtils.isEmpty(value)) {
//                            return;
//                        }
//                        mJubiter.verifyPIN(mFilContextID, value, new JubCallback<Void>() {
//                            @Override
//                            public void onSuccess(Void aVoid) {
//                                showLog("verifyPIN success");
//                                mJubiter.filSetMyAddress(mFilContextID, new JubCallback<String>() {
//                                    @Override
//                                    public void onSuccess(String s) {
//                                        showLog("filSetMyAddress " + s);
//                                    }
//
//                                    @Override
//                                    public void onFailed(long errorCode) {
//                                        showLog("filSetMyAddress " + errorCode);
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
//                        mJubiter.cancelVirtualPwd(mFilContextID, null);
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
        mJubiter.filShowAddress(mFilContextID, new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                showLog("filShowAddress " + s);
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("filShowAddress " + errorCode);
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
                mJubiter.filGetAddress(mFilContextID, Integer.parseInt(value), new JubCallback<String>() {
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
        if (mFilContextID != -1) {
            mJubiter.clearContext(mFilContextID, null);
        }
    }
}