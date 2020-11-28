package com.jubiter.sdk.example;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.jubiter.sdk.example.dialog.InputDialog;
import com.jubiter.sdk.example.dialog.SelectDialog;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.SelectAdapter;
import com.jubiter.sdk.example.utils.Utils;

import java.util.ArrayList;
import java.util.List;

public class FingerPrintManagerActivity extends AppCompatActivity {

    private ScrollView mScrollView;
    private TextView mTxtLog;
    private Context mContext;
    private List<String> mFingerprintList, mVerifyTypeList;
    private SelectAdapter mSelectAdapter;
    private JubiterImpl mJubiter;
    private SelectDialog mSelectDialog;
    private ProgressDialog mDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_finger_print_manager);
        mContext = this;
        mFingerprintList = new ArrayList<>();
        mVerifyTypeList = new ArrayList<>();
        mVerifyTypeList.add("via device");
        mVerifyTypeList.add("via 9 Grids");
        mVerifyTypeList.add("via APDU");
        mVerifyTypeList.add("via fingerprint");
        initView();
        mJubiter = JubiterImpl.getInstance(mContext);
        mDialog = new ProgressDialog(this);
        checkBootLoader();
    }

    private void initView() {
        ListView mListView = findViewById(R.id.list_finger);
        mScrollView = findViewById(R.id.scrollView);
        mTxtLog = findViewById(R.id.txt_log);

        mSelectAdapter = new SelectAdapter(mContext, mFingerprintList);
        mListView.setAdapter(mSelectAdapter);
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                deleteFingerprint(new Byte(mFingerprintList.get(i)));
            }
        });

        mSelectDialog = new SelectDialog()
                .init(mContext, mVerifyTypeList)
                .setCancelable(false)
                .setTitle("Please select mode:")
                .setOnItemClickListener(new AdapterView.OnItemClickListener() {
                    @Override
                    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                        onVerify(i);
                        mSelectDialog.dismiss();
                    }
                });
    }

    private void checkBootLoader() {
        mJubiter.isBootLoader(new JubCallback<Boolean>() {
            @Override
            public void onSuccess(Boolean result) {
                showLog("isBootLoader true");
                enumFinger();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("isBootLoader false");
                enumFinger();
            }
        });
    }


    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.delete:
                mTxtLog.setText("");
                break;
            case R.id.btc_enroll:
                enroll();
                break;
            case R.id.txt_verify:
                showVerifyTypeDialog();
                break;
            case R.id.btc_erase_all:
                eraseAll();
                break;
            default:
                break;
        }
    }

    private void eraseAll() {
        mJubiter.eraseFingerprint(new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("eraseFingerprint success");
                enumFinger();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("eraseFingerprint " + errorCode);
            }
        });
    }

    private void enroll() {
        mJubiter.enrollFingerprint(new JubCallback<String>() {
            @Override
            public void onSuccess(String s) {
                if (TextUtils.isEmpty(s)) {
                    showLog("enrollFingerprint success");
                    enumFinger();
                } else {
                    // showLog("FgpNextIndex:" + aByte + " " + s);
                }
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("enrollFingerprint " + errorCode);
            }
        });
    }

    private void enumFinger() {
        mJubiter.enumFingerprint(new JubCallback<List<String>>() {
            @Override
            public void onSuccess(List<String> list) {
                showLog("enumFingerprint " + list);
                mSelectAdapter.clean();
                for (String item : list) {
                    mSelectAdapter.addItem(item);
                }
                mSelectAdapter.notifyDataSetChanged();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("enumFingerprint " + errorCode);
            }
        });
    }

    private void showVerifyTypeDialog() {
        mSelectDialog.show();
    }


    private void onVerify(int i) {
        switch (i) {
            case 0:
                deviceVerify();
                break;
            case 1:
                nineGridsVerify();
                break;
            case 2:
                apduVerify();
                break;
            case 3:
                fingerprintVerify();
                break;
        }
    }

    private void deviceVerify() {
        showProgressDialog("Identity verification via device in progress");
        mJubiter.identityVerify(1, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                dismissProgressDialog();
                showLog("IdentityVerify success");
            }

            @Override
            public void onFailed(long errorCode) {
                dismissProgressDialog();
                showLog("IdentityVerify " + errorCode);
            }
        });
    }


    private void nineGridsVerify() {
        mJubiter.identityShowNineGrids(new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                showLog("identityShowNineGrids success");
                new InputDialog(mContext, new InputDialog.callback() {
                    @Override
                    public void onClickListener(String pin) {
                        mJubiter.identityVerifyPIN(2, pin, new JubCallback<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                showLog("identityVerifyPIN success");
                            }

                            @Override
                            public void onFailed(long errorCode) {
                                showLog("identityVerifyPIN " + errorCode);
                            }
                        });
                    }

                    @Override
                    public void onCancel() {
                        mJubiter.identityCancelNineGrids(new JubCallback<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                showLog("identityCancelNineGrids success");
                            }

                            @Override
                            public void onFailed(long errorCode) {
                                showLog("identityVerifyPIN " + errorCode);
                            }
                        });
                    }
                }).show();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("identityShowNineGrids " + errorCode);
            }
        });
    }

    private void apduVerify() {
        new InputDialog(mContext, new InputDialog.callback() {
            @Override
            public void onClickListener(String pin) {
                mJubiter.identityVerifyPIN(3, pin, new JubCallback<Void>() {
                    @Override
                    public void onSuccess(Void aVoid) {
                        showLog("identityVerifyPIN success");
                    }

                    @Override
                    public void onFailed(long errorCode) {
                        showLog("identityVerifyPIN " + errorCode);
                    }
                });
            }

            @Override
            public void onCancel() {

            }
        }).show();
    }


    private void fingerprintVerify() {
        showProgressDialog("Identity verification via fingerprint in progress");
        mJubiter.identityVerify(4, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void aVoid) {
                dismissProgressDialog();
                showLog("IdentityVerify success");
            }

            @Override
            public void onFailed(long errorCode) {
                dismissProgressDialog();
                showLog("IdentityVerify " + errorCode);
            }
        });
    }

    private void deleteFingerprint(Byte b) {
        mJubiter.deleteFingerprint(b, new JubCallback<Void>() {
            @Override
            public void onSuccess(Void s) {
                showLog("deleteFingerprint success");
                enumFinger();
            }

            @Override
            public void onFailed(long errorCode) {
                showLog("deleteFingerprint " + errorCode);
            }
        });
    }


    void showProgressDialog(String msg) {
        mDialog.setMessage(msg);
        mDialog.show();
    }

    void showProgressDialog() {
        mDialog.show();
    }

    void dismissProgressDialog() {
        mDialog.dismiss();
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