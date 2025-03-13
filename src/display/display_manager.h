#pragma once

#include <M5Stack.h>

class DisplayManager {
public:
    DisplayManager();
    void showAPInfo(const String& ssid, const String& password, const String& ip);
    void showMessage(const String& message);
    void showWiFiScanResults(const String& results);
    void showWiFiConnectionStatus(const String& status);
    void showQrCode(const String& url);
};