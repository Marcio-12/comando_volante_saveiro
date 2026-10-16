#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#define PINO_VOLANTE 36 // GPIO 36 (Pino VP do ESP32)

// ---------------------------------------------------------------------
// CALIBRAÇÃO DOS BOTOES (INSERIR OS VALORES AQUI APOS MEDIÇÃO)
// O ADC do ESP32 le de 0 a 4095 (0 V a 3,3 V). Altere os limites MIN e MAX:
// ---------------------------------------------------------------------

// --- BOTAO: AUMENTAR VOLUME ---
const int VOL_UP_ADC_MIN = 1000; // <-- EDITAR AQUI (Minimo ADC)
const int VOL_UP_ADC_MAX = 1300; // <-- EDITAR AQUI (Maximo ADC)

// --- BOTAO: DIMINUIR VOLUME ---
const int VOL_DOWN_ADC_MIN = 1800; // <-- EDITAR AQUI (Minimo ADC)
const int VOL_DOWN_ADC_MAX = 2100; // <-- EDITAR AQUI (Maximo ADC)

// --- BOTAO: PROXIMA FAIXA ---
const int NEXT_TRACK_ADC_MIN = 2500; // <-- EDITAR AQUI (Minimo ADC)
const int NEXT_TRACK_ADC_MAX = 2800; // <-- EDITAR AQUI (Maximo ADC)

// --- BOTAO: FAIXA ANTERIOR ---
const int PREV_TRACK_ADC_MIN = 3000; // <-- EDITAR AQUI (Minimo ADC)
const int PREV_TRACK_ADC_MAX = 3300; // <-- EDITAR AQUI (Maximo ADC)

// --- BOTAO: PLAY / PAUSE / MUTE ---
const int PLAY_PAUSE_ADC_MIN = 3500; // <-- EDITAR AQUI (Minimo ADC)
const int PLAY_PAUSE_ADC_MAX = 3800; // <-- EDITAR AQUI (Maximo ADC)

// ---------------------------------------------------------------------
// CONFIGURAÇÕES TÉCNICAS E TEMPOS
// ---------------------------------------------------------------------
const unsigned long TEMPO_DEBOUNCE_MS = 250;
const int QUANTIDADE_LEITURAS_MEDIA = 10;

#endif // CONFIGURACAO_H
