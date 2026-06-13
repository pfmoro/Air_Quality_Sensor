#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "MQ135_corr.h"


// ===== Leituras =====
extern float temperatura;
extern float umidade;

extern MQ135_corr mq135;

extern int mq135_raw_adc;
extern int mq135_raw_corrigido_adc;

extern float ppm_corrigido;
extern float rzero_estimado;

// ===== Estados =====
extern bool dht_ok;
extern bool mq135_ok;
extern bool oled_ok;

// ===== Interface =====
void initSensors();
void readSensors();
void updateOLED();

// Hora formatada para OLED
String getDisplayTime();


// API para MQ135
float getCurrentRZero();
void setCurrentRZero(float rzero);


#endif
