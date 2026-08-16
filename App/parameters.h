#ifndef PARAMETERS_H
#define PARAMETERS_H

// =====================================================
// INTERVALOS
// =====================================================

// Aquisição dos sensores
const unsigned long INTERVALO_SENSORES = 5000;

// Atualização máxima do OLED
const unsigned long INTERVALO_OLED = 1000;

// Alternância das páginas do OLED
const unsigned long INTERVALO_OLED_PAGINAS = 3000;

// Envio ao ThingSpeak
const unsigned long INTERVALO_THINGSPEAK = 600000;

//Valores padrões do FieldMAP do thingspeak:
constexpr int DEFAULT_FIELDMAP[4] = {
    0, 0, 8, 0
};


// =====================================================
// SEGURANÇA
// =====================================================

// Limite mínimo aceitável para heap livre.
// Atualmente não utilizado, mas mantido como
// ponto de expansão futura.
#define CRITICAL_HEAP_LIMIT 8000

#endif