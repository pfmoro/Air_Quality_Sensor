# Monitoramento Avançado de Qualidade do Ar (MQ‑135 + DHT11 + OLED) 💨

Esta placa é dedicada ao monitoramento da qualidade do ar (concentração de gases) usando o sensor [MQ-135](https://www.makerhero.com/blog/como-funciona-o-sensor-de-gas-mq-135/?srsltid=AfmBOorwAhi8It0JkbR7EZQjq7tZ1lGu5Q90_x1cF1DfJ-fndEwpIFdH)

NEsta versão atualizada refletiremos:

* A criação da biblioteca **`MQ135_corr`** (fork local da MQ135)
* O **processo correto de calibração (RZero)**
* A **leitura estimada de RZero via Serial**
* Reconexão automática de Wi‑Fi e diagnóstico de IP

O projeto evoluiu de uma leitura analógica simples para um sistema de monitoramento ambiental **com compensação atmosférica, calibração explícita e telemetria em nuvem**.

---

## 🔩 Implementações de Hardware

### Sensores e Proteção Elétrica

* **MQ‑135** – Sensor de qualidade do ar (CO₂ equivalente / VOCs)
* **DHT11** – Sensor de temperatura e umidade (usado para correção química)
* **Divisor de tensão no A0** – Protege a entrada analógica da NodeMCU (3,3 V máx)

> ⚠️ O MQ‑135 **deve ser alimentado em 5 V (VIN)** para funcionamento correto do aquecedor interno.

### Tabela de Pinagem

| Componente   | Pino NodeMCU     | Observação                    |
| ------------ | ---------------- | ----------------------------- |
| MQ‑135 (VCC) | **VIN**          | Alimentação 5 V (obrigatório) |
| MQ‑135 (GND) | **GND**          | Terra comum                   |
| MQ‑135 (A0)  | **Divisor → A0** | Proteção do ADC               |
| DHT11 (VCC)  | **3V3**          | Alimentação lógica            |
| DHT11 (DATA) | **D3**           | Comunicação digital           |
| OLED (GND)   | **GND**          | Terra comum                   |
| OLED (VCC)   | **3V3**          | ALimentação Lógica            |
| OLED (SDA)   | **D1**           | Padrão I2C                    |
| OLED (SCL)   | **D2**           | Padrão I2C                    |

### Divisor de Tensão (Proteção A0)

Configuração utilizada:

* **R1 = 1 kΩ** (entre MQ‑135 A0 e A0 da NodeMCU)
* **R2 = 2 kΩ** (dois resistores de 1 kΩ em série entre A0 e GND)

Isso resulta em um fator aproximado de:

```
Vout = Vin × (2k / (1k + 2k)) ≈ 0,66
```

No software, a leitura é **re‑escalonada por 1,5** para compensar o divisor.

---

## 🚀 Software – Visão Geral

O firmware realiza:

1. **Leitura analógica do MQ‑135** (com correção do divisor)
2. **Leitura de temperatura e umidade (DHT11)**
3. **Cálculo de PPM corrigido** usando `MQ135_corr`
4. **Estimativa dinâmica de RZero**
5. **Servidor Web (JSON)**
6. **Envio periódico ao ThingSpeak**
7. **Reconexão automática de Wi‑Fi + log de IP**

---

## 📚 Biblioteca `MQ135_corr`

### O que é?

`MQ135_corr` é um **fork local** da biblioteca MQ135 tradicional, criado para:

* Tornar explícito o valor de **RZERO**
* Facilitar **calibração manual baseada em medições reais**
* Expor a função de **estimativa de RZero em tempo real**

📁 O arquivo **`MQ135_corr.h`** deve ficar **no mesmo diretório do `.ino`**.

---

## 🔥 Processo de Burn‑In e Calibração (ESSENCIAL)

### 1️⃣ Burn‑In do Sensor

Antes de qualquer calibração:

* **Mínimo funcional:** 20 minutos ligados
* **Calibração real:** **≥ 24 horas contínuas**
* Ambiente: **ar limpo, externo ou bem ventilado**

> Durante esse período, **não confie nos valores de PPM**.

---

### 2️⃣ Leitura do RZero Estimado

Após o burn‑in, o código imprime no Serial:

```
RZero Estimado: 76.43 kΩ  [VALIDO APÓS BURN-IN ≥ 24h EM AR LIMPO]
```

Esse valor representa o **RZero real do seu sensor específico**.



---

## ☁️ ThingSpeak

* Envio automático a cada **10 minutos**
* **Field 8** → `ppm_corrigido`

Configuração no código:

```cpp
const unsigned long timerDelay = 600000;
```

---

## 📡 Wi‑Fi – Diagnóstico e Reconexão

O firmware:

* Exibe progresso de conexão
* Mostra o **IP recebido**
* Detecta quedas de conexão
* Reconecta automaticamente

Exemplo no Serial:

```
[SISTEMA] WiFi conectado
[SISTEMA] IP obtido: 192.168.1.42
[WiFi] Conexão perdida! Tentando reconectar...
[WiFi] Reconectado com sucesso. IP: 192.168.1.42
```

---

## 📌 Observações Importantes

* MQ‑135 **não mede CO₂ real**, mas um equivalente baseado em VOCs
* Resultados são **comparativos e ambientais**, não laboratoriais
* Calibração correta **impacta drasticamente a qualidade dos dados**



🧠 Arquitetura do Sistema (Firmware)

O firmware foi modularizado para separar claramente responsabilidades entre sensores, comunicação, interface e visualização.

A arquitetura segue o padrão:
```
main.ino
│
├── sensors (MQ135 + DHT11 + leitura ADC)
├── wifi manager (conexão e reconexão)
├── web server (API JSON + interface HTML)
├── display OLED (visualização local)
└── cloud (ThingSpeak)
🧩 Módulos do Sistema
🌡️ sensors.cpp / sensors.h
```
Responsável por toda a camada de aquisição de dados.

Funções principais:
Leitura do MQ-135 (ADC + correção de divisor)
Leitura do DHT11 (temperatura e umidade)
Cálculo de:
PPM corrigido
RZero estimado (via biblioteca MQ135_corr)
Filtragem básica de ruído (média móvel simples, quando aplicada)
Saída do módulo:

Estrutura de dados central:
```
struct SensorData {
    float temperature;
    float humidity;
    float mq135_adc_raw;
    float mq135_adc_corrected;
    float ppm;
    float rzero;
};
```

📡 wifi.cpp / wifi.h

Gerencia toda a conectividade Wi-Fi.

Responsabilidades:
Retry automático em caso de falha
Reconexão em runtime (watchdog simples)
Log de IP obtido via Serial
Comportamento:
Em caso de queda:
tenta reconectar continuamente
não bloqueia o loop principal
🌐 Web Server (web.cpp)

📡 config.h
cadastro da Conexão inicial com SSID/Senha (via wifi.h)

Implementa uma interface local acessível via navegador.

📍 Endpoint principal: /

Retorna JSON com todos os dados do sistema:
```json
{
  "temperature": 25.4,
  "humidity": 61.2,
  "mq135_adc_raw": 312,
  "mq135_adc_corrected": 468,
  "ppm": 11.8,
  "rzero": 76.4,
  "wifi_ip": "192.168.1.42"
}
```
📊 Endpoint futuro (opcional / expansão):
/dashboard → interface HTML leve (modo debug visual)
/config → ajustes remotos (futuro)
/metrics → apenas dados brutos para integração
💡 Papel do Web Server

Ele funciona como:

Interface de debug local
Gateway para integração externa (Python, Node-RED, etc.)
Fonte de dados para automações futuras
🖥️ Display OLED (SSD1306 – I2C 0x3C)

O display OLED atua como interface local em tempo real.

Função principal:

Exibir os dados críticos sem necessidade de Wi-Fi ou acesso web.

📊 Layout do display (modo atual)

O firmware alterna entre telas:
```
Tela 1 – Estado do ar
MQ-135
PPM: 12.3
Tela 2 – Ambiente
Temp: 25.4 C
Hum:  61 %
Tela 3 – Sistema
WiFi OK
192.168.1.42
```

🔄 Atualização
Refresh típico: ~1–2 segundos
Alternância de telas em loop
Escrita otimizada para evitar flicker
📌 Papel do OLED no sistema
Feedback imediato sem rede
Diagnóstico rápido em campo
Visualização portátil do sensor
☁️ Integração entre módulos

O fluxo geral do sistema é:
```
[SENSORS]
   ↓
[sensors.cpp]
   ↓
[main.ino orchestration]
   ↓
 ┌──────────────┬──────────────┐
 │ OLED display │ Web server   │
 └──────────────┴──────────────┘
        ↓
   ThingSpeak (cloud)
```
🔁 Ciclo de Execução

O firmware opera em loop contínuo:

Lê sensores (MQ135 + DHT11)
Atualiza estrutura de dados global
Atualiza OLED
Responde requisições HTTP
A cada N minutos:
envia dados para ThingSpeak
Monitora Wi-Fi em background
🧠 Observação de arquitetura

O sistema foi projetado com foco em:

Baixo acoplamento entre módulos
Facilidade de debug via Serial + Web
Escalabilidade (novos sensores podem ser adicionados sem alterar lógica central)
🚀 Resultado final

O sistema evolui de um sensor MQ-135 isolado para uma plataforma de monitoramento ambiental embarcada, com:

Visualização local (OLED)
Interface web (JSON)
Telemetria em nuvem
Estrutura modular extensível

---

---

## ✅ Checklist Final

* [ ] MQ‑135 ligado em **VIN (5 V)**
* [ ] Divisor de tensão montado corretamente
* [ ] Burn‑in ≥ 24h
* [ ] RZero observado e fixado em `MQ135_corr.h`
* [ ] Wi‑Fi estável
* [ ] ThingSpeak recebendo dados

---