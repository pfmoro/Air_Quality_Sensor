#ifndef WEB_H
#define WEB_H

#include <Arduino.h>

void initWeb();

void handleWeb();

String getJSON();
extern int fieldMap[4];

#endif