#include "sensors.h"

#include <ESP8266WiFi.h>
#include <time.h>

#include <DHT.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "time_utils.h"
#include "MQ135_corr.h"
#include "parameters.h"

// ===== Configurações =====
#define DHTPIN D4
#define DHTTYPE DHT11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// ===== Objetos =====
DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);
MQ135_corr mq135;

// ===== Variáveis Globais =====
float temperatura = NAN;
float umidade = NAN;

int mq135_raw_adc = 0;
int mq135_raw_corrigido_adc = 0;

float ppm_corrigido = NAN;
float rzero_estimado = NAN;

// ===== Estados =====
bool dht_ok = false;
bool mq135_ok = false;
bool oled_ok = false;

// ===== Controle OLED =====
unsigned long lastOLEDPageChange = 0;
unsigned long lastOLEDUpdate =0;

uint8_t oledPage = 0;

// =====================================================
// Inicialização
// =====================================================

void initSensors() {

    dht.begin();

    Serial.println(F("[DHT] Inicializado."));

    // OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS)) {

        oled_ok = false;

        Serial.println(F("[OLED] Nao encontrado."));
    }
    else {

        oled_ok = true;

        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(1);

        display.setCursor(0, 0);
        display.println(F("OLED OK"));

        display.display();

        Serial.println(F("[OLED] Inicializado."));
    }
}

// =====================================================
// Leitura Sensores
// =====================================================

void readSensors() {

    // -------------------------------
    // DHT11
    // -------------------------------

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {

        dht_ok = false;

        Serial.println(F("[DHT] Falha na leitura."));
    }
    else {

        temperatura = t;
        umidade = h;

        dht_ok = true;
    }

    // -------------------------------
    // MQ135
    // -------------------------------

    mq135_raw_adc = analogRead(A0);

    mq135_raw_corrigido_adc =
        constrain(
        round(mq135_raw_adc * 1.5),
        0,
        1023
    );

    if (mq135_raw_adc <= 0 ||
        mq135_raw_adc >= 1023) {

        mq135_ok = false;

        ppm_corrigido = NAN;
        rzero_estimado = NAN;

        Serial.println(F("[MQ135] Leitura invalida."));
    }
    else {

        mq135_ok = true;

        if (dht_ok) {

            ppm_corrigido =
                mq135.getCorrectedPPM(
                    mq135_raw_corrigido_adc,
                    temperatura,
                    umidade
                );

            rzero_estimado =
                mq135.getRZero(
                    mq135_raw_corrigido_adc,
                    temperatura,
                    umidade
                );
        }
        else {

            // Continua funcionando,
            // porém sem compensação.

            ppm_corrigido =
                mq135.getPPM(
                    mq135_raw_corrigido_adc
                );

            rzero_estimado = NAN;
        }
    }

}


// =====================================================
// OLED
// =====================================================

void updateOLED() {

    if (!oled_ok) {
        return;
    }

    unsigned long now = millis();


    lastOLEDUpdate = now;

    // Alterna páginas a cada 3 s.
    if (now - lastOLEDPageChange >= INTERVALO_OLED_PAGINAS) {

        oledPage++;

        if (oledPage > 2) {
            oledPage = 0;
        }

        lastOLEDPageChange = now;
    }

    display.clearDisplay();

    display.setTextSize(1);

    // =================================================
    // NOTA ARQUITETURAL
    //
    // Atualmente o OLED está implementado
    // em sensors.cpp por simplicidade,
    // uma vez que este projeto tende a
    // permanecer pequeno.
    //
    // Caso futuramente sejam adicionados:
    //
    // - menus;
    // - animações;
    // - múltiplos displays;
    // - botões;
    // - páginas configuráveis;
    //
    // recomenda-se migrar toda esta
    // lógica para oled.cpp/oled.h.
    // =================================================

    // -------------------------------
    // Cabeçalho
    // -------------------------------

    display.setCursor(0, 0);

    display.print(F("IP:"));

    if (WiFi.status() == WL_CONNECTED) {

        display.println(
            WiFi.localIP()
        );
    }
    else {

        display.println(F("SEM WIFI"));
    }

    display.print(F("Hora:"));

    display.println(
        getTimeString()
    );

    display.drawLine(
        0,
        18,
        128,
        18,
        SSD1306_WHITE
    );

    // -------------------------------
    // Conteúdo alternado
    // -------------------------------

    display.setTextSize(2);

    display.setCursor(0, 28);

    switch (oledPage) {

        case 0:

            display.println(F("PPM"));

            display.setCursor(0, 48);

            if (mq135_ok &&
                !isnan(ppm_corrigido)) {

                display.print(
                    ppm_corrigido,
                    1
                );
            }
            else {

                display.print(F("ERRO"));
            }

            break;

        case 1:

            display.println(F("TEMP"));

            display.setCursor(0, 48);

            if (dht_ok) {

                display.print(
                    temperatura,
                    1
                );

                display.print(F("C"));
            }
            else {

                display.print(F("ERRO"));
            }

            break;

        case 2:

            display.println(F("UMID"));

            display.setCursor(0, 48);

            if (dht_ok) {

                display.print(
                    umidade,
                    0
                );

                display.print(F("%"));
            }
            else {

                display.print(F("ERRO"));
            }

            break;
    }

    display.display();
}

float getCurrentRZero() {
    return mq135.getRZeroValue();
}

void setCurrentRZero(float rzero) {
    mq135.setRZero(rzero);
}
