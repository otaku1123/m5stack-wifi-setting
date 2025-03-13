#pragma once

#include <WiFi.h>
#include "display/display_manager.h"

class WiFiManager
{
public:
    WiFiManager(DisplayManager &display);
    void startAP();
    void connectToWiFi(const char *ssid, const char *password);
    String scanNetworks();
    IPAddress getLocalIP();

private:
    DisplayManager &display;
    const char *apSsid = "M5Stack_Setup";
    const char *apPassword = "12345678";
};