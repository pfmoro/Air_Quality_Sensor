#include "time_utils.h"
#include <Arduino.h>
#include <time.h>

String getTimeString(bool includeSeconds) {
    time_t now = time(nullptr);

    // Se o NTP sincronizou a hora do WiFi (timestamp > 100000)
    if (now > 100000) {
        struct tm* t = localtime(&now);
        char buffer[10];

        if (includeSeconds) {
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
        } else {
            snprintf(buffer, sizeof(buffer), "%02d:%02d", t->tm_hour, t->tm_min);
        }
        return String(buffer);
    }

    // Fallback: Uptime do sistema via millis()
    unsigned long totalSegundos = millis() / 1000;
    unsigned int horas = totalSegundos / 3600;
    unsigned int minutos = (totalSegundos % 3600) / 60;
    unsigned int segundos = totalSegundos % 60;

    char buffer[16];
    if (includeSeconds) {
        snprintf(buffer, sizeof(buffer), "UP %02u:%02u:%02u", horas, minutos, segundos);
    } else {
        snprintf(buffer, sizeof(buffer), "UP %02u:%02u", horas, minutos);
    }

    return String(buffer);
}