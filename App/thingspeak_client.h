#ifndef THINGSPEAK_CLIENT_H
#define THINGSPEAK_CLIENT_H

#include <Arduino.h>

// =====================================================
// Configuração ThingSpeak
//
// fieldMap:
// [0] Temperatura
// [1] Umidade
// [2] PPM Corrigido
// [3] RZero Estimado
//
// O fieldMap é mantido apenas em RAM.
//
// Caso futuramente seja desejada persistência,
// recomenda-se LittleFS ou EEPROM.
//
// A perda do fieldMap não compromete a validade
// científica das leituras, diferentemente do
// parâmetro RZero.
// =====================================================

extern int fieldMap[4];


// Intervalo entre envios (ms)
// Default: 10 minutos
extern unsigned long thingSpeakInterval;

// =====================================================
// Interface
// =====================================================

void initThingSpeak();

void handleThingSpeak();

#endif
