#include <WiFi.h>
#include <WebServer.h>
#include <M5Stack.h>

WebServer server(80);

void handleRoot()
{
    server.send(200, "text/html",
                "<html><body>"
                "<h2>Wi-Fi Settings</h2>"
                "<form action='/setwifi' method='POST'>"
                "SSID: <input type='text' name='ssid'><br>"
                "Password: <input type='password' name='password'><br>"
                "<input type='submit' value='Connect'>"
                "</form>"
                "</body></html>");
}

void handleSetWiFi()
{
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0)
    {
        WiFi.begin(ssid.c_str(), password.c_str());

        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 20)
        {
            delay(500);
            retry++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            server.send(200, "text/html", "<h3>Connected! IP: " + WiFi.localIP().toString() + "</h3>");
        }
        else
        {
            server.send(200, "text/html", "<h3>Connection Failed!</h3>");
        }
    }
}

void setup()
{
    M5.begin();
    WiFi.softAP("M5Stack_Setup"); // M5StackをWi-Fi APにする
    server.on("/", handleRoot);
    server.on("/setwifi", HTTP_POST, handleSetWiFi);
    server.begin();
}

void loop()
{
    server.handleClient();
}
