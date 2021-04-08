package com.jubiter.sdk.example.utils;

import android.os.Handler;
import android.text.Html;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

import java.text.SimpleDateFormat;
import java.util.Date;

public class Utils {

    public static final SimpleDateFormat mFormat = new SimpleDateFormat("yyyy-MM-dd   hh:mm:ss");

    public static void scrollBottom(final ScrollView scroll, final View inner) {
        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                if (scroll == null || inner == null) {
                    return;
                }
                // 内层高度超过外层
                int offset = inner.getMeasuredHeight()
                        - scroll.getMeasuredHeight();
                if (offset < 0) {
                    offset = 0;
                }
                scroll.scrollTo(0, offset);
            }
        });
    }


    public static void parseLog(String s, ScrollView scrollView, TextView logView) {
        long currentTimeMillis = System.currentTimeMillis();
        logView.append("Time:\t" + mFormat.format(new Date(currentTimeMillis)) + "\n");
        logView.append(Html.fromHtml("<p><font color=\"#85d46f\">" + s + "</p>"));
        scrollBottom(scrollView, logView);
    }
}
