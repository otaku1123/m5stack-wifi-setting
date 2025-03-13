#pragma once

#include <WiFi.h>

class WiFiManager {
    public:
        WiFiManager();
        void startAP();
        void connectToWiFi(const char *ssid, const char *password);
        String scanNetworks();
        IPAddress getLocalIP();
    private:
        const char* apSsid = "M5Stack_Setup";
        const char* apPassword = "12345678";
};