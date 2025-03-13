#include "wifi/wifi_manager.h"
#include "web_server/web_server_manager.h"
#include <M5Stack.h>

DisplayManager display;
WiFiManager wifiManager(display);
WebServerManager webServerManager(wifiManager);

void setup() {
    M5.begin();
    wifiManager.startAP();
    webServerManager.begin();
}

void loop() {
    webServerManager.handleClient();
}
