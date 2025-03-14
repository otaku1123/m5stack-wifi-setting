#include "web_server_manager.h"
#include <M5Stack.h>

WebServerManager::WebServerManager(
    WiFiManager& wifiManager
) : server(80), wifiManager(wifiManager) {}

void WebServerManager::begin()
{
    server.on("/", std::bind(&WebServerManager::handleRoot, this));
    server.on("/scan", std::bind(&WebServerManager::handleScan, this));
    server.on("/setwifi", HTTP_POST, std::bind(&WebServerManager::handleSetWiFi, this));
    server.begin();
    Serial.println("Web Server started");
}

void WebServerManager::handleClient()
{
    server.handleClient();
}

void WebServerManager::handleRoot() {
    server.send(200, "text/html",
        "<html><body><h2>Wi-Fi Settings</h2>"
        "<button onclick='scanWiFi()'>Scan Wi-Fi</button>"
        "<ul id='ssidList'></ul>"
        "<form action='/setwifi' method='POST'>"
        "SSID: <input type='text' id='ssid' name='ssid'><br>"
        "Password: <input type='password' name='password'><br>"
        "<input type='submit' value='Connect'></form>"
        "<script>function scanWiFi() {fetch('/scan').then(response => response.json()).then(data => {let list = document.getElementById('ssidList'); list.innerHTML = ''; data.forEach(ssid => {let item = document.createElement('li'); item.innerHTML = `<a href='#' onclick='selectSSID(\"${ssid}\")'>${ssid}</a>`; list.appendChild(item);});});} function selectSSID(ssid) {document.getElementById('ssid').value = ssid;}</script>"
        "</body></html>");
}

void WebServerManager::handleScan() {
    server.send(200, "application/json", wifiManager.scanNetworks());
}

void WebServerManager::handleSetWiFi() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0) {
        wifiManager.connectToWiFi(ssid.c_str(), password.c_str());
        // server.send(200, "text/html", "<h3>Attempting to connect...</h3>");
        delay(2000);
        // note: 再起動は無くても良いかもしれない
        ESP.restart();
    }
}

