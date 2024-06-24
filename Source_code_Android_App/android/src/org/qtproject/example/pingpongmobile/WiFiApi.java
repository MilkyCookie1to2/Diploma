package org.qtproject.example.pingpongmobile;

import android.content.Context;
import android.os.Bundle;
import android.app.Activity;
import android.net.ConnectivityManager;
import android.net.wifi.WifiManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.ScanResult;
import android.content.BroadcastReceiver;
import java.util.List;
import android.net.NetworkSpecifier;
import android.net.wifi.WifiNetworkSpecifier.Builder;
import android.net.NetworkRequest;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.MacAddress;
import android.net.ConnectivityManager.NetworkCallback;
import android.net.Uri;
import android.provider.Settings;

public class WiFiApi extends BroadcastReceiver{

    private WifiManager wifiManager;

    public native void wifiScanComplete(String resultJson);
    public native void statusConnection(String msg);

    static int checkEnableWifi(Context context){
        WifiManager wifiMan = (WifiManager) context.getSystemService(context.WIFI_SERVICE);
        if(wifiMan.isWifiEnabled()){
            return 1;
        } else {
            return 0;
        }
    }

    public int connectToPlayer(Context context, String BSSID) {
        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);

        final NetworkSpecifier specifier = new Builder()
            .setBssid(MacAddress.fromString(BSSID))
            .setWpa2Passphrase("123456789")
            .build();

        final NetworkRequest request = new NetworkRequest.Builder()
             .addTransportType(NetworkCapabilities.TRANSPORT_WIFI)
             .removeCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
             .setNetworkSpecifier(specifier)
             .build();

        final NetworkCallback networkCallback = new NetworkCallback() {

            @Override
            public void onAvailable(Network network) {
                connectivityManager.bindProcessToNetwork(network);
                statusConnection("Success");
            }

            @Override
            public void onLost(Network network) {
                statusConnection("Соединение потеряно");
            }

            @Override
            public void onUnavailable() {
                statusConnection("Недоступен");
            }

        };

        connectivityManager.requestNetwork(request, networkCallback, 50000);

        return 0;
    }

    public int checkConnection(Context context) {
        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);
        NetworkInfo wifiConnection = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        wifiManager = (WifiManager) context.getSystemService(context.WIFI_SERVICE);

        if (wifiConnection.isConnected()) {
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();

            if (wifiInfo.getBSSID().equals("e0:5a:1b:d1:6d:41") || wifiInfo.getBSSID().equals("08:3a:f2:aa:6d:09")) {
                return 1;
            }
        }

        context.registerReceiver(this, new IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
        wifiManager.startScan();
        return 0;
    }

    public static String getScanResultsJson(List<ScanResult> results) {
        String BSSIDs = "";

        for (ScanResult result : results) {
            BSSIDs += result.BSSID;
            BSSIDs += " ";
        }

        return BSSIDs;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        boolean success = intent.getBooleanExtra(
                               WifiManager.EXTRA_RESULTS_UPDATED, false);
        List<ScanResult> results = wifiManager.getScanResults();
        String resultJson = getScanResultsJson(results);
        context.unregisterReceiver(this);
        wifiScanComplete(resultJson);
    }

    public static void openPermissionSettings(Context context) {
        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        Uri uri = Uri.fromParts("package", context.getPackageName(), null);
        intent.setData(uri);
        context.startActivity(intent);
    }
}
