package com.jubiter.sdk.example.utils;

/**
 * @Date 2018/5/18  13:21
 * @Author ZJF
 * @Version 1.0
 */
public interface JubCallback<V> {

    void onSuccess(V v);

    void onFailed(long errorCode);
}

