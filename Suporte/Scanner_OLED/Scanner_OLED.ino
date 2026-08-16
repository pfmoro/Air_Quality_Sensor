#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 // Comum para displays OLED 0.96"

// Cria o objeto do display (sem pino de reset externo)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Inicializa o OLED com o endereço I2C 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("ERRO: OLED SSD1306 nao encontrado. Verifique a fiação!"));
    while (1); // Para aqui se não encontrar
  }

  Serial.println(F("OLED detectado com sucesso!"));
  
  // Limpa o buffer
  display.clearDisplay();

  // Configura texto
  display.setTextSize(2);      // Tamanho da fonte
  display.setTextColor(WHITE); // Cor do texto
  display.setCursor(30, 20);   // Posição (coluna, linha)

  // Escreve o texto
  display.println(F("OK!"));

  // Envia para o display
  display.display();
}

void loop() {
  // Nada para fazer aqui
}