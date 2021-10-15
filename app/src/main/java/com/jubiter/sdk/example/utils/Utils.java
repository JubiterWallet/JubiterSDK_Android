package com.jubiter.sdk.example.utils;

import android.os.Handler;
import android.text.Html;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonParser;

import java.text.SimpleDateFormat;
import java.util.Date;

import io.github.novacrypto.base58.BadCharacterException;
import io.github.novacrypto.base58.Base58;

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


//    private static final String LINE_SEPARATOR = System.getProperty("line.separator");
//
//    public static String formatJson(String msg) {
//
//        String message;
//
//        try {
//            if (msg.startsWith("{")) {
//                JSONObject jsonObject = new JSONObject(msg);
//                message = jsonObject.toString(4);//最重要的方法，就一行，返回格式化的json字符串，其中的数字4是缩进字符数
//            } else if (msg.startsWith("[")) {
//                JSONArray jsonArray = new JSONArray(msg);
//                message = jsonArray.toString(4);
//            } else {
//                message = msg;
//            }
//        } catch (JSONException e) {
//            message = msg;
//        }
//        StringBuilder stringBuilder = new StringBuilder();
//        message = LINE_SEPARATOR + message;
//        String[] lines = message.split(LINE_SEPARATOR);
//        for (String line : lines) {
//            stringBuilder.append(line).append("\n");
//        }
//        return stringBuilder.toString();
//    }

    public static String formatJson(String uglyJSONString){
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        JsonParser jp = new JsonParser();
        JsonElement je = jp.parse(uglyJSONString);
        return gson.toJson(je);
    }

    public static String xpub2tpub(String xpub){
        try {
            byte[] decode = Base58.base58Decode(xpub);
            String re = HexUtils.convertBytesToString(decode);
            if(re.toUpperCase().startsWith("0488B21E")){
                re = re.replace("0488b21e","043587cf");
                return Base58.base58Encode(HexUtils.fromString(re));
            } else {
                return xpub;
            }
        } catch (BadCharacterException e) {
            e.printStackTrace();
        }
        return xpub;
    }
}
