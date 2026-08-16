#include "wifi.h"

#include <ESP8266WiFi.h>

#include "config.h"

void initWiFi()
{
    WiFi.mode(WIFI_STA); // É altamente recomendável manter ativo para garantir o modo cliente

    // 1. Correção da checagem: Verifica se o primeiro caractere não é nulo (ou seja, se não está vazio "")
    if (BSSID[0] != '\0') {
        Serial.println("BSSID detectado. Convertendo string para array...");
        
        uint8_t bssid[6];
        
        // 2. Correção do sscanf: Removidos os "0x" da máscara para ler o formato padrão "AA:BB:CC..."
        int parsed = sscanf(BSSID, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
                            &bssid[0], &bssid[1], &bssid[2], &bssid[3], &bssid[4], &bssid[5]);

        // Se a conversão leu os 6 blocos com sucesso, força a conexão com BSSID
        if (parsed == 6) {
            Serial.printf("Conectando via BSSID forçado: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                          bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
          
            WiFi.begin(WIFI_SSID, WIFI_PASS, 0, bssid);
        } else {
            Serial.println("Erro no formato do BSSID! Tentando conexão padrão...");
            WiFi.begin(WIFI_SSID, WIFI_PASS);
        }
    } else {
        // Se a string for "", inicia a conexão normal
        Serial.println("Nenhum BSSID fornecido. Tentando conexão normal padrão...");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }

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