#include "wifi/wifi_manager.h"
#include "web_server/web_server_manager.h"
#include "preference/preference_manager.h"
#include <M5Stack.h>
#include <JPEGDecoder.h>
#include <WiFiUdp.h>

DisplayManager display;
PreferenceManager *preferenceManager = PreferenceManager::getInstance();
WiFiManager wifiManager(display, preferenceManager);
WebServerManager webServerManager(wifiManager);

// APモードかどうか
bool isAPMode = false;

// 映像受信用UDP
WiFiUDP udpVideo;

// 映像受信用ポート
const int VIDEO_PORT = 5005;

// JPEG画像の最大サイズ
const int MAX_IMAGE_SIZE = 8192 * 2;

// 受信データを蓄積するバッファ
uint8_t imageBuffer[MAX_IMAGE_SIZE];

int receivedSize = 0;
jpeg_div_t scale = JPEG_DIV_NONE;

void displayJPEG(uint8_t *data, int len);

void setup()
{
    Serial.begin(115200);

    M5.begin();

    if (!wifiManager.connectToSavedWiFi())
    {
        wifiManager.startAP();
        webServerManager.begin();
        isAPMode = true;
    }
    else
    {
        isAPMode = false;
        udpVideo.begin(VIDEO_PORT);
    }
}

void loop()
{
    if (isAPMode)
    {
        webServerManager.handleClient();
    }
    else
    {
        // UDP 映像データの受信
        int packetSize = udpVideo.parsePacket();
        if (packetSize > 0)
        {
            int len = udpVideo.read(imageBuffer + receivedSize, MAX_IMAGE_SIZE - receivedSize);

            if (len > 0)
            {
                receivedSize += len;
                if (receivedSize >= MAX_IMAGE_SIZE)
                {
                    Serial.println("Image size is too large. Aborting...");
                    receivedSize = 0;
                    JpegDec.abort();
                }
                else
                {
                    Serial.printf("Received packet: %d bytes, Total: %d bytes\n", len, receivedSize);

                    // 終了パケットの判定（適切な方法で決める）
                    // JPEG形式の開始マーカーと終了マーカーで判定したい
                    // 受信したパケットサイズが小さくなったら終了とみなす
                    if (len < 1024)
                    {
                        Serial.println("Complete image received. Decoding...");
                        M5.Lcd.drawJpg(imageBuffer, receivedSize, 0, 0, 0, 0, 0, 0, scale);

                        // バッファをリセット
                        receivedSize = 0;
                        JpegDec.abort();
                    }
                }
            }
        }
    }
}
