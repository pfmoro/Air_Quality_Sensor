#include <ESP8266WiFi.h>

// Configurações de rede
const char* ssid = "Rede";
const char* password = "Senha";

// Variável de Timeout em segundos
const int timeoutSegundos = 20;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("\n--- Teste de Conexão Wi-Fi ---");

  WiFi.begin(ssid, password);
  Serial.print("Conectando");

  // Marca o tempo de início
  unsigned long tempoInicio = millis();

  // Tenta conectar enquanto o status for diferente de conectado 
  // E o tempo decorrido for menor que o timeout definido
  while (WiFi.status() != WL_CONNECTED && (millis() - tempoInicio < (timeoutSegundos * 1000))) {
    delay(500);
    Serial.print(".");
  }

  // Verifica o resultado final após sair do laço
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[SUCESSO] Conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[FALHA] Não foi possível conectar dentro do tempo limite.");
    Serial.println("Verifique o SSID, Senha ou o sinal da rede.");
  }
}

void loop() {
  // Nada a fazer no loop para este teste
}