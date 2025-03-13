#pragma once

#include "wifi/wifi_manager.h"
#include <WebServer.h>

class WebServerManager {
public:
    WebServerManager(WiFiManager &wifiManager);
    void begin();
    void handleClient();
private:
    WebServer server;
    WiFiManager& wifiManager;
    void handleRoot();
    void handleScan();
    void handleSetWiFi();
};