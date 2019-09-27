package com.jubiter.sdk.example;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.google.protobuf.InvalidProtocolBufferException;
import com.jubiter.sdk.ConnectionStateCallback;
import com.jubiter.sdk.JuBiterBitcoin;
import com.jubiter.sdk.JuBiterEthereum;
import com.jubiter.sdk.JuBiterWallet;
import com.jubiter.sdk.jni.InnerScanCallback;
import com.jubiter.sdk.proto.BitcoinProtos;
import com.jubiter.sdk.proto.CommonProtos;
import com.jubiter.sdk.proto.EthereumProtos;

import java.util.ArrayList;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.annotation.Size;
import androidx.appcompat.app.AppCompatActivity;
import pub.devrel.easypermissions.AppSettingsDialog;
import pub.devrel.easypermissions.EasyPermissions;

public class MainActivity extends AppCompatActivity implements EasyPermissions.PermissionCallbacks {
    private static final String TAG = "JuBiterTest";

    private final static int REQUEST_PERMISSION = 0x1001;

    private String mnemonic;
    private String seed;
    private int deviceID;
    private int contextID;


    enum COIN_TYPE {
        BTC,
        ETH,
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
        if (!hasPermissions()) {
            requestPermissions("Permission request", REQUEST_PERMISSION, Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION);
        } else {
            JuBiterWallet.initDevice();
        }
    }

    private void init() {
        addListenerOnGenerateMnemonicBtn();
        addListenerOnCheckMnemonicBtn();
        addListenerOnGenerateSeedBtn();
        addListenerOnSeedToMasterKeyBtn();
        addListenerOnGetDeviceInfoBtn();
        addListenerOnGetDeviceCertBtn();
        addListenerOnSendApduBtn();

        addListenerOnScanDeviceBtn();
        addListenerOnConnectDeviceBtn();
        addListenerOnCancelConnectBtn();
        addListenerOnIsConectedBtn();
        addListenerOnDisconnectDevice();

        addListenerOnIsInitializeBtn();
        addListenerOnIsBootloaderBtn();
        addListenerOnSetTimeoutBtn();
        addListenerOnEnumAppletsBtn();
        addListenerOnEnumSupportCoins();
        addListenerOnGetAppletVersionBtn();
        addListenerOnQueryBatteryBtn();

        addListenerOnClearContextBtn();
        addListenerOnShowVirtualPWDBtn();
        addListenerOnCancelVirtualPWDBtn();
        addListenerOnVerifyPINBtn();

        addListenerOnBTCCreateContext_SoftwareBtn();
        addListenerOnBTCCreateContextBtn();
        addListenerOnBTCGetMainHDNodetn();
        addListenerOnBTCGetHDNodeBtn();
        addListenerOnBTCGetAddressBtn();
        addListenerOnBTCTransactionBtn();
        addListenerOnBuildUSDTOutputBtn();
        addListenerOnSetUintBtn();

        addListenerOnETHCreateContext_SoftwareBtn();
        addListenerOnETHCreateContextBtn();
        addListenerOnETHGetMainHDNodetn();
        addListenerOnETHGetHDNodeBtn();
        addListenerOnETHGetAddressBtn();
        addListenerOnETHTransactionBtn();
        addListenerOnBuildERC20AbiBtn();

    }

