#include <Wire.h>

void setup() {
  Wire.begin(); // Inicializa a comunicação I2C
  
  // No NodeMCU/ESP8266, se você usar pinos diferentes do padrão:
  // Wire.begin(D2, D1); // SDA, SCL
  
  Serial.begin(115200);
  while (!Serial); // Aguarda a abertura do monitor serial
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Escaneando...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // O i2c_scanner usa o valor de retorno do
    // Write.endTransmisstion para saber se um dispositivo respondeu ao endereço.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado no endereço 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Erro desconhecido no endereço 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  if (nDevices == 0)
    Serial.println("Nenhum dispositivo I2C encontrado\n");
  else
    Serial.println("Concluído\n");

  delay(5000); // Aguarda 5 segundos para o próximo scan
}