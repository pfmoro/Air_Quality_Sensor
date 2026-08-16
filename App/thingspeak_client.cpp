#include "thingspeak_client.h"

#include <ESP8266WiFi.h>

#include "config.h"
#include "sensors.h"
#include "parameters.h"
#include <ThingSpeak.h>

// =====================================================
// Cliente TCP
// =====================================================

WiFiClient tsClient;

// =====================================================
// Configuração
// =====================================================

// Temperatura
// Umidade
// PPM Corrigido
// RZero Estimado
int fieldMap[4] = {
    DEFAULT_FIELDMAP[0],
    DEFAULT_FIELDMAP[1],
    DEFAULT_FIELDMAP[2],
    DEFAULT_FIELDMAP[3]
};

// obtem de parameters.h
unsigned long thingSpeakInterval =
    INTERVALO_THINGSPEAK;

// =====================================================
// Inicialização
// =====================================================

void initThingSpeak() {

    ThingSpeak.begin(tsClient);

    Serial.println(F("[ThingSpeak] Inicializado."));
}

// =====================================================
// Envio periódico
// =====================================================

void sendThingSpeak() {

    // Sem WiFi, não tenta enviar
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }


    // =================================================
    // Monta vetor de valores
    // =================================================

    float valores[4] = {
        temperatura,
        umidade,
        ppm_corrigido,
        rzero_estimado
    };

    // =================================================
    // Configura os campos válidos
    // =================================================

    for (int i = 0; i < 4; i++) {

        if (fieldMap[i] < 1 ||
            fieldMap[i] > 8) {

            continue;
        }

        if (isnan(valores[i])) {

            continue;
        }

        ThingSpeak.setField(
            fieldMap[i],
            valores[i]
        );
    }

    // =================================================
    // Envia ao ThingSpeak
    // =================================================

    int status = ThingSpeak.writeFields(
        THINGSPEAK_CHANNEL,
        THINGSPEAK_API_KEY
    );

    // =================================================
    // Diagnóstico
    // =================================================

    if (status == 200) {

        Serial.println(
            F("[ThingSpeak] Dados enviados.")
        );
    }
    else {

        Serial.print(
            F("[ThingSpeak] Erro ao enviar. Codigo: ")
        );

        Serial.println(status);
    }
}
