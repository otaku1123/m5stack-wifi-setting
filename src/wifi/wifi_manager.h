#pragma once

#include <WiFi.h>
#include "display/display_manager.h"
#include "preference/preference_manager.h"

class WiFiManager
{
public:
    WiFiManager(DisplayManager &display, PreferenceManager *preferenceManager);
    ~WiFiManager();

    void startAP();
    bool connectToSavedWiFi();
    void connectToWiFi(const char *ssid, const char *password);
    String scanNetworks();
    IPAddress getLocalIP();
    void saveWiFiCredentials(const char *ssid, const char *password);
    void clearWiFiCredentials();

private:
    DisplayManager &display;
    PreferenceManager *pPreferenceManager;
    const char *apSsid = "M5Stack_Setup";
    const char *apPassword = "12345678";
};