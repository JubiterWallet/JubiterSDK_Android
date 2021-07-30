package com.jubiter.sdk.example;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import com.google.gson.Gson;
import com.google.protobuf.Any;
import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.ConnectionStateCallback;
import com.jubiter.sdk.JuBiterBLEDevice;
import com.jubiter.sdk.JuBiterBitcoin;
import com.jubiter.sdk.JuBiterEOS;
import com.jubiter.sdk.JuBiterEthereum;
import com.jubiter.sdk.JuBiterSWIWallet;
import com.jubiter.sdk.JuBiterFilecoin;
import com.jubiter.sdk.JuBiterTRX;
import com.jubiter.sdk.JuBiterWallet;
import com.jubiter.sdk.JuBiterXRP;
import com.jubiter.sdk.example.dialog.SelectDeviceDialog;
import com.jubiter.sdk.example.net.bean.ethhistory.EthHistory;
import com.jubiter.sdk.example.net.bean.ethinfo.EthAccountInfo;
import com.jubiter.sdk.example.net.btc.BtcModel;
import com.jubiter.sdk.example.net.bean.SimpleBean;
import com.jubiter.sdk.example.net.bean.fee.Fees;
import com.jubiter.sdk.example.net.bean.btchisory.BtcHistory;
import com.jubiter.sdk.example.net.bean.btctransaction.Input;
import com.jubiter.sdk.example.net.bean.btctransaction.Output;
import com.jubiter.sdk.example.net.bean.btctransaction.PreTransaction;
import com.jubiter.sdk.example.net.bean.btctransaction.PreTransactionBean;
import com.jubiter.sdk.example.net.bean.btctransaction.Prevout;
import com.jubiter.sdk.example.net.bean.txid.TxStatus;
import com.jubiter.sdk.example.net.eth.EthModel;
import com.jubiter.sdk.example.utils.HexUtils;
import com.jubiter.sdk.example.utils.JubCallback;
import com.jubiter.sdk.example.utils.ThreadUtils;
import com.jubiter.sdk.example.utils.Utils;
import com.jubiter.sdk.proto.BitcoinProtos;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EOSProtos;
import com.jubiter.sdk.proto.EthereumProtos;
import com.jubiter.sdk.proto.FilecoinProtos;
import com.jubiter.sdk.proto.RippleProtos;

import org.tron.protos.Protocol;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


/**
 * @Date 2018/4/19  11:30
 * @Author ZJF
 * @Version 1.0
 */
public class JubiterImpl {

    private String TAG = "JubiterImpl";
    private DeviceType mDeviceType;

    public enum BTC_TransType {
        BTC_TEST, BTC_P2PKH, BTC_P2WPKH, LTC, DASH, BCH, QTUM, USDT
    }

    public enum EOS_TransType {
        EOS, EOS_TOKEN, EOS_BUY_RAM, EOS_SELL_RAM, EOS_STAKE, EOS_UNSTAKE
    }

    public enum ETH_TransType {
        ETH_TEST, ETH, ETH_ERC20, ETH_ERC721
    }

    public enum TRX_TransType {
        TRX, TRC10, TRCFree, TRCUnfreeze, TRC20, TRC20_TRANSFER, TRC721
    }

    public enum DeviceType {
        BLE, BIO, SWI, NFC
    }

    public enum FIL_TransType {
        FIL
    }

    private Context mContext;

    private int deviceHandle;


    private JubiterImpl(Context context) {
        mContext = context;
        JuBiterWallet.initDevice();
    }

    private static JubiterImpl mImpl;

    public static JubiterImpl getInstance(Context context) {
        if (mImpl == null) {
            mImpl = new JubiterImpl(context);
        }
        return mImpl;
    }

    public void connectBt(final ConnectionStateCallback callback) {
        SelectDeviceDialog dialog = new SelectDeviceDialog(mContext, new SelectDeviceDialog.OnSelectedListener() {
            @Override
            public void onSelected(final JuBiterBLEDevice device) {
                if (device == null) {
                    return;
                }
                JuBiterWallet.connectDeviceAsync(device.getName(), device.getMac(), 30 * 1000, new ConnectionStateCallback() {
                    @Override
                    public void onConnected(final String mac, final int handle) {
                        deviceHandle = handle;
                        mDeviceType = DeviceType.BLE;
                        callback.onConnected(mac, handle);
                    }

                    @Override
                    public void onDisconnected(final String mac) {
                        callback.onDisconnected(mac);
                    }

                    @Override
                    public void onError(final int error) {
                        callback.onError(error);

                    }
                });
            }
        });
        dialog.show();
    }

    public int disConnectBt() {
        return JuBiterWallet.disconnectDevice(deviceHandle);
    }

    public void connectSwi() {
        CommonProtos.ResultInt resultInt = JuBiterSWIWallet.swiConnectDevice();
        deviceHandle = resultInt.getValue();
        mDeviceType = DeviceType.SWI;
        JuBiterSWIWallet.swiBuildFromMnemonic(deviceHandle,
                "", "convince stove grab pink educate chapter march hood neglect sphere walnut unfair");
    }

    public void disconnectSwi() {
        JuBiterSWIWallet.swiDisconnectDevice(deviceHandle);
    }

