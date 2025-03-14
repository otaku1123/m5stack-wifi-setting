#include "wifi_manager.h"

WiFiManager::WiFiManager(
    DisplayManager& display,
    PreferenceManager *preferenceManager) 
: display(display),
pPreferenceManager(preferenceManager) {
}

WiFiManager::~WiFiManager() {
}

void WiFiManager::startAP()
{
    WiFi.softAP(apSsid, apPassword);
    display.showAPInfo(apSsid, apPassword, WiFi.softAPIP().toString());
}

bool WiFiManager::connectToSavedWiFi()
{
    String ssid = pPreferenceManager->getPreference("ssid");
    String password = pPreferenceManager->getPreference("password");

    Serial.printf("Saved Wi-Fi credentials: %s=%s\n", ssid.c_str(), password.c_str());

    if (ssid.length() == 0 || password.length() == 0)
    {
        Serial.println("No saved Wi-Fi credentials found");
        return false;
    }

    connectToWiFi(ssid.c_str(), password.c_str());

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        display.showWiFiConnectionStatus("Connected: " + WiFi.localIP().toString());
        return true;
    }

    return true;
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
        display.showWiFiConnectionStatus("Connected: " + WiFi.localIP().toString());
        saveWiFiCredentials(ssid, password);
    }
    else
    {
        Serial.println("\nWiFi connection failed!");
        display.showWiFiConnectionStatus("Connection Failed");
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

void WiFiManager::saveWiFiCredentials(const char *ssid, const char *password) {
    pPreferenceManager->setPreference("ssid", ssid);
    pPreferenceManager->setPreference("password", password);
}

void WiFiManager::clearWiFiCredentials() {
    pPreferenceManager->clearPreference("ssid");
    pPreferenceManager->clearPreference("password");
}