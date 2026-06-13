#include "time_utils.h"

#include <time.h>

String getTimeString(bool includeSeconds) {

    time_t now = time(nullptr);

    if (now > 100000) {

        struct tm* t = localtime(&now);

        char buffer[10];

        if (includeSeconds) {

            snprintf(
                buffer,
                sizeof(buffer),
                "%02d:%02d:%02d",
                t->tm_hour,
                t->tm_min,
                t->tm_sec
            );

        } else {

            snprintf(
                buffer,
                sizeof(buffer),
                "%02d:%02d",
                t->tm_hour,
                t->tm_min
            );
        }

        return String(buffer);
    }

    // fallback: uptime

    unsigned long segundos =
        millis() / 1000;

    unsigned int horas =
        segundos / 3600;

    unsigned int minutos =
        (segundos % 3600) / 60;

    char buffer[12];

    snprintf(
        buffer,
        sizeof(buffer),
        "UP %02u:%02u",
        horas,
        minutos
    );

    return String(buffer);
}