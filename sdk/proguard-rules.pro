# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile




# natvie 方法不混淆
-keepclasseswithmembernames class * {
    native <methods>;
}

-keep class com.jubiter.sdk.jni.InnerDiscCallback{*;}
-keep class com.jubiter.sdk.jni.InnerScanCallback{*;}
-keep class com.jubiter.sdk.jni.NativeApi{*;}
-keep class com.jubiter.sdk.jni.utils.JUB_DEVICE_INFO{*;}

-keep class com.ftsafe.bluetooth.key.ckey.FTBtKeyNativeApi{*;}
-keep class com.ftsafe.bluetooth.key.ckey.FTBluetoothKey{*;}


