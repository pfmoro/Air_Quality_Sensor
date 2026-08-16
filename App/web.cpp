#include "web.h"

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <time.h>

#include "time_utils.h"
#include "sensors.h"
#include "thingspeak_client.h"

ESP8266WebServer server(80);

// =====================================================
// JSON
// =====================================================

String getJSON() {

    char buffer[256];

    snprintf(
        buffer,
        sizeof(buffer),
        "{\"temperatura\":%.1f,"
        "\"umidade\":%.1f,"
        "\"ppm\":%.1f,"
        "\"rzero\":%.2f,"
        "\"wifi\":%s}",
        temperatura,
        umidade,
        ppm_corrigido,
        rzero_estimado,
        (WiFi.status() == WL_CONNECTED) ? "true" : "false"
    );

    return String(buffer);
}

// =====================================================
// /data
// =====================================================

void handleData() {

    server.send(
        200,
        "application/json",
        getJSON()
    );
}


// =====================================================
// /
// =====================================================

void handleRoot() {

    if (server.method() == HTTP_POST) {

        if (server.hasArg("rzero")) {

            float novoRZero =
                server.arg("rzero").toFloat();

            if (novoRZero > 0.0f) {

                setCurrentRZero(
                    novoRZero
                );

                Serial.print(
                    F("[WEB] Novo RZero: ")
                );

                Serial.println(
                    novoRZero
                );
            }
        }
    }

    server.setContentLength(
        CONTENT_LENGTH_UNKNOWN
    );

    server.send(
        200,
        "text/html",
        ""
    );

    server.sendContent(
        F("<html><body>")
    );

    server.sendContent(
        F("<h2>MQ135 Air Monitor</h2>")
    );

    server.sendContent(
        "<p><b>IP:</b> "
        + WiFi.localIP().toString()
        + "</p>"
    );

    server.sendContent(
        "<p><b>Hora:</b> "
        + getTimeString(true)
        + "</p>"
    );

    server.sendContent(
        "<p><b>WiFi:</b> "
        + String(
            WiFi.status() ==
            WL_CONNECTED ?
            "Conectado" :
            "Desconectado"
        )
        + "</p>"
    );

    server.sendContent(
        "<p><b>Temperatura:</b> "
        + String(temperatura, 1)
        + " C</p>"
    );

    server.sendContent(
        "<p><b>Umidade:</b> "
        + String(umidade, 1)
        + " %</p>"
    );

    server.sendContent(
        "<p><b>PPM:</b> "
        + String(ppm_corrigido, 1)
        + "</p>"
    );

    server.sendContent(
        "<p><b>RZero:</b> "
        + String(
            getCurrentRZero(),
            2
        )
        + "</p>"
    );

    // -------------------------
    // Calibração RZero
    // -------------------------

    server.sendContent(
        F("<h3>Calibracao RZero</h3>")
    );

    server.sendContent(
        F("<form method='POST'>")
    );

    server.sendContent(
        "RZero: "
        "<input name='rzero' value='"
        + String(
            getCurrentRZero(),
            2
        )
        + "'>"
    );

    server.sendContent(
        F("<input type='submit' value='Atualizar'>")
    );

    server.sendContent(
        F("</form>")
    );

    // -------------------------
    // Links
    // -------------------------

    server.sendContent(
        F("<hr>")
    );

    server.sendContent(
        F("<a href='/data'>JSON</a><br>")
    );

    server.sendContent(
        F("<a href='/thingspeak'>ThingSpeak</a><br>")
    );

    server.sendContent(
        F("</body></html>")
    );

    server.client().stop();
}

// =====================================================
// /thingspeak
// =====================================================

void handleThingSpeakPage() {

    if (server.method() == HTTP_POST) {

        for (int i = 0; i < 4; i++) {

            String arg =
                "f" + String(i + 1);

            if (server.hasArg(arg)) {

                fieldMap[i] =
                    server.arg(arg).toInt();
            }
        }

        if (server.hasArg("intervalo")) {

            unsigned long intervalo =
                server.arg(
                    "intervalo"
                ).toInt();

            if (intervalo >= 15000) {

                thingSpeakInterval =
                    intervalo;
            }
        }
    }

    server.setContentLength(
        CONTENT_LENGTH_UNKNOWN
    );

    server.send(
        200,
        "text/html",
        ""
    );

    server.sendContent(
        F("<html><body>")
    );

    server.sendContent(
        F("<h2>ThingSpeak</h2>")
    );

    server.sendContent(
        F("<form method='POST'>")
    );

    const char* nomes[4] = {

        "Temperatura",
        "Umidade",
        "PPM",
        "RZero"

    };

    for (int i = 0; i < 4; i++) {

        server.sendContent(
            nomes[i]
        );

        server.sendContent(
            " -> Field "
        );

        server.sendContent(
            "<input name='f"
            + String(i + 1)
            + "' value='"
            + String(fieldMap[i])
            + "'><br>"
        );
    }

    server.sendContent(
        "<br>Intervalo (ms): "
        "<input name='intervalo' value='"
        + String(
            thingSpeakInterval
        )
        + "'><br><br>"
    );

    server.sendContent(
        F("<input type='submit' value='Salvar'>")
    );

    server.sendContent(
        F("</form>")
    );

    server.sendContent(
        F("<br><a href='/'>Voltar</a>")
    );

    server.sendContent(
        F("</body></html>")
    );

    server.client().stop();
}

// =====================================================
// Inicialização
// =====================================================

void initWeb() {

    server.on(
        "/",
        handleRoot
    );

    server.on(
        "/data",
        handleData
    );

    server.on(
        "/thingspeak",
        handleThingSpeakPage
    );

    server.begin();

    Serial.println(
        F("[WEB] Servidor iniciado."));

    Serial.println(F("[IP]: "));
    Serial.println(WiFi.localIP());
   
}

// =====================================================
// Loop
// =====================================================

void handleWeb() {

    server.handleClient();
}