    private void addListenerOnScanDeviceBtn() {
        findViewById(R.id.scanDevice_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                JuBiterWallet.startScan(new InnerScanCallback() {
                    @Override
                    public void onScanResult(String name, String uuid, int devType) {
                        printDevice(name, uuid, devType);
                    }

                    @Override
                    public void onStop() {

                    }

                    @Override
                    public void onError(int errorCode) {

                    }
                });
            }
        });

    }

    private void addListenerOnConnectDeviceBtn() {
        findViewById(R.id.connectDevice_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                JuBiterWallet.stopScan();
                JuBiterWallet.connectDeviceAsync("DD:A6:FE:7D:9C:27", 15 * 1000, new ConnectionStateCallback() {
                    @Override
                    public void onConnected(String name, int handle) {
                        Log.d(TAG, ">>> onConnected - handle: " + handle);
                        deviceID = handle;
                    }

                    @Override
                    public void onDisconnected(String mac) {
                        Log.d(TAG, ">>> disconnect: " + mac);
                    }

                    @Override
                    public void onError(int error) {
                        Log.d(TAG, ">>> onError: " + error);
                    }
                });
            }
        });
    }

    private void addListenerOnDisconnectDevice() {
        findViewById(R.id.disconnectDevice_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                JuBiterWallet.disconnectDevice(deviceID);
            }
        });
    }

    private void addListenerOnCancelConnectBtn() {
        findViewById(R.id.cancelConnect_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int rv = JuBiterWallet.cancelConnect("DD:A6:FE:7D:9C:27");
                Log.d(TAG, ">>> cancelConnect: " + rv);
            }
        });
    }

    private void addListenerOnIsConectedBtn() {
        findViewById(R.id.isConnected_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean rv = JuBiterWallet.isConnected(deviceID);
                Log.d(TAG, ">>> isConnected: " + rv);
            }
        });
    }

    private void addListenerOnGenerateMnemonicBtn() {
        findViewById(R.id.generateMnemonic_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterWallet.generateMnemonic(CommonProtos.ENUM_MNEMONIC_STRENGTH.STRENGTH128);
                        Log.d(TAG, ">>> generateMnemonic - rv : " + result.getRv() + " value: " + result.getRes());
                        mnemonic = result.getRes();
                    }
                });
            }
        });
    }

    private void addListenerOnCheckMnemonicBtn() {
        findViewById(R.id.checkMnemonic_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = JuBiterWallet.checkMnemonic(mnemonic);
                Log.d(TAG, ">>> generateMnemonic - rv : " + result);
            }
        });
    }

    private void addListenerOnGenerateSeedBtn() {
        findViewById(R.id.generateSeed_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterWallet.generateSeed(mnemonic, "123");
                        Log.d(TAG, ">>> generateSeed - rv : " + result.getRv() + " value: " + result.getRes());
                        seed = result.getRes();
                    }
                });
            }
        });
    }

    private void addListenerOnSeedToMasterKeyBtn() {
        findViewById(R.id.seedToMasterKey_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterWallet.seedToMasterPrivateKey(seed, CommonProtos.CURVES.secp256k1);
                        Log.d(TAG, ">>> seedToMasterPrivateKey - rv : " + result.getRv() + " value: " + result.getRes());
                    }
                });

            }
        });
    }

    private void addListenerOnGetDeviceInfoBtn() {
        findViewById(R.id.getDeviceInfo_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultAny result = JuBiterWallet.getDeviceInfo(deviceID);
                        for (com.google.protobuf.Any detail : result.getResList()) {
                            try {
                                CommonProtos.DeviceInfo deviceInfo = detail.unpack(CommonProtos.DeviceInfo.class);
                                Log.d(TAG, "rv : " + deviceInfo.toString());
                            } catch (InvalidProtocolBufferException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                });
            }
        });
    }

    private void addListenerOnGetDeviceCertBtn() {
        findViewById(R.id.getDeviceCert_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterWallet.getDeviceCert(deviceID);
                        Log.d(TAG, ">>> getDeviceCert - rv : " + result.getRv() + " value: " + result.getRes());
                    }
                });

            }
        });
    }

    private void addListenerOnSendApduBtn() {
        findViewById(R.id.sendApdu_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterWallet.sendApdu(deviceID, "00A40400");
                        Log.d(TAG, ">>> sendApdu - rv : " + result.getRv() + " value: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnIsInitializeBtn() {
        findViewById(R.id.isInitialize_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean result = JuBiterWallet.isInitialize(deviceID);
                Log.d(TAG, ">>> isInitialize : " + result);
            }
        });
    }

    private void addListenerOnIsBootloaderBtn() {
        findViewById(R.id.isBootLoader_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean result = JuBiterWallet.isBootLoader(deviceID);
                Log.d(TAG, ">>> isInitialize : " + result);
            }
        });
    }

    private void addListenerOnSetTimeoutBtn() {
        findViewById(R.id.setTimeout_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                CommonProtos.ResultString result = JuBiterWallet.setTimeout();
//                printLog(result);
            }
        });
    }

    private void addListenerOnEnumAppletsBtn() {
        findViewById(R.id.enumApplets_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultString result = JuBiterWallet.enumApplets(deviceID);
                Log.d(TAG, ">>> enumApplets - rv : " + result.getRv() + " value: " + result.getRes());
            }
        });
    }

    private void addListenerOnEnumSupportCoins() {
        findViewById(R.id.enumSupportCoins_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultString result = JuBiterWallet.enumSupportCoins(deviceID);
                Log.d(TAG, ">>> enumSupportCoins - rv : " + result.getRv() + " value: " + result.getRes());
            }
        });
    }

    private void addListenerOnGetAppletVersionBtn() {
        findViewById(R.id.getAppletVersion_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultString result = JuBiterWallet.getAppletVersion(deviceID, "d1560001328300424c44000042544301");
                Log.d(TAG, ">>> getAppletVersion - rv : " + result.getRv() + " value: " + result.getRes());
            }
        });
    }

    private void addListenerOnQueryBatteryBtn() {
        findViewById(R.id.queryBattery_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultInt result = JuBiterWallet.queryBattery(deviceID);
                Log.d(TAG, ">>> rv: " + result.getRv() + " res: " + result.getRes());
            }
        });
    }

    private void addListenerOnClearContextBtn() {
        findViewById(R.id.clearContext_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = JuBiterWallet.clearContext(contextID);
                Log.d(TAG, ">>> rv: " + result);
            }
        });
    }

    private void addListenerOnShowVirtualPWDBtn() {
        findViewById(R.id.showVirtualPWD_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = JuBiterWallet.showVirtualPWD(contextID);
                Log.d(TAG, ">>> rv: " + result);
            }
        });
    }

    private void addListenerOnCancelVirtualPWDBtn() {
        findViewById(R.id.cancelVirtualPWD_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int result = JuBiterWallet.cancelVirtualPWD(contextID);
                Log.d(TAG, ">>> rv: " + result);
            }
        });
    }

    private void addListenerOnVerifyPINBtn() {
        findViewById(R.id.verifyPIN_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultInt result = JuBiterWallet.verifyPIN(contextID, "5555");
                Log.d(TAG, ">>> rv: " + result);
            }
        });
    }

    //*********************************** BTC 钱包 **********************************************

    private void addListenerOnBTCCreateContext_SoftwareBtn() {
        findViewById(R.id.btcCreateContext_software_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BitcoinProtos.ContextCfgBTC config = BitcoinProtos.ContextCfgBTC.newBuilder()
                        .setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                        .setMainPath("m/44'/0'/0'")
                        .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.p2pkh)
                        .build();
                CommonProtos.ResultInt result = JuBiterBitcoin.createContext_Software(config, "xpub6CAxrkiSbwkn4LayKD6qBcZg4tQvhHBH7TofQjNV9Lb3cB5u8owxdLGfc2bKoz2McoviAMXzWHwSaqc5Sm8C9SWMsnvuBw1bjEwtWsMZZFX");
                Log.d(TAG, ">>> rv: " + result.getRes());
            }
        });
    }

    private void addListenerOnBTCCreateContextBtn() {
        findViewById(R.id.btcCreateContext_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        BitcoinProtos.ContextCfgBTC config = BitcoinProtos.ContextCfgBTC.newBuilder()
                                .setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                                .setMainPath("m/44'/0'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.p2pkh)
                                .build();
                        CommonProtos.ResultInt result = JuBiterBitcoin.createContext(config, deviceID);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                        contextID = result.getRes();
                    }
                });
            }
        });
    }

    private void addListenerOnBTCGetMainHDNodetn() {
        findViewById(R.id.btcGetMainHDNode_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterBitcoin.getMainHDNode(contextID);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnBTCGetHDNodeBtn() {
        findViewById(R.id.btcGetHDNode_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.Bip32Path bip32Path = CommonProtos.Bip32Path.newBuilder()
                                .setAddressIndex(0)
                                .setChange(false)
                                .build();
                        CommonProtos.ResultString result = JuBiterBitcoin.getHDNode(contextID, bip32Path);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnBTCGetAddressBtn() {
        findViewById(R.id.btcGetAddress_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.Bip32Path bip32Path = CommonProtos.Bip32Path.newBuilder()
                                .setAddressIndex(0)
                                .setChange(false)
                                .build();
                        CommonProtos.ResultString result = JuBiterBitcoin.getAddress(contextID, bip32Path, false);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnBTCTransactionBtn() {
        findViewById(R.id.btcTransaction_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        BitcoinProtos.ContextCfgBTC config = BitcoinProtos.ContextCfgBTC.newBuilder()
                                .setCoinType(BitcoinProtos.ENUM_COIN_TYPE_BTC.COINBTC)
                                .setMainPath("m/44'/0'/0'")
                                .setTransType(BitcoinProtos.ENUM_TRAN_STYPE_BTC.p2pkh)
                                .build();
                        CommonProtos.ResultInt result = JuBiterBitcoin.createContext(config, deviceID);
                        if (0 != result.getRv()) {
                            Log.d(TAG, "createContext : " + result.getRv());
                            return;
                        }

                        final int contextID2 = result.getRes();
                        int rv = JuBiterWallet.showVirtualPWD(contextID2);
                        Log.d(TAG, "showVirtualPWD : " + rv);
                        if (0 != rv) {
                            return;
                        }

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                showPINDialog(COIN_TYPE.BTC, contextID2);
                            }
                        });
                    }
                });
            }
        });
    }

    private void showPINDialog(final COIN_TYPE coinType, final int contextID) {
        VerifyPinDialog dialog = new VerifyPinDialog(MainActivity.this, new VerifyPinDialog.Callback() {
            @Override
            public void onClickListener(String pin) {
                CommonProtos.ResultInt resultInt = JuBiterWallet.verifyPIN(contextID, pin);
                Log.d(TAG, ">>> verifyPIN : " + resultInt.getRv());
                if (0 != resultInt.getRv()) {
                    return;
                }
                switch (coinType) {
                    case BTC:
                        btcTransaction(contextID);
                        break;
                    case ETH:
                        ethTransaction(contextID);
                        break;
                }
            }
        });
        dialog.show();
    }

    private void btcTransaction(int contextID) {
        // inputs
        CommonProtos.Bip32Path bip32Path_1 = CommonProtos.Bip32Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        BitcoinProtos.InputBTC inputBTC_1 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32Path_1)
                .setPreHash("2a2e910f9fb2b04f7f1ddbfb4ab05785250c2b395f572ce591167c8451f0891e")
                .setAmount(1200)
                .setPreIndex(0)
                .build();

        CommonProtos.Bip32Path bip32Path_2 = CommonProtos.Bip32Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        BitcoinProtos.InputBTC inputBTC_2 = BitcoinProtos.InputBTC.newBuilder()
                .setPath(bip32Path_2)
                .setPreHash("0fe4fafd846b18fe545bbc2dcb70ecb1290ec0de6219cd2299cd0a1561c8d583")
                .setAmount(1000)
                .setPreIndex(0)
                .build();

        // outputs
        BitcoinProtos.StandardOutput ouput_1 = BitcoinProtos.StandardOutput.newBuilder()
                .setAddress("3MA1nkDJhthnC7DxoixmhLiXTyFo1eKJu7")
                .setAmount(800)
                .setChangeAddress(false)
                .build();
        BitcoinProtos.OutputBTC outputBTC_1 = BitcoinProtos.OutputBTC.newBuilder()
                .setStandardOputput(ouput_1)
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.STANDARD)
                .build();

        CommonProtos.Bip32Path bip32Path_3 = CommonProtos.Bip32Path.newBuilder()
                .setAddressIndex(0)
                .setChange(false)
                .build();
        // 找零
        BitcoinProtos.StandardOutput ouput_2 = BitcoinProtos.StandardOutput.newBuilder()
                .setAddress("1JpuFuiBfMzm99JzZG4rpZexxjortaH42t")
                .setChangeAddress(true)
                .setAmount(500)
                .setPath(bip32Path_3)
                .build();
        BitcoinProtos.OutputBTC outputBTC_2 = BitcoinProtos.OutputBTC.newBuilder()
                .setStandardOputput(ouput_2)
                .setType(BitcoinProtos.ENUM_SCRIPT_TYPE_BTC.STANDARD)
                .build();

        BitcoinProtos.TransactionBTC transactionBTC = BitcoinProtos.TransactionBTC.newBuilder()
                .setVersion(1)
                .addInputs(0, inputBTC_1)
                .addInputs(1, inputBTC_2)
                .addOutputs(0, outputBTC_1)
                .addOutputs(1, outputBTC_2)
                .setLocktime(0)
                .build();
        CommonProtos.ResultString signResult = JuBiterBitcoin.signTransaction(contextID, transactionBTC);
        Log.d(TAG, ">>> signTransaction - rv : " + signResult.getRv() + " value: " + signResult.getRes());
    }

    private void addListenerOnBuildUSDTOutputBtn() {
        findViewById(R.id.buildUSDToutput_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultAny result = JuBiterBitcoin.buildUSDTOutput(contextID, "1JpuFuiBfMzm99JzZG4rpZexxjortaH42t", 2000);

                List<BitcoinProtos.OutputBTC> outputBTCList = new ArrayList<>();
                for (com.google.protobuf.Any res : result.getResList()) {
                    try {
                        BitcoinProtos.OutputBTC output = res.unpack(BitcoinProtos.OutputBTC.class);
                        outputBTCList.add(output);
                    } catch (InvalidProtocolBufferException e) {
                        e.printStackTrace();
                    }
                }
                Log.d(TAG, "rv : " + outputBTCList.toString());
            }
        });
    }

    private void addListenerOnSetUintBtn() {
        findViewById(R.id.setUint_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int rv = JuBiterBitcoin.setUint(contextID, BitcoinProtos.BTC_UNIT_TYPE.BTC);
                Log.d(TAG, ">>> setUint rv: " + rv);
            }
        });
    }

    private void addListenerOnETHCreateContext_SoftwareBtn() {
        findViewById(R.id.ethCreateContext_software_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                        .setMainPath("m/44'/60'/0'")
                        .build();
                CommonProtos.ResultInt result = JuBiterEthereum.createContext_Software(config, "");
                Log.d(TAG, ">>> rv: " + result.getRes());
            }
        });
    }

    private void addListenerOnETHCreateContextBtn() {
        findViewById(R.id.ethCreateContext_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                                .setMainPath("m/44'/60'/0'")
                                .build();
                        CommonProtos.ResultInt result = JuBiterEthereum.createContext(config, deviceID);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                        contextID = result.getRes();
                    }
                });
            }
        });
    }

    private void addListenerOnETHGetMainHDNodetn() {
        findViewById(R.id.ethGetMainHDNode_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.ResultString result = JuBiterEthereum.getMainHDNode(contextID, EthereumProtos.ENUM_PUB_FORMAT.XPUB);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnETHGetHDNodeBtn() {
        findViewById(R.id.ethGetHDNode_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.Bip32Path bip32Path = CommonProtos.Bip32Path.newBuilder()
                                .setAddressIndex(0)
                                .setChange(false)
                                .build();
                        CommonProtos.ResultString result = JuBiterEthereum.getHDNode(contextID, EthereumProtos.ENUM_PUB_FORMAT.XPUB, bip32Path);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnETHGetAddressBtn() {
        findViewById(R.id.ethGetAddress_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        CommonProtos.Bip32Path bip32Path = CommonProtos.Bip32Path.newBuilder()
                                .setAddressIndex(0)
                                .setChange(false)
                                .build();
                        CommonProtos.ResultString result = JuBiterEthereum.getAddress(contextID, bip32Path, false);
                        Log.d(TAG, ">>> rv: " + result.getRes());
                    }
                });
            }
        });
    }

    private void addListenerOnETHTransactionBtn() {
        findViewById(R.id.ethTransaction_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ThreadUtils.execute(new Runnable() {
                    @Override
                    public void run() {
                        EthereumProtos.ContextCfgETH config = EthereumProtos.ContextCfgETH.newBuilder()
                                .setMainPath("m/44'/60'/0'")
                                .build();
                        CommonProtos.ResultInt result = JuBiterEthereum.createContext(config, deviceID);
                        if (0 != result.getRv()) {
                            Log.d(TAG, "createContext : " + result.getRv());
                            return;
                        }

                        final int contextID2 = result.getRes();
                        int rv = JuBiterWallet.showVirtualPWD(contextID2);
                        Log.d(TAG, "showVirtualPWD : " + rv);
                        if (0 != rv) {
                            return;
                        }

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                showPINDialog(COIN_TYPE.ETH, contextID2);
                            }
                        });
                    }
                });
            }
        });
    }

    private void ethTransaction(int contextID) {
        CommonProtos.Bip32Path bip32Path = CommonProtos.Bip32Path.newBuilder()
                .setChange(false)
                .setAddressIndex(0)
                .build();
        EthereumProtos.TransactionETH transactionETH = EthereumProtos.TransactionETH.newBuilder()
                .setPath(bip32Path)
                .setNonce(13)
                .setGasLimit(310000)
                .setGasPriceInWei("10000000000")
                .setTo("0xef31DEc147DCDcd64F6a0ABFA7D441B62A216BC9")
                .setValueInWei("500000000000000")
                .setInput("4A75626974657257616C6C6574")
                .build();
        CommonProtos.ResultString result = JuBiterEthereum.signTransaction(contextID, transactionETH);
        Log.d(TAG, ">>> signTransaction - rv : " + result.getRv() + " value: " + result.getRes());
    }


    private void addListenerOnBuildERC20AbiBtn() {
        findViewById(R.id.buildErc20Abi_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CommonProtos.ResultString result = JuBiterEthereum.buildERC20Abi(contextID, "", "10000000");
                Log.d(TAG, ">>> buildERC20Abi rv: " + result);
            }
        });
    }


    private void printDevice(String name, String mac, int deviceType) {
        Log.d(TAG, "name : " + name + " mac: " + mac + " type: " + deviceType);
    }

    public boolean hasPermissions() {
        return EasyPermissions.hasPermissions(this, Manifest.permission.ACCESS_COARSE_LOCATION);
    }

    public void requestPermissions(@NonNull String rationale,
                                   int requestCode, @Size(min = 1) @NonNull String... perms) {
        EasyPermissions.requestPermissions(this, rationale, requestCode, perms);
    }

    @Override
    public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {
        JuBiterWallet.initDevice();
    }

    @Override
    public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {
        if (EasyPermissions.somePermissionPermanentlyDenied(this, perms)) {
            new AppSettingsDialog.Builder(this).build().show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }
}
