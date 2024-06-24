public class WiFiApi {
    public static  void enableWifi(Context context){
        WifiManager wifiManager = (WifiManager) context.getSystemService(context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(true);//Turn on Wifi
    }
}
