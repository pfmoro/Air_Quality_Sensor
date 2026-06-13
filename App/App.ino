#include "wifi.h"
#include "parameters.h"
#include "sensors.h"
#include "web.h"
#include "thingspeak_client.h"

void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println(F("================================="));
    Serial.println(F("MQ135 Air Monitor"));
    Serial.println(F("Inicializando..."));
    Serial.println(F("================================="));

    initWiFi();

    initSensors();

    initThingSpeak();

    initWeb();

    Serial.println(F("Sistema pronto."));
}

unsigned long lastSensorRead = 0;
unsigned long lastThingSpeak = 0;
unsigned long lastOledUpdate = 0;

void loop() {

    unsigned long now = millis();

    if (now - lastSensorRead >= INTERVALO_SENSORES) {

        lastSensorRead = now;

        readSensors();
    }

    if (now - lastThingSpeak >= thingSpeakInterval) {

        lastThingSpeak = now;

        sendThingSpeak();
    }

        if (now - lastOledUpdate >= INTERVALO_OLED) {

        lastOledUpdate = now;

        updateOLED();
    }

    handleWeb();

    yield();
}