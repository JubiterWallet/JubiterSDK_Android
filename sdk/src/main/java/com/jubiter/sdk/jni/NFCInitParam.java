package com.jubiter.sdk.jni;

import android.content.Context;
import android.nfc.Tag;

/**
 * @author fengshuo
 * @date 2020/9/10
 * @time 14:02
 */
public class NFCInitParam {
    public Context mContext;
    public Tag mTag;

    public NFCInitParam(Context context, Tag tag) {
        mContext = context;
        mTag = tag;
    }
}
