#include "wifi_manager.h"

WiFiManager::WiFiManager() {}

void WiFiManager::startAP()
{
    WiFi.softAP(apSsid, apPassword);
}

void WiFiManager::connectToWiFi(const char *ssid, const char *password)
{
    WiFi.softAPdisconnect(true);

    WiFi.begin(ssid, password);
    Serial.printf("Connecting to WiFi: %s\n", ssid);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.println("\nWiFi connection failed!");
    }
}

String WiFiManager::scanNetworks() {
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) json += ",";
        json += "\"" + WiFi.SSID(i) + "\"";
    }
    json += "]";
    return json;
}

IPAddress WiFiManager::getLocalIP() {
    return WiFi.localIP();
}