#include <WiFi.h>
#include <WebServer.h>
#include <M5Stack.h>

// 設定用のWebサーバー
WebServer server(80);

void displayWiFiInfo(); // LCDにAP情報を表示
void handleRoot();
void handleScan();
void handleSetWiFi();

void setup()
{
    M5.begin();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Starting AP Mode...");

    // M5StackをWi-Fi APモードにする
    const char *apSSID = "M5Stack_Setup";
    const char *apPassword = "12345678"; // 任意のパスワード
    WiFi.softAP(apSSID, apPassword);

    // APモードのIPアドレスを取得
    IPAddress apIP = WiFi.softAPIP();
    Serial.printf("AP Mode: SSID=%s, IP=%s\n", apSSID, apIP.toString().c_str());

    // LCDにWi-Fi情報を表示
    displayWiFiInfo();

    // Webサーバーのエンドポイントを設定
    server.on("/", handleRoot);
    server.on("/scan", handleScan); // **SSIDスキャン結果を取得**
    server.on("/setwifi", HTTP_POST, handleSetWiFi);
    server.begin();
    Serial.println("Web Server started");
}

void loop()
{
    server.handleClient();
}

// **Wi-Fi接続情報をLCDに表示**
void displayWiFiInfo()
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Wi-Fi Setup Mode");
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.print("SSID: M5Stack_Setup");
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.print("Password: 12345678");
    M5.Lcd.setCursor(10, 90);
    M5.Lcd.print("Connect to Wi-Fi and");
    M5.Lcd.setCursor(10, 110);
    M5.Lcd.print("open browser:");
    M5.Lcd.setCursor(10, 140);
    M5.Lcd.print("http://192.168.4.1/");
}

// **Wi-Fi設定ページ（SSIDリストを取得可能）**
void handleRoot()
{
    server.send(200, "text/html",
                "<html><body>"
                "<h2>Wi-Fi Settings</h2>"
                "<button onclick='scanWiFi()'>Scan Wi-Fi</button>"
                "<ul id='ssidList'></ul>"
                "<form action='/setwifi' method='POST'>"
                "SSID: <input type='text' id='ssid' name='ssid'><br>"
                "Password: <input type='password' name='password'><br>"
                "<input type='submit' value='Connect'>"
                "</form>"
                "<script>"
                "function scanWiFi() {"
                " fetch('/scan').then(response => response.json()).then(data => {"
                "   let list = document.getElementById('ssidList');"
                "   list.innerHTML = '';"
                "   data.forEach(ssid => {"
                "     let item = document.createElement('li');"
                "     item.innerHTML = `<a href='#' onclick='selectSSID(\"${ssid}\")'>${ssid}</a>`;"
                "     list.appendChild(item);"
                "   });"
                " });"
                "}"
                "function selectSSID(ssid) {"
                " document.getElementById('ssid').value = ssid;"
                "}"
                "</script>"
                "</body></html>");
}

// **Wi-Fiスキャン結果をJSONで返す**
void handleScan()
{
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; i++)
    {
        if (i > 0)
            json += ",";
        json += "\"" + WiFi.SSID(i) + "\"";
    }
    json += "]";
    server.send(200, "application/json", json);
}

// **Wi-Fi接続設定を処理**
void handleSetWiFi()
{
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0)
    {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.print("Connecting to Wi-Fi...");
        Serial.printf("Connecting to SSID: %s\n", ssid.c_str());

        // Wi-Fi接続
        WiFi.softAPdisconnect(true); // APモードを終了
        WiFi.begin(ssid.c_str(), password.c_str());

        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 20)
        {
            delay(500);
            Serial.print(".");
            M5.Lcd.print(".");
            retry++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nConnected!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());

            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(10, 10);
            M5.Lcd.print("Wi-Fi Connected!");
            M5.Lcd.setCursor(10, 40);
            M5.Lcd.print(WiFi.localIP().toString());

            server.send(200, "text/html", "<h3>Connected! IP: " + WiFi.localIP().toString() + "</h3>");
        }
        else
        {
            Serial.println("\nWi-Fi connection failed!");
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(10, 10);
            M5.Lcd.print("Wi-Fi Failed!");

            server.send(200, "text/html", "<h3>Connection Failed!</h3>");
        }
    }
}
