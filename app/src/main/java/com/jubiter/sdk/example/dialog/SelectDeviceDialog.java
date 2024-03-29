package com.jubiter.sdk.example.dialog;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnShowListener;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;


import com.jubiter.sdk.JuBiterBLEDevice;
import com.jubiter.sdk.JuBiterWallet;
import com.jubiter.sdk.ScanResultCallback;
import com.jubiter.sdk.example.R;
import com.jubiter.sdk.example.utils.ThreadUtils;

import java.util.ArrayList;
import java.util.List;

public class SelectDeviceDialog extends Dialog implements View.OnClickListener, OnItemClickListener, OnShowListener, OnCancelListener {

    public static final int TYPE_BLUETOOTH_3 = 1;
    public static final int TYPE_BLUETOOTH_4 = 2;

    // Debugging
    protected static final String TAG = SelectDeviceDialog.class.getSimpleName();
    protected static final boolean D = true;

    private TextView tvTitle;
    private ProgressBar progressBar;
    private ListView mListView;
    private Button btnClose;
    private Button btnRrfresh;

    private OnSelectedListener mOnSelectedListener;

    private static BtDeviceAdapter mDeviceListAdapter;
    private List<JuBiterBLEDevice> mDeviceNameList;


    public SelectDeviceDialog(Context context,
                              OnSelectedListener selectedListener) {
        super(context);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        intiUI(context);


        mDeviceNameList = new ArrayList<>();
        this.mOnSelectedListener = selectedListener;

        mDeviceListAdapter = new BtDeviceAdapter(context, mDeviceNameList);
        mListView.setAdapter(mDeviceListAdapter);
        mListView.setOnItemClickListener(this);
        setCancelable(false);
        this.setOnShowListener(this);
        this.setOnCancelListener(this);
    }

    private void intiUI(Context context) {

        ViewGroup.LayoutParams lp1 = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        ViewGroup.LayoutParams lp2 = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT);
        LinearLayout.LayoutParams lp5 = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1);
        lp5.setMargins(10, 10, 10, 10);
        LinearLayout.LayoutParams lp6 = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1);


        // ========================= 顶部布局 =========================

        LinearLayout titleLayout = new LinearLayout(context);
        titleLayout.setOrientation(LinearLayout.HORIZONTAL);
        titleLayout.setLayoutParams(lp1);

        tvTitle = new TextView(context);
        tvTitle.setText("选择设备");
        tvTitle.setTextColor(getContext().getResources().getColor(R.color.colorPrimaryDark));
        tvTitle.setTextSize(22);

        progressBar = new ProgressBar(context);
        progressBar.setLayoutParams(lp2);
        progressBar.setVisibility(View.GONE);

        titleLayout.addView(tvTitle);
        titleLayout.addView(progressBar);


        // ========================= 底部布局 =========================

        LinearLayout buttonsLayout = new LinearLayout(context);
        buttonsLayout.setOrientation(LinearLayout.HORIZONTAL);
        buttonsLayout.setLayoutParams(lp1);
        btnClose = new Button(context);
        btnRrfresh = new Button(context);


        btnClose.setLayoutParams(lp5);
        btnRrfresh.setLayoutParams(lp5);

        btnClose.setText("关闭");
        btnClose.setTextColor(Color.WHITE);
        btnClose.setBackgroundResource(R.drawable.bg_btn);

        btnRrfresh.setText("刷新");
        btnRrfresh.setTextColor(Color.WHITE);
        btnRrfresh.setBackgroundResource(R.drawable.bg_btn);

        btnClose.setOnClickListener(this);
        btnRrfresh.setOnClickListener(this);

        buttonsLayout.addView(btnClose);
        buttonsLayout.addView(btnRrfresh);

        // ===================== 中部的 ListView 布局 =================

        mListView = new ListView(context);
        mListView.setLayoutParams(lp6);
        mListView.setHeaderDividersEnabled(true);
        mListView.setFooterDividersEnabled(true);

        // ===================== 根布局 ==============================

        LinearLayout rootLayout = new LinearLayout(context);
        rootLayout.setOrientation(LinearLayout.VERTICAL);
        rootLayout.setBackgroundResource(R.drawable.setbar_bg);
        rootLayout.addView(titleLayout);
        rootLayout.addView(mListView);
        rootLayout.addView(buttonsLayout);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(rootLayout);
    }

    public interface OnSelectedListener {
        void onSelected(JuBiterBLEDevice device);
    }


    public ScanResultCallback callback = new ScanResultCallback() {
        @Override
        public void onScanResult(JuBiterBLEDevice device) {
            mDeviceListAdapter.addItem(device);
            mDeviceListAdapter.notifyDataSetChanged();
        }

        @Override
        public void onStop() {

        }

        @Override
        public void onError(int errorCode) {

        }
    };



    @Override
    public void onClick(View v) {
        if (v == btnClose) {
            this.cancel();
            mOnSelectedListener.onSelected(null);
        } else if (v == btnRrfresh) {
            JuBiterWallet.stopScan();
            mDeviceNameList.clear();
            mDeviceListAdapter.notifyDataSetChanged();
            JuBiterWallet.startScan(callback);
        }
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
        SelectDeviceDialog.this.cancel();
        //先停止扫描，再进行下一步操作。解决华为手机连接慢的问题
        JuBiterWallet.stopScan();
        ThreadUtils.toMainThread(new Runnable() {
            @Override
            public void run() {
                mOnSelectedListener.onSelected(mDeviceListAdapter.getItem(position));
            }
        });
    }

    @Override
    public void onShow(DialogInterface dialog) {
        JuBiterWallet.startScan(callback);
        progressBar.setVisibility(View.VISIBLE);
    }


    @Override
    public void onCancel(DialogInterface dialog) {
        JuBiterWallet.stopScan();
    }
}