    public void queryBattery(final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultInt result = JuBiterWallet.queryBattery(deviceHandle);
                if (result.getStateCode() == 0) {
                    callback.onSuccess(result.getValue());
                } else {
                    callback.onFailed(result.getStateCode());
                }
            }
        });
    }

    // device
    public void isBootLoader(final JubCallback<Boolean> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                callback.onSuccess(JuBiterWallet.isBootLoader(deviceHandle));
            }
        });
    }

    public void getDeviceType(final JubCallback<String> callback) {

    }

    public DeviceType getDeviceType() {
        return mDeviceType;
    }

    public void setTimeout(final int timeout, final JubCallback<Void> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                int ret = JuBiterWallet.setTimeout(deviceHandle, timeout);
                if (ret == 0) {
                    callback.onSuccess(null);
                } else {
                    callback.onFailed(ret);
                }
            }
        });
    }

    public void getDeviceInfo(final JubCallback<String> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultAny deviceInfoResult = JuBiterWallet.getDeviceInfo(deviceHandle);
                if (deviceInfoResult.getStateCode() == 0) {
                    for (com.google.protobuf.Any detail : deviceInfoResult.getValueList()) {
                        try {
                            CommonProtos.DeviceInfo deviceInfo = detail.unpack(CommonProtos.DeviceInfo.class);
                            callback.onSuccess(deviceInfo.toString());
                            Log.d(TAG, "rv : " + deviceInfo.toString());
                        } catch (InvalidProtocolBufferException e) {
                            e.printStackTrace();
                        }
                    }
                } else {
                    callback.onFailed(deviceInfoResult.getStateCode());
                }
            }
        });
    }

    public void enumApplets(final JubCallback<String> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString applets = JuBiterWallet.enumApplets(deviceHandle);
                if (applets.getStateCode() == 0) {
                    callback.onSuccess(applets.getValue());
                } else {
                    callback.onFailed(applets.getStateCode());
                }
            }
        });
    }

    public void getDeviceCert(final JubCallback<String> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString deviceCert = JuBiterWallet.getDeviceCert(deviceHandle);
                if (deviceCert.getStateCode() == 0) {
                    callback.onSuccess(deviceCert.getValue());
                } else {
                    callback.onFailed(deviceCert.getStateCode());
                }
            }
        });
    }

    //comm
    public void showVirtualPwd(final int contextID, final JubCallback<Void> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                final int ret = JuBiterWallet.showVirtualPWD(contextID);
                ThreadUtils.toMainThread(new Runnable() {
                    @Override
                    public void run() {
                        if (ret == 0) {
                            callback.onSuccess(null);
                        } else {
                            callback.onFailed(ret);
                        }
                    }
                });
            }
        });
    }

    public void cancelVirtualPwd(final int contextID, final JubCallback<Void> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                int ret = JuBiterWallet.cancelVirtualPWD(contextID);
                if (callback != null) {
                    if (ret == 0) {
                        callback.onSuccess(null);
                    } else {
                        callback.onFailed(ret);
                    }
                }
            }
        });
    }

    public void verifyPIN(final int contextID, final String pin, final JubCallback<Void> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                final CommonProtos.ResultInt resultInt = JuBiterWallet.verifyPIN(contextID, pin);
                ThreadUtils.toMainThread(new Runnable() {
                    @Override
                    public void run() {
                        if (resultInt.getStateCode() == 0) {
                            callback.onSuccess(null);
                        } else {
                            callback.onFailed(resultInt.getValue());
                        }
                    }
                });
            }
        });
    }

    public void clearContext(final int contextID, final JubCallback<Void> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                int ret = JuBiterWallet.clearContext(contextID);
                if (callback != null) {
                    if (ret == 0) {
                        callback.onSuccess(null);
                    } else {
                        callback.onFailed(ret);
                    }
                }
            }
        });
    }

    // Bio
    public void enumFingerprint(JubCallback<List<String>> callback) {

    }

    public void identityVerify(int mode, JubCallback<Void> callback) {

    }

    public void identityVerifyPIN(int mode, String pin, JubCallback<Void> callback) {

    }

    public void identityShowNineGrids(JubCallback<Void> callback) {

    }

    public void identityCancelNineGrids(JubCallback<Void> callback) {

    }


    public void enrollFingerprint(JubCallback<String> callback) {

    }

    public void deleteFingerprint(byte id, JubCallback<Void> callback) {

    }

    public void eraseFingerprint(JubCallback<Void> callback) {

    }

    public void verifyFingerprint(long contextID, JubCallback<Void> callback) {

    }

    //BTC
    public void btcCreateContext(final BTC_TransType transType, final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                BitcoinProtos.ContextCfgBTC.Builder builder = BitcoinProtos.ContextCfgBTC.newBuilder();
                switch (transType) {
                    case BTC_TEST:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                                .setMainPath("m/44'/1'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case BTC_P2PKH:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                                .setMainPath("m/44'/0'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case BTC_P2WPKH:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                                .setMainPath("m/49'/0'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2SH_P2WPKH);
                        break;
                    case BCH:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBCH)
                                .setMainPath("m/44'/145'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case LTC:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINLTC)
                                .setMainPath("m/44'/2'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case USDT:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINUSDT)
                                .setMainPath("m/44'/0'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case DASH:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINDASH)
                                .setMainPath("m/44'/5'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                    case QTUM:
                        builder.setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINQTUM)
                                .setMainPath("m/44'/2301'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.P2PKH);
                        break;
                }
                BitcoinProtos.ContextCfgBTC cfgBTC = builder.build();

                CommonProtos.ResultInt context = JuBiterBitcoin.createContext(cfgBTC, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });

    }

    public void setBTCUnit(final int contextID, final BitcoinProtos.BTC_UNIT_TYPE unit, final JubCallback<Void> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                int ret = JuBiterBitcoin.setUint(contextID, unit);
                if (ret == 0) {
                    callback.onSuccess(null);
                } else {
                    callback.onFailed(ret);
                }
            }
        });
    }

    public void btcGetAddress(final BTC_TransType transType, final int contextID, final long index, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(index)
                        .setChange(false)
                        .build();
                callback.onSuccess("BTCGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterBitcoin.getMainHDNode(contextID);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                if(transType == BTC_TransType.BTC_TEST){
                    callback.onSuccess(Utils.xpub2tpub(mainHDNode.getValue()));
                } else {
                    callback.onSuccess(mainHDNode.getValue());
                }
                callback.onSuccess("BTCGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterBitcoin.getHDNode(contextID, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                if(transType == BTC_TransType.BTC_TEST){
                    callback.onSuccess(Utils.xpub2tpub(HDNode.getValue()));
                } else {
                    callback.onSuccess(HDNode.getValue());
                }
                callback.onSuccess("BTCGetAddress ");
                CommonProtos.ResultString address = JuBiterBitcoin.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void btcShowAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterBitcoin.getAddress(contextID, path, true);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void btcSetMyAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterBitcoin.setAddress(contextID, path);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void btcTransaction(int contextID, BTC_TransType transType, int decimal, String transferInputValue, JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        transferInputValue = fixValueStr(transferInputValue, "", decimal).replace(".", "");
        switch (transType) {
            case BTC_TEST:
                btcP2pkhTransactionTest(contextID, transferInputValue, callback);
                break;
            case BTC_P2PKH:
            case BTC_P2WPKH:
            case BCH:
            case LTC:
            case DASH:
            case QTUM:
                btcP2pkhTransaction(contextID, transferInputValue, callback);
                break;
            case USDT:
                usdtTransaction(contextID, transferInputValue, callback);
                break;
        }
    }

    private void btcP2pkhTransaction(final int contextID, String transferInputValue, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        long inputValue = Long.parseLong(transferInputValue);

        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();

        CommonProtos.Bip44Path bip32PathChange = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(true)
                .build();

        // inputs
        BitcoinProtos.InputBTC inputBTC_0 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32Path)
                .setPreHash("2a2e910f9fb2b04f7f1ddbfb4ab05785250c2b395f572ce591167c8451f0891e")
                .setAmount(4944420633545L)
                .setPreIndex(0)
                .build();

        BitcoinProtos.InputBTC inputBTC_1 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32PathChange)
                .setPreHash("0fe4fafd846b18fe545bbc2dcb70ecb1290ec0de6219cd2299cd0a1561c8d583")
                .setAmount(10492)
                .setPreIndex(0)
                .build();

        BitcoinProtos.InputBTC inputBTC_Self = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32PathChange)
                .setPreHash("0fe4fafd846b18fe545bbc2dcb70ecb1290ec0de6219cd2299cd0a1561c8d583")
                .setAmount(inputValue)
                .setPreIndex(0)
                .build();

        // outputs
        CommonProtos.ResultString address = JuBiterBitcoin.getAddress(contextID, bip32Path, false);
        String outputSelfAddress = "";
        if (address.getStateCode() == 0) {
            outputSelfAddress = address.getValue();
        }

        BitcoinProtos.OutputBTC outputBTC_0 = BitcoinProtos.OutputBTC.newBuilder()
                .setStdOutput(BitcoinProtos.StandardOutput.newBuilder()
                        .setAddress(outputSelfAddress)
                        .setAmount(3744400045565L)
                        .setChangeAddress(true)
                        .setPath(bip32Path)
                        .build())
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.SC_P2PKH)
                .build();

        BitcoinProtos.OutputBTC outputBTC_1 = BitcoinProtos.OutputBTC.newBuilder()
                .setStdOutput(BitcoinProtos.StandardOutput.newBuilder()
                        .setAddress(outputSelfAddress)
                        .setChangeAddress(true)
                        .setAmount(1018920)
                        .setPath(bip32Path)
                        .build())
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.SC_P2PKH)
                .build();

        BitcoinProtos.OutputBTC outputBTC_Self = BitcoinProtos.OutputBTC.newBuilder()
                .setStdOutput(BitcoinProtos.StandardOutput.newBuilder()
                        .setAddress(outputSelfAddress)
                        .setChangeAddress(false)
                        .setAmount(inputValue)
                        .setPath(bip32Path)
                        .build())
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.SC_P2PKH)
                .build();

        final BitcoinProtos.TransactionBTC transactionBTC = BitcoinProtos.TransactionBTC.newBuilder()
                .setVersion(1)
                .addInputs(0, inputBTC_0)
                .addInputs(1, inputBTC_1)
                .addInputs(2, inputBTC_Self)
                .addOutputs(0, outputBTC_0)
                .addOutputs(1, outputBTC_1)
                .addOutputs(2, outputBTC_Self)
                .setLocktime(0)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString resultString = JuBiterBitcoin.signTransaction(contextID, transactionBTC);
                if (resultString.getStateCode() == 0) {
                    callback.onSuccess(resultString.getValue());
                } else {
                    callback.onFailed(resultString.getStateCode());
                }
            }
        });
    }

    private void usdtTransaction(final int contextID, String transferInputValue, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        long inputValue = Long.parseLong(transferInputValue);

        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();

        CommonProtos.Bip44Path bip32PathChange = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(true)
                .build();

        // inputs
        BitcoinProtos.InputBTC inputBTC_0 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32Path)
                .setPreHash("2a2e910f9fb2b04f7f1ddbfb4ab05785250c2b395f572ce591167c8451f0891e")
                .setAmount(4944420633545L)
                .setPreIndex(0)
                .build();

        BitcoinProtos.InputBTC inputBTC_1 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32PathChange)
                .setPreHash("0fe4fafd846b18fe545bbc2dcb70ecb1290ec0de6219cd2299cd0a1561c8d583")
                .setAmount(10492)
                .setPreIndex(0)
                .build();


        // outputs
        CommonProtos.ResultString address = JuBiterBitcoin.getAddress(contextID, bip32Path, false);
        String outputSelfAddress = "";
        if (address.getStateCode() == 0) {
            outputSelfAddress = address.getValue();
        }

        BitcoinProtos.OutputBTC outputBTC_0 = BitcoinProtos.OutputBTC.newBuilder()
                .setStdOutput(BitcoinProtos.StandardOutput.newBuilder()
                        .setAddress(outputSelfAddress)
                        .setAmount(3744400045565L)
                        .setChangeAddress(true)
                        .setPath(bip32Path)
                        .build())
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.SC_P2PKH)
                .build();

        CommonProtos.ResultAny result = JuBiterBitcoin.buildUSDTOutput(contextID, "1EXoDusjGwvnjZUyKkxZ4UHEf77z6A5S4P", Long.parseLong(transferInputValue));
        List<BitcoinProtos.OutputBTC> outputBTCList = new ArrayList<>();
        for (com.google.protobuf.Any res : result.getValueList()) {
            try {
                BitcoinProtos.OutputBTC output = res.unpack(BitcoinProtos.OutputBTC.class);
                outputBTCList.add(output);
            } catch (InvalidProtocolBufferException e) {
                e.printStackTrace();
            }
        }

        BitcoinProtos.TransactionBTC.Builder builder = BitcoinProtos.TransactionBTC.newBuilder()
                .setVersion(1)
                .addInputs(0, inputBTC_0)
                .addInputs(1, inputBTC_1)
                .addOutputs(0, outputBTC_0);
        for (BitcoinProtos.OutputBTC outputBTC : outputBTCList) {
            builder.addOutputs(outputBTC);
        }
        final BitcoinProtos.TransactionBTC transactionBTC = builder
                .setLocktime(0)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString resultString = JuBiterBitcoin.signTransaction(contextID, transactionBTC);
                if (resultString.getStateCode() == 0) {
                    callback.onSuccess(resultString.getValue());
                } else {
                    callback.onFailed(resultString.getStateCode());
                }
            }
        });
    }

    // BTC 后台
    public void btcQueryHistory(final int contextID, JubCallback<String> callback) {
        Log.d("context: ",contextID+"");
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString address = JuBiterBitcoin.getMainHDNode(contextID);
                String addressStr = Utils.xpub2tpub(address.getValue());
                try {
                    BtcHistory btcHistory = BtcModel.getInstance().queryTransactionByAccount(
                            addressStr, 1);
                    if (btcHistory.getData() == null) {
                        callback.onFailed(btcHistory.getCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(btcHistory));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    public void btcQueryBalanceByAccount(final int contextID, JubCallback<String> callback) {
        Log.d("context: ",contextID+"");
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString address = JuBiterBitcoin.getMainHDNode(contextID);
                String addressStr = Utils.xpub2tpub(address.getValue());
                try {
                    SimpleBean balance = BtcModel.getInstance().queryBalanceByAccount(
                            addressStr);
                    if (TextUtils.isEmpty(balance.getData())) {
                        callback.onFailed(balance.getCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(balance));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private String btcTxid;

    // BTC交易全流程
    private void btcP2pkhTransactionTest(final int contextID, String transferInputValue, final JubCallback<String> callback) {
        long inputValue = Long.parseLong(transferInputValue);
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    BtcModel btcModel = BtcModel.getInstance();
                    Fees fees = btcModel.getMinerFeeEstimations();
                    if (fees.getData() == null) {
                        callback.onFailed(fees.getCode());
                        callback.onSuccess("getMinerFeeEstimations error");
                        return;
                    }
                    long fee = fees.getData().getFastestFee().longValue();

                    CommonProtos.ResultString address = JuBiterBitcoin.getMainHDNode(contextID);
                    if (address.getStateCode() != 0 || TextUtils.isEmpty(address.getValue())) {
                        callback.onSuccess("getMainHDNode error");
                        callback.onFailed(address.getStateCode());
                        return;
                    }
                    String addressStr = Utils.xpub2tpub(address.getValue());
                    PreTransactionBean preTransactionBean = btcModel.getPreTransaction(
                            addressStr,
                            fee + "",
                            inputValue,
                            "n2LPmFxg9hRZgAg89sG4HWyEM5GM1SAmud"
                    );
                    if (preTransactionBean.getPreTransaction() == null) {
                        Log.e("btcP2pkhTransactionTest", preTransactionBean.getMsg());
                        callback.onFailed(preTransactionBean.getCode());
                        callback.onSuccess("getPreTransaction error");
                        return;
                    }
                    if( preTransactionBean.getCode() != 0){
                        callback.onFailed(preTransactionBean.getCode());
                        callback.onSuccess("getPreTransaction error 2 "+preTransactionBean.getMsg());
                        return;
                    }
                    PreTransaction transaction = preTransactionBean.getPreTransaction();


                    BitcoinProtos.TransactionBTC.Builder builder = BitcoinProtos.TransactionBTC.newBuilder()
                            .setVersion(1)
                            .setLocktime(0);

                    List<Input> inputs = transaction.getInputs();
                    for (int i = 0; i < inputs.size(); i++) {
                        Input input = inputs.get(i);
                        Prevout prevout = input.getPrevout();
                        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                                .setAddressIndex(input.getIndex())
                                .setChange(input.getChange() == 1)
                                .build();
                        BitcoinProtos.InputBTC inputBTC = BitcoinProtos.InputBTC.newBuilder()
                                .setPath(bip32Path)
                                .setPreHash(prevout.getHash())
                                .setAmount(Long.parseLong(input.getAmount()))
                                .setPreIndex(prevout.getIndex())
                                .build();
                        builder.addInputs(i, inputBTC);
                    }

                    List<Output> outputs = transaction.getOutputs();
                    for (int i = 0; i < outputs.size(); i++) {
                        Output output = outputs.get(i);
                        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                                .setAddressIndex(output.getIndex())
                                .setChange(output.getChange() == 1)
                                .build();
                        BitcoinProtos.OutputBTC outputBTC = BitcoinProtos.OutputBTC.newBuilder()
                                .setStdOutput(BitcoinProtos.StandardOutput.newBuilder()
                                        .setAddress(output.getAddress())
                                        .setAmount(Long.parseLong(output.getAmount()))
                                        .setChangeAddress(output.getIsChange())
                                        .setPath(bip32Path)
                                        .build())
                                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.SC_P2PKH)
                                .build();
                        builder.addOutputs(i, outputBTC);
                    }
                    BitcoinProtos.TransactionBTC transactionBTC = builder.build();
                    CommonProtos.ResultString resultString = JuBiterBitcoin.signTransaction(contextID, transactionBTC);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess("raw: " + resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                        callback.onSuccess("signTransaction error");
                        return;
                    }

                    SimpleBean broadcast = btcModel.broadcastTransaction(resultString.getValue());
                    btcTxid = broadcast.getData();
                    if (TextUtils.isEmpty(btcTxid)) {
                        callback.onFailed(broadcast.getCode());
                        return;
                    }
                    callback.onSuccess("txid:" + btcTxid);

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    // BTC 交易查询 by txid
    public void btcQueryTransactionById(JubCallback<String> callback) {
        if (TextUtils.isEmpty(btcTxid)) {
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    TxStatus txStatus = BtcModel.getInstance().queryTransactionById(
                            btcTxid);
                    if (txStatus.getData() == null) {
                        callback.onFailed(txStatus.getStatusCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(txStatus));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }


    //ETH
    public void ethCreateContext(final ETH_TransType transType, final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                EthereumProtos.ContextCfgETH contextCfgETH = EthereumProtos.ContextCfgETH.newBuilder()
                        .setMainPath("m/44'/60'/0'")
                        //.setChainId(transType == ETH_TransType.ETH_TEST ? 42 : 1)
                        .setChainId(transType == ETH_TransType.ETH_TEST ? 4 : 1)
                        .build();

                CommonProtos.ResultInt context = JuBiterEthereum.createContext(contextCfgETH, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });
    }

    public void ethGetAddress(final int contextID, final int index, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(index)
                .setChange(false)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {

                callback.onSuccess("ETHGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterEthereum.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                callback.onSuccess(mainHDNode.getValue());
                callback.onSuccess("ETHGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterEthereum.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                callback.onSuccess(HDNode.getValue());
                callback.onSuccess("ETHGetAddress ");
                CommonProtos.ResultString address = JuBiterEthereum.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void ethShowAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterEthereum.getAddress(contextID, path, true);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

    public void ethSetMyAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterEthereum.setAddress(contextID, path);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

    public void ethTransaction(final int contextID, ETH_TransType transType, String transferInputValue, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final String valueStr = fixValueStr(transferInputValue, "", 18).replace(".", "");

        final EthereumProtos.TransactionETH.Builder builder = EthereumProtos.TransactionETH.newBuilder();
        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        builder.setPath(bip32Path)
                .setGasPriceInWei("10000000000");
        if (transType == ETH_TransType.ETH_ERC20) {
            ThreadUtils.execute(new Runnable() {
                @Override
                public void run() {
                    int setErc20Token = JuBiterEthereum.setERC20Token(
                            contextID,
                            "mfcc",
                            18,
                            "0xf45b778e53d858c79bf4dfbdd5c1bfdb426bb891");
                    if (setErc20Token != 0) {
                        callback.onFailed(setErc20Token);
                        return;
                    }

                    CommonProtos.ResultString erc20Abi = JuBiterEthereum.buildERC20TransferAbi(
                            contextID,
                            "0xef31DEc147DCDcd64F6a0ABFA7D441B62A216BC9",
                            valueStr
                    );
                    if (erc20Abi.getStateCode() != 0) {
                        callback.onFailed(erc20Abi.getStateCode());
                        return;
                    }
                    EthereumProtos.TransactionETH erc20Tx = builder.setNonce(12)
                            .setGasLimit(96000)
                            .setTo("0xf45b778e53d858c79bf4dfbdd5c1bfdb426bb891")
                            .setValueInWei("0")
                            .setInput(erc20Abi.getValue())
                            .build();
                    CommonProtos.ResultString resultString = JuBiterEthereum.signTransaction(contextID, erc20Tx);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                    }
                }
            });
        } else if (transType == ETH_TransType.ETH_ERC721) {

            ThreadUtils.execute(new Runnable() {
                @Override
                public void run() {
                    int setErc721Token = JuBiterEthereum.setERC721Token(
                            contextID,
                            "Meebits",
                            "0x7bd29408f11d2bfc23c34f18275bbf23bb716bc7"
                    );
                    if (setErc721Token != 0) {
                        callback.onFailed(setErc721Token);
                        return;
                    }

                    CommonProtos.ResultString erc721Abi = JuBiterEthereum.buildERC721TransferAbi(
                            contextID,
                            "0x0416b53d81ee3d868bbe3ce7d93980b45159b8a0",
                            "0xbb11ddc26f4e55475775310e5cbe188650e75212",
                            "14973"
                    );
                    if (erc721Abi.getStateCode() != 0) {
                        callback.onFailed(erc721Abi.getStateCode());
                        return;
                    }
                    EthereumProtos.TransactionETH erc721Tx = builder.setNonce(12)
                            .setGasLimit(96000)
                            .setValueInWei("0")
                            .setTo("0x7bd29408f11d2bfc23c34f18275bbf23bb716bc7")
                            .setInput(erc721Abi.getValue())
                            .build();
                    CommonProtos.ResultString resultString = JuBiterEthereum.signTransaction(contextID, erc721Tx);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                    }
                }
            });

        } else if (transType == ETH_TransType.ETH) {
            final EthereumProtos.TransactionETH transactionETH = builder.setNonce(13)
                    .setGasLimit(310000)
                    .setTo("0xef31DEc147DCDcd64F6a0ABFA7D441B62A216BC9")
                    .setValueInWei(valueStr)
                    .build();

            ThreadUtils.execute(new Runnable() {
                @Override
                public void run() {
                    CommonProtos.ResultString resultString = JuBiterEthereum.signTransaction(contextID, transactionETH);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                    }
                }
            });

        } else if (transType == ETH_TransType.ETH_TEST) {
            ethTransactionTest(contextID, valueStr, callback);
        }
    }

    // ETH 后台
    public void ethQueryHistory(final int contextID, JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterEthereum.getAddress(contextID, path, false);

                try {
                    EthHistory ethHistory = EthModel.getInstance().queryTransactionsByAddrs(
                            address.getValue(), "", 1);
                    if (ethHistory.getData() == null) {
                        callback.onFailed(ethHistory.getCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(ethHistory));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    public void ethQueryAccountInfo(final int contextID, JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterEthereum.getAddress(contextID, path, false);

                try {
                    EthAccountInfo accountInfo = EthModel.getInstance().queryAccountInfoByAddr(
                            address.getValue(),"");
                    if (accountInfo.getData() == null) {
                        callback.onFailed(accountInfo.getStatusCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(accountInfo));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private String ethTxid;

    // eth 交易全流程
    private void ethTransactionTest(final int contextID, String valueStr, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    EthModel ethModel = EthModel.getInstance();
                    Fees fees = ethModel.getMinerFeeEstimations();
                    if (fees.getData() == null) {
                        callback.onFailed(fees.getCode());
                        return;
                    }
                    long fee = fees.getData().getFastestFee().longValue();

                    CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                            .setAddressIndex(0)
                            .setChange(false)
                            .build();

                    CommonProtos.ResultString address = JuBiterEthereum.getAddress(contextID, bip32Path, false);

                    EthAccountInfo accountInfo = EthModel.getInstance().queryAccountInfoByAddr(
                            address.getValue(),"");
                    if (accountInfo.getData() == null) {
                        callback.onFailed(accountInfo.getStatusCode());
                        return;
                    }
                    EthereumProtos.TransactionETH transactionETH = EthereumProtos.TransactionETH
                            .newBuilder()
                            .setPath(bip32Path)
                            .setGasPriceInWei(fee * 1000000000L + "")
                            .setNonce(Integer.parseInt(accountInfo.getData().getNonce()))
                            .setGasLimit(27800)
                            .setTo("0xB530870a393DeE9f8D0b15160CFFbaF1f50CDEbB")
                            .setValueInWei(valueStr)
                            .build();

                    CommonProtos.ResultString resultString = JuBiterEthereum.signTransaction(contextID, transactionETH);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                        return;
                    }

                    SimpleBean broadcast = ethModel.broadcastTransaction(resultString.getValue());
                    ethTxid = broadcast.getData();
                    if (TextUtils.isEmpty(ethTxid)) {
                        callback.onFailed(broadcast.getCode());
                        return;
                    }
                    callback.onSuccess("txid:" + ethTxid);

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    // ETH 交易查询 by txid
    public void ethQueryTransactionById(JubCallback<String> callback) {
        if (TextUtils.isEmpty(ethTxid)) {
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    TxStatus txStatus = EthModel.getInstance().queryTransactionById(
                            ethTxid);
                    if (txStatus.getData() == null) {
                        callback.onFailed(txStatus.getStatusCode());
                        return;
                    }
                    Gson gson = new Gson();
                    String formatJson = Utils.formatJson(gson.toJson(txStatus));
                    Log.d("JSON", formatJson);
                    callback.onSuccess(formatJson);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    //EOS
    public void eosCreateContext(final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ContextCfg cfg = CommonProtos.ContextCfg.newBuilder()
                        .setMainPath("m/44'/194'/0'")
                        .build();
                CommonProtos.ResultInt context = JuBiterEOS.createContext(cfg, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });
    }

    public void eosGetAddress(final int contextID, int index, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(index)
                .setChange(false)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                callback.onSuccess("EOSGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterEOS.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                callback.onSuccess(mainHDNode.getValue());
                callback.onSuccess("EOSGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterEOS.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                callback.onSuccess(HDNode.getValue());
                callback.onSuccess("EOSGetAddress ");
                CommonProtos.ResultString address = JuBiterEOS.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void eosShowAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterEOS.getAddress(contextID, path, true);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

//    public void eosSetMyAddress(final int contextID, final JubCallback<String> callback) {
//        ThreadUtils.execute(new Runnable() {
//            @Override
//            public void run() {
//                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
//                        .setAddressIndex(0)
//                        .setChange(false)
//                        .build();
//                CommonProtos.ResultString address = JuBiterEOS.setAddress(contextID, path);
//                if (address.getStateCode() == 0) {
//                    callback.onSuccess(address.getValue());
//                } else {
//                    callback.onFailed(address.getStateCode());
//                }
//            }
//        });
//    }


    public void eosTransaction(final int contextID, final EOS_TransType transType, String value, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final EOSProtos.ActionListEOS.Builder actionListBuilder = EOSProtos.ActionListEOS.newBuilder();
        final EOSProtos.ActionEOS.Builder actionBuilder = EOSProtos.ActionEOS.newBuilder();
        switch (transType) {
            case EOS:
                value = fixEosValueStr(value);
                actionBuilder
                        .setCurrency("eosio.token")
                        .setName("transfer")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.XFER)
                        .setXferAction(EOSProtos.TransferAction.newBuilder()
                                .setFrom("ftsafetest55")
                                .setTo("zijunzimo555")
                                .setAsset(value)
                                .setMemo("First EOS tx for ftsafetest55.")
                                .build());
                break;
            case EOS_TOKEN:
                value = fixValueStr(value, " MPT", 4);
                actionBuilder
                        .setCurrency("metpacktoken")
                        .setName("transfer")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.XFER)
                        .setXferAction(EOSProtos.TransferAction.newBuilder()
                                .setFrom("ftsafetest55")
                                .setTo("zijunzimo555")
                                .setAsset(value)
                                .setMemo("@gmail.com")
                                .build());
                break;
            case EOS_BUY_RAM:
                value = fixEosValueStr(value);
                actionBuilder
                        .setCurrency("eosio")
                        .setName("buyram")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.BUYRAM)
                        .setBuyRamAction(EOSProtos.BuyRamAction.newBuilder()
                                .setPayer("ftsafetest55")
                                .setQuant(value)
                                .setReceiver("ftsafetest55")
                                .build());
                break;
            case EOS_SELL_RAM:
                actionBuilder
                        .setCurrency("eosio")
                        .setName("sellram")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.SELLRAM)
                        .setSellRamAction(EOSProtos.SellRamAction.newBuilder()
                                .setAccount("ftsafetest55")
                                .setByte(value)
                                .build());
                break;
            case EOS_STAKE:
                value = fixEosValueStr(value);
                actionBuilder
                        .setCurrency("eosio")
                        .setName("delegatebw")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.DELE)
                        .setDeleAction(EOSProtos.DelegateAction.newBuilder()
                                .setFrom("ftsafetest55")
                                .setReceiver("ftsafetest55")
                                .setCpuQty(value)
                                .setNetQty(value)
                                .setTransfer(false)
                                .setStake(true)
                                .build());
                break;
            case EOS_UNSTAKE:
                value = fixEosValueStr(value);
                actionBuilder
                        .setCurrency("eosio")
                        .setName("delegatebw")
                        .setType(EOSProtos.ENUM_EOS_ACTION_TYPE.UNDELE)
                        .setDeleAction(EOSProtos.DelegateAction.newBuilder()
                                .setFrom("zijunzimo555")
                                .setReceiver("zijunzimo555")
                                .setCpuQty(value)
                                .setNetQty(value)
                                .setTransfer(false)
                                .setStake(false)
                                .build());
                break;
        }
        actionListBuilder.addActions(actionBuilder.build());
        if (transType == EOS_TransType.EOS || transType == EOS_TransType.EOS_TOKEN) {
            ThreadUtils.execute(new Runnable() {
                @Override
                public void run() {
                    String memo = actionBuilder.getXferAction().getMemo();
                    callback.onSuccess("calculateMemoHash");
                    CommonProtos.ResultString resultString = JuBiterEOS.calculateMemoHash(memo);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                        eosBuildActions(contextID, actionListBuilder.build(), transType, callback);
                    } else {
                        callback.onFailed(resultString.getStateCode());
                    }
                }
            });
        } else {
            eosBuildActions(contextID, actionListBuilder.build(), transType, callback);
        }
    }


    private void eosBuildActions(final int contextID, final EOSProtos.ActionListEOS list, final EOS_TransType transType, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                callback.onSuccess("Eos buildAction");
                CommonProtos.ResultString resultString = JuBiterEOS.buildAction(contextID, list);
                if (resultString.getStateCode() == 0) {
                    callback.onSuccess(resultString.getValue());
                    eosExecuteTransaction(contextID, resultString.getValue(), transType, callback);
                } else {
                    callback.onFailed(resultString.getStateCode());
                }
            }
        });
    }

    private void eosExecuteTransaction(final int contextID, String actions, EOS_TransType transType, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final EOSProtos.TransactionEOS.Builder txBuilder = EOSProtos.TransactionEOS.newBuilder();
        txBuilder.setActionsInJSON(actions);
        switch (transType) {
            case EOS:
                txBuilder.setChainID("")
                        .setExpiration("900")
                        .setReferenceBlockId("052d9df0e118655258f6da291ff71c69d4f31b575a24ca550d67877ddd9b3b07")
                        .setReferenceBlockTime("1572215383");
                break;
            case EOS_TOKEN:
            case EOS_BUY_RAM:
            case EOS_STAKE:
            case EOS_UNSTAKE:
                txBuilder.setChainID("")
                        .setExpiration("900")
                        .setReferenceBlockId("052de5f233dc30d0a11a09d063f43181bea9ec44bc66b33c24d9198dc6e4c15c")
                        .setReferenceBlockTime("1572253404");
                break;
            case EOS_SELL_RAM:
                txBuilder.setChainID("9743d3df5dd83d217484cd4a0e003f94a3af6dc4f478b5bd2b47f29e128cb397")
                        .setExpiration("900")
                        .setReferenceBlockId("9743d3df5dd83d217484cd4a0e003f94a3af6dc4f478b5bd2b47f29e128cb397")
                        .setReferenceBlockTime("1536309328");
                break;
        }

        CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();

        txBuilder.setPath(path);

        callback.onSuccess("EOS transaction");
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString transaction = JuBiterEOS.signTransaction(contextID, txBuilder.build());
                if (transaction.getStateCode() == 0) {
                    callback.onSuccess(transaction.getValue());
                } else {
                    callback.onFailed(transaction.getStateCode());
                }
            }
        });
    }

    private String fixEosValueStr(String oriValue) {
        return fixValueStr(oriValue, " EOS", 4);
    }

    private String fixValueStr(String oriValue, String end, int decimal) {
        if (TextUtils.isEmpty(oriValue)) {
            return "1";
        }
        if (oriValue.startsWith(".")) {
            oriValue = "0" + oriValue;
        }
        if (!oriValue.contains(".")) {
            oriValue += ".";
        }
        int pointLen = oriValue.substring(oriValue.indexOf(".") + 1).length();
        StringBuilder oriValueBuilder = new StringBuilder(oriValue);
        while (pointLen < decimal) {
            oriValueBuilder.append("0");
            pointLen = oriValueBuilder.substring(oriValue.indexOf(".") + 1).length();
        }
        oriValue = oriValueBuilder.toString();
        return oriValue + end;
    }

    //XRP
    public void xrpCreateContext(final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ContextCfg contextCfg = CommonProtos.ContextCfg.newBuilder()
                        .setMainPath("m/44'/144'/0'")
                        .build();
                CommonProtos.ResultInt context = JuBiterXRP.createContext(contextCfg, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });
    }

    public void xrpGetAddress(final int contextID, int index, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(index)
                .setChange(false)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                callback.onSuccess("XRPGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterXRP.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                callback.onSuccess(mainHDNode.getValue());
                callback.onSuccess("XRPGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterXRP.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                callback.onSuccess(HDNode.getValue());
                callback.onSuccess("XRPGetAddress ");
                CommonProtos.ResultString address = JuBiterXRP.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void xrpShowAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterXRP.getAddress(contextID, path, true);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

//    public void xrpSetMyAddress(final int contextID, final JubCallback<String> callback) {
//        ThreadUtils.execute(new Runnable() {
//            @Override
//            public void run() {
//                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
//                        .setAddressIndex(0)
//                        .setChange(false)
//                        .build();
//                CommonProtos.ResultString address = JuBiterXRP.setAddress(contextID, path);
//                if (address.getStateCode() == 0) {
//                    callback.onSuccess(address.getValue());
//                } else {
//                    callback.onFailed(address.getStateCode());
//                }
//            }
//        });
//    }

    public void xrpTransaction(final int contextID, String transferInputValue, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        transferInputValue = fixValueStr(transferInputValue, "", 6).replace(".", "");
        final RippleProtos.TransactionXRP transactionXRP = RippleProtos.TransactionXRP.newBuilder()
                .setType(RippleProtos.ENUM_XRP_TX_TYPE.PYMT)
                .setAccount("r9LqNeG6qHxjeUocjvVki2XR35weJ9mZgQ")
                .setFee("10")
                .setSequence("1")
                .setAccountTxnId("-")
                .setFlags("2147483648")
                .setLastLedgerSequence("0")
                .setMemo(RippleProtos.XrpMemo.newBuilder()
                        .setType("http://example.eom/memo/generic")
                        .setData("rent")
                        .setFormat("text/plain")
                        .build())
                .setSourceTag("-")
                .setPymt(RippleProtos.PymtXRP.newBuilder()
                        .setType(RippleProtos.ENUM_XRP_PYMT_TYPE.DXRP)
                        .setAmount(RippleProtos.PymtAmount.newBuilder()
                                .setCurrency("-")
                                .setValue(transferInputValue)
                                .setIssuer("-")
                                .build())
                        .setDestination("rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh")
                        .setDestinationTag("0")
                        .setInvoiceId("-")
                        .build())
                .build();


        final CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ResultString result = JuBiterXRP.signTransaction(contextID, bip32Path, transactionXRP);
                if (result.getStateCode() == 0) {
                    callback.onSuccess(result.getValue());
                } else {
                    callback.onFailed(result.getStateCode());
                }
            }
        });
    }

    //TRX
    public void trxCreateContext(final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.ContextCfg contextCfg = CommonProtos.ContextCfg.newBuilder()
                        .setMainPath("m/44'/195'/0'")
                        .build();
                CommonProtos.ResultInt context = JuBiterTRX.createContext(contextCfg, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });
    }

    public void trxGetAddress(final int contextID, int index, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        final CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(index)
                .setChange(false)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                callback.onSuccess("TRXGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterTRX.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.XPUB);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                callback.onSuccess(mainHDNode.getValue());
                callback.onSuccess("TRXGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterTRX.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                callback.onSuccess(HDNode.getValue());
                callback.onSuccess("TRXGetAddress ");
                CommonProtos.ResultString address = JuBiterTRX.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void trxShowAddress(final int contextID, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterTRX.getAddress(contextID, path, true);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

//    public void trxSetMyAddress(final int contextID, final JubCallback<String> callback) {
//        ThreadUtils.execute(new Runnable() {
//            @Override
//            public void run() {
//                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
//                        .setAddressIndex(0)
//                        .setChange(false)
//                        .build();
//                CommonProtos.ResultString address = JuBiterTRX.setAddress(contextID, path);
//                if (address.getStateCode() == 0) {
//                    callback.onSuccess(address.getValue());
//                } else {
//                    callback.onFailed(address.getStateCode());
//                }
//            }
//        });
//    }

    public void trxTransaction(final int contextID,
                               TRX_TransType transType,
                               String transferInputValue, final JubCallback<String> callback) {
        if(contextID == -1){
            callback.onFailed(-1);
            return;
        }
        transferInputValue = fixValueStr(transferInputValue, "", 6).replace(".", "");
        org.tron.protos.Protocol.Transaction.Builder builder = org.tron.protos.Protocol.Transaction.newBuilder();

        String unsignedData = "";

        switch (transType) {
            case TRX: {

                //                builder.setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
//                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
//                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
//                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
//                                        .setToAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
//                                        .setAmount(Long.parseLong(transferInputValue))
//                                        .build()))
//                                .build())
//                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
//                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
//                        .setExpiration(1603346250000L)
//                        .setTimestamp(1603346193445L)
//                        // TRX 交易不能指定 feeLimit, TRC20 等代币交易需要
////                        .setFeeLimit(0)
//                        .build());

                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.TransferContract.newBuilder()
                                        // TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41A4EAD5670E9CCBB14B6FE39F38AD77D7A81D34E0")))
                                        // TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6
                                        .setToAddress(ByteString.copyFrom(HexUtils.fromString("41786A052D68171D0DFEE36D3C440968FCFE3906BA")))
                                        .setAmount(Long.parseLong(transferInputValue))
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        // TRX 交易不能指定 feeLimit, TRC20 等代币交易需要
//                        .setFeeLimit(0)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "TRX unsigned data >>>" + unsignedData);

                break;
            }

            case TRC10: {
                int trc10AssetRet = JuBiterTRX.SetTRC10Asset(contextID, "PEER".getBytes(), 6, "PEER".getBytes());
                if (trc10AssetRet != 0) {
                    callback.onFailed(trc10AssetRet);
                    return;
                }
//                builder.setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
//                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferAssetContract)
//                                .setParameter(Any.pack(org.tron.protos.contract.AssetIssueContractOuterClass.TransferAssetContract.newBuilder()
//                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
//                                        .setAssetName(ByteString.copyFrom("1003406".getBytes()))
//                                        .setToAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
//                                        .setAmount(Long.parseLong(transferInputValue))
//                                        .build()))
//                                .build())
//                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
//                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
//                        .setExpiration(1603346250000L)
//                        .setTimestamp(1603346193445L)
//                        .setFeeLimit(0)
//                        .build());

                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.TransferAssetContract)
                                .setParameter(Any.pack(org.tron.protos.contract.AssetIssueContractOuterClass.TransferAssetContract.newBuilder()
                                        // TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41A4EAD5670E9CCBB14B6FE39F38AD77D7A81D34E0")))
                                        // TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6
                                        .setToAddress(ByteString.copyFrom(HexUtils.fromString("41786A052D68171D0DFEE36D3C440968FCFE3906BA")))
                                        .setAssetName(ByteString.copyFrom("1003406".getBytes()))
                                        .setAmount(Long.parseLong(transferInputValue))
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        // TRX 交易不能指定 feeLimit, TRC20 等代币交易需要
//                        .setFeeLimit(0)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "TRC10 unsigned data >>>" + unsignedData);

                break;
            }

            case TRCFree: {
//                builder.setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
//                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                // 资源冻结
//                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.FreezeBalanceContract)
//                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.FreezeBalanceContract.newBuilder()
//                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
//                                        .setReceiverAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
//                                        // 带宽、能量
//                                        .setResource(org.tron.protos.contract.Common.ResourceCode.BANDWIDTH)
//                                        .build()))
//                                .build())
//                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
//                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
//                        .setExpiration(1603346250000L)
//                        .setTimestamp(1603346193445L)
//                        .setFeeLimit(0)
//                        .build());

                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                // 资源冻结
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.FreezeBalanceContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.FreezeBalanceContract.newBuilder()
                                        // TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41A4EAD5670E9CCBB14B6FE39F38AD77D7A81D34E0")))
                                        // TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6
                                        .setReceiverAddress(ByteString.copyFrom(HexUtils.fromString("41786A052D68171D0DFEE36D3C440968FCFE3906BA")))
//                                        // 带宽、能量
                                        .setResource(org.tron.protos.contract.Common.ResourceCode.BANDWIDTH)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        // TRX 交易不能指定 feeLimit, TRC20 等代币交易需要
//                        .setFeeLimit(0)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "TRCFree unsigned data >>>" + unsignedData);

                break;
            }

            case TRCUnfreeze: {
//                builder.setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
//                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                // 资源解冻
//                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.UnfreezeBalanceContract)
//                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.UnfreezeBalanceContract.newBuilder()
//                                        .setOwnerAddress(ByteString.copyFrom("TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG".getBytes()))
//                                        .setReceiverAddress(ByteString.copyFrom("TLb2e2uRhzxvrxMcC8VkL2N7zmxYyg3Vfc".getBytes()))
//                                        // 带宽、能量
//                                        .setResource(org.tron.protos.contract.Common.ResourceCode.BANDWIDTH)
//                                        .build()))
//                                .build())
//                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
//                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
//                        .setExpiration(1603346250000L)
//                        .setTimestamp(1603346193445L)
//                        .setFeeLimit(0)
//                        .build());

                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                // 资源解冻
                                .setType(org.tron.protos.Protocol.Transaction.Contract.ContractType.UnfreezeBalanceContract)
                                .setParameter(Any.pack(org.tron.protos.contract.BalanceContract.UnfreezeBalanceContract.newBuilder()
                                        // TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41A4EAD5670E9CCBB14B6FE39F38AD77D7A81D34E0")))
                                        // TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6
                                        .setReceiverAddress(ByteString.copyFrom(HexUtils.fromString("41786A052D68171D0DFEE36D3C440968FCFE3906BA")))
//                                        // 带宽、能量
                                        .setResource(org.tron.protos.contract.Common.ResourceCode.BANDWIDTH)
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        // TRX 交易不能指定 feeLimit, TRC20 等代币交易需要
//                        .setFeeLimit(0)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "TRCFree unsigned data >>>" + unsignedData);

                break;
            }

            case TRC20: {
                int setTrc20Token = JuBiterTRX.setTRC20Token(contextID,
                        "JST",
                        18,
                        "TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t");
                if (setTrc20Token != 0) {
                    callback.onFailed(setTrc20Token);
                    return;
                }

                CommonProtos.ResultString abiRes = JuBiterTRX.buildTRC20TransferAbi(contextID,
                        "TX8K7GXogXRPZnUsxmQrM3ZyHKXQwd93ZQ",
                        "1000000000000000000");
                if (abiRes.getStateCode() != 0) {
                    callback.onFailed(abiRes.getStateCode());
                    return;
                }

//                builder.setRawData(org.tron.protos.Protocol.Transaction.raw.newBuilder()
//                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
//                                .setType(Protocol.Transaction.Contract.ContractType.TriggerSmartContract)
//                                .setParameter(Any.pack(org.tron.protos.contract.SmartContractOuterClass.TriggerSmartContract.newBuilder()
//                                        // TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG
//                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41E193FE01FBD5B5FE3CFD74E21B153DC2AA93C010")))
//                                        // TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t
//                                        .setContractAddress(ByteString.copyFrom(HexUtils.fromString("41A614F803B6FD780986A42C78EC9C7F77E6DED13C")))
//                                        .setData(ByteString.copyFrom(HexUtils.fromString(abiRes.getValue())))
//                                        .build()))
//                                .build())
//                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
//                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
//                        .setExpiration(1603346250000L)
//                        .setTimestamp(1603346193445L)
//                        .setFeeLimit(120000)
//                        .build());

                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(Protocol.Transaction.Contract.ContractType.TriggerSmartContract)
                                .setParameter(Any.pack(org.tron.protos.contract.SmartContractOuterClass.TriggerSmartContract.newBuilder()
                                        // TWXxKuBCstP1mxnErRxUNCnthkpT6W5KgG
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41E193FE01FBD5B5FE3CFD74E21B153DC2AA93C010")))
                                        // TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t
                                        .setContractAddress(ByteString.copyFrom(HexUtils.fromString("41A614F803B6FD780986A42C78EC9C7F77E6DED13C")))
                                        .setData(ByteString.copyFrom(HexUtils.fromString(abiRes.getValue())))
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom("8610".getBytes()))
                        .setRefBlockHash(ByteString.copyFrom("6a630e523f995e67".getBytes()))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(120000)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "trc721 unsigned data >>>" + unsignedData);
                break;
            }
            case TRC20_TRANSFER:

                break;

            case TRC721: {
                int setTrc721Token = JuBiterTRX.setTRC721Token(
                        contextID,
                        "DWARFDWARFDWARF",
                        "TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6");
                if (setTrc721Token != 0) {
                    callback.onFailed(setTrc721Token);
                    return;
                }

                CommonProtos.ResultString trc721AbiRes = JuBiterTRX.buildTRC721TransferAbi(
                        contextID,
                        "TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi",
                        "TV6tnwwWAbRxnYZ5TnSdLbibznWZLY4vXf",
                        "7600000000000000");
                if (trc721AbiRes.getStateCode() != 0) {
                    callback.onFailed(trc721AbiRes.getStateCode());
                    return;
                }
                Log.d(TAG, "build trc721 (String) >>>" + trc721AbiRes.getValue());

                // todo: TRC721 地址前缀 PREFIX 不确定，TRX 为 0x41，trc20 未检查，TRC721 待定（测试上链情况）
                Protocol.Transaction.raw raw = org.tron.protos.Protocol.Transaction.raw.newBuilder()
                        .addContract(org.tron.protos.Protocol.Transaction.Contract.newBuilder()
                                .setType(Protocol.Transaction.Contract.ContractType.TriggerSmartContract)
                                .setParameter(Any.pack(org.tron.protos.contract.SmartContractOuterClass.TriggerSmartContract.newBuilder()
                                        // TR1D93WMkqjhFSxDsLTedzXVPHQXsGdGQi
                                        .setOwnerAddress(ByteString.copyFrom(HexUtils.fromString("41A4EAD5670E9CCBB14B6FE39F38AD77D7A81D34E0")))
                                        // TLwu6VYaVBc5fsaTCWRHSdk71DNrZ6Vsj6
                                        .setContractAddress(ByteString.copyFrom(HexUtils.fromString("41786A052D68171D0DFEE36D3C440968FCFE3906BA")))
                                        // warning: 不能用 trc721AbiRes.getValueBytes() ，虽然数据类型一致，但是内容不对
                                        .setData(ByteString.copyFrom(HexUtils.fromString(trc721AbiRes.getValue())))
                                        .build()))
                                .build())
                        .setRefBlockBytes(ByteString.copyFrom(HexUtils.fromString("8610")))
                        .setRefBlockHash(ByteString.copyFrom(HexUtils.fromString("6a630e523f995e67")))
                        .setExpiration(1603346250000L)
                        .setTimestamp(1603346193445L)
                        .setFeeLimit(12000000)
                        .build();

                byte[] array = raw.toByteArray();
                unsignedData = HexUtils.convertBytesToString(array);
                Log.d(TAG, "trc721 unsigned data >>>" + unsignedData);

                break;
            }
            default:
                break;
        }
        org.tron.protos.Protocol.Transaction transactionTrx = builder.build();

        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();

        // 无数据缓存，跳过 verifyPIN

//        CommonProtos.ResultString result = JuBiterTRX.packContract(contextID, transactionTrx);

        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, unsignedData);
//        CommonProtos.ResultString signRes = JuBiterTRX.signTransaction(contextID, bip32Path, result.getValue());
        if (signRes.getStateCode() == 0) {
            callback.onSuccess(signRes.getValue());
        } else {
            callback.onFailed(signRes.getStateCode());
        }
    }


    /**
     * FIL
     * @param callback
     */
    public void filCreateContext(final JubCallback<Integer> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                FilecoinProtos.ContextCfgFIL contextCfgFIL = FilecoinProtos.ContextCfgFIL.newBuilder()
                        .setMainPath("m/44'/461'/0'")
                        .build();

                CommonProtos.ResultInt context = JuBiterFilecoin.createContext(contextCfgFIL, deviceHandle);
                if (context.getStateCode() == 0) {
                    callback.onSuccess(context.getValue());
                } else {
                    callback.onFailed(context.getStateCode());
                }
            }
        });
    }

    public void filSetMyAddress(final int contextID, final JubCallback<String> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterFilecoin.setAddress(contextID, path);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }

    public void filGetAddress(final int contextID, final int index, final JubCallback<String> callback) {
        final CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(index)
                .setChange(false)
                .build();
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {

                callback.onSuccess("FILGetMainHDNode");
                CommonProtos.ResultString mainHDNode = JuBiterFilecoin.getMainHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX);
                if (mainHDNode.getStateCode() != 0) {
                    callback.onFailed(mainHDNode.getStateCode());
                    return;
                }
                callback.onSuccess(mainHDNode.getValue());
                callback.onSuccess("FILGetHDNode");
                CommonProtos.ResultString HDNode = JuBiterFilecoin.getHDNode(contextID, CommonProtos.ENUM_PUB_FORMAT.HEX, path);
                if (HDNode.getStateCode() != 0) {
                    callback.onFailed(HDNode.getStateCode());
                    return;
                }
                callback.onSuccess(HDNode.getValue());
                callback.onSuccess("FILGetAddress ");
                CommonProtos.ResultString address = JuBiterFilecoin.getAddress(contextID, path, false);
                if (address.getStateCode() != 0) {
                    callback.onFailed(address.getStateCode());
                    return;
                }
                callback.onSuccess(address.getValue());
            }
        });
    }

    public void filShowAddress(final int contextID, final JubCallback<String> callback) {
        ThreadUtils.execute(new Runnable() {
            @Override
            public void run() {
                CommonProtos.Bip44Path path = CommonProtos.Bip44Path.newBuilder()
                        .setAddressIndex(0)
                        .setChange(false)
                        .build();
                CommonProtos.ResultString address = JuBiterFilecoin.getAddress(contextID, path, true);
                if (address.getStateCode() == 0) {
                    callback.onSuccess(address.getValue());
                } else {
                    callback.onFailed(address.getStateCode());
                }
            }
        });
    }



    public void filTransaction(final int contextID, FIL_TransType transType, String transferInputValue, final JubCallback<String> callback) {
        final String valueStr = fixValueStr(transferInputValue, "", 18).replace(".", "");

        CommonProtos.Bip44Path bip32Path = CommonProtos.Bip44Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();

        final FilecoinProtos.TransactionFIL.Builder builder = FilecoinProtos.TransactionFIL.newBuilder();
        if (transType == FIL_TransType.FIL) {
            final FilecoinProtos.TransactionFIL transactionFIL = builder.setNonce(3)
                    .setPath(bip32Path)
                    .setGasFeeCapInAtto("31492978632")
                    .setGasPremiumInAtto("151239")
                    .setGasLimit(611585)
                    .setTo("f1susdjal2rogayvfr6m7wypfhawcmuc3ny7v7osi")
                    .setValueInAtto(valueStr)
                    .build();

            ThreadUtils.execute(new Runnable() {
                @Override
                public void run() {
                    CommonProtos.ResultString resultString = JuBiterFilecoin.signTransaction(contextID, transactionFIL);
                    if (resultString.getStateCode() == 0) {
                        callback.onSuccess(resultString.getValue());
                    } else {
                        callback.onFailed(resultString.getStateCode());
                    }
                }
            });

        }
    }
}