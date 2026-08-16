#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  delay(2000); // importante: DHT11 precisa de um tempo para estabilizar

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na inicialização/leitura do DHT (valores NaN).");
  } else {
    Serial.print("Leitura OK. Umidade: ");
    Serial.print(h);
    Serial.print("% | Temp: ");
    Serial.print(t);
    Serial.println("°C");
  }
}

void loop() {
}
