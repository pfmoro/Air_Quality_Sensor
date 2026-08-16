#include "wifi.h"

#include <ESP8266WiFi.h>

#include "config.h"

void initWiFi()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASS);

    Serial.print(F("Conectando ao WiFi"));

   unsigned long start = millis();

while (WiFi.status() != WL_CONNECTED)
{
    delay(500);
    Serial.print('.');

    if (millis() - start > 30000)
    {
        Serial.println();
        Serial.println(F("Falha ao conectar ao WiFi."));
        return;
    }
}

    Serial.println();
    Serial.println(F("WiFi conectado."));
    Serial.print(F("SSID: "));
    Serial.println(WIFI_SSID);

    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());

    Serial.print(F("RSSI: "));
    Serial.print(WiFi.RSSI());
    Serial.println(F(" dBm"));
}

bool wifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String getIPAddress()
{
    if (!wifiConnected())
        return "";

    return WiFi.localIP().toString();
}