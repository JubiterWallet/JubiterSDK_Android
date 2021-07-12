package com.jubiter.sdk.example.net;


import androidx.annotation.StringDef;

import java.io.IOException;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.util.concurrent.TimeUnit;

import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


public class RetrofitManager {

    @StringDef({RELEASE_SERVER.BTC_SERVICE, RELEASE_SERVER.ETH_SERVICE})
    @Retention(RetentionPolicy.SOURCE)
    public @interface RELEASE_SERVER {
        public static final String BTC_SERVICE = "https://wxotp.ftsafe.cn:3003";
        public static final String ETH_SERVICE = "https://wxotp.ftsafe.cn";
    }

    @StringDef({DEBUG_SERVER.BTC_DEBUG, DEBUG_SERVER.ETH_DEBUG})
    @Retention(RetentionPolicy.SOURCE)
    public @interface DEBUG_SERVER {
        public static final String BTC_DEBUG = "http://192.168.22.98:3003";
        public static final String ETH_DEBUG = "http://192.168.22.98:3009";
    }

    private static final long CONNECT_TIMEOUT = 40L;
    private static final long READ_TIMEOUT = 60L;
    private static final long WRITE_TIMEOUT = 60L;


    private static final Interceptor mLogInterceptor = new Interceptor() {
        @Override
        public Response intercept(Chain chain) throws IOException {
            Request original = chain.request();
            // 自定义请求头
            Request.Builder requestBuilder = original.newBuilder()
                    .addHeader("Connection", "close");
            Request request = requestBuilder.build();

            Response response = chain.proceed(request);
            return response;
        }
    };

    /**
     * 获取 OkHttpClient 实例
     *
     * @return
     */
    private static OkHttpClient getsOkHttpClient() {
        return SingletonHolder.sOkHttpClient;
    }

    private static class SingletonHolder {
        private static volatile OkHttpClient sOkHttpClient = new OkHttpClient.Builder()
                .connectTimeout(CONNECT_TIMEOUT, TimeUnit.SECONDS)
                .readTimeout(READ_TIMEOUT, TimeUnit.SECONDS)
                .writeTimeout(WRITE_TIMEOUT, TimeUnit.SECONDS)
                .addInterceptor(mLogInterceptor)
                .retryOnConnectionFailure(true)
                .build();
    }

    public static <T> T createBTC(Class<T> clazz) {
        Retrofit retrofit = new Retrofit.Builder().baseUrl(RELEASE_SERVER.BTC_SERVICE)
                .client(getsOkHttpClient())
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        return retrofit.create(clazz);
    }

    public static <T> T createETH(Class<T> clazz) {
        Retrofit retrofit = new Retrofit.Builder().baseUrl(RELEASE_SERVER.ETH_SERVICE)
                .client(getsOkHttpClient())
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        return retrofit.create(clazz);
    }
}
