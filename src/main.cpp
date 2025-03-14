#include "wifi/wifi_manager.h"
#include "web_server/web_server_manager.h"
#include "preference/preference_manager.h"
#include <M5Stack.h>

DisplayManager display;
PreferenceManager *preferenceManager = PreferenceManager::getInstance();
WiFiManager wifiManager(display, preferenceManager);
WebServerManager webServerManager(wifiManager);

void setup() {
    Serial.begin(115200);

    M5.begin();

    if (!wifiManager.connectToSavedWiFi()) {
        wifiManager.startAP();
    }

    webServerManager.begin();
}

void loop() {
    webServerManager.handleClient();
}
