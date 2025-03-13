#include "display_manager.h"

DisplayManager::DisplayManager() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
}

void DisplayManager::showAPInfo(const String& ssid, const String& password, const String& ip) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Wi-Fi Setup Mode");
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print("SSID: " + ssid);
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.print("Password: " + password);
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.print("Access: http://" + ip);
}

void DisplayManager::showMessage(const String& message) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print(message);
}

void DisplayManager::showWiFiScanResults(const String& results) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Wi-Fi Networks:");
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print(results);
}

void DisplayManager::showWiFiConnectionStatus(const String& status) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Wi-Fi Status:");
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print(status);
}