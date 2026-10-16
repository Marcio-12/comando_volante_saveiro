#include "botoes.h"

#include <BleKeyboard.h>

BotoesVolante::BotoesVolante(uint8_t pino) : pino_(pino) {}

void BotoesVolante::iniciar() {
    pinMode(pino_, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(pino_, ADC_11db);

    const uint16_t primeiraLeitura =
        static_cast<uint16_t>(analogRead(pino_));
    for (uint8_t i = 0; i < MAX_AMOSTRAS; ++i) {
        amostras_[i] = primeiraLeitura;
    }

    somaAmostras_ = static_cast<uint32_t>(primeiraLeitura) * MAX_AMOSTRAS;
    leituraFiltrada_ = primeiraLeitura;
    candidato_ = mapearLeitura(primeiraLeitura);
    estavel_ = candidato_;
    iniciado_ = true;
}

uint16_t BotoesVolante::lerMediaMovel() {
    const uint16_t leitura = static_cast<uint16_t>(analogRead(pino_));

    somaAmostras_ -= amostras_[indiceAmostra_];
    amostras_[indiceAmostra_] = leitura;
    somaAmostras_ += leitura;
    indiceAmostra_ =
        static_cast<uint8_t>((indiceAmostra_ + 1) % MAX_AMOSTRAS);

    return static_cast<uint16_t>(somaAmostras_ / MAX_AMOSTRAS);
}

BotaoVolante BotoesVolante::mapearLeitura(uint16_t leitura) const {
    if (leitura >= VOL_UP_ADC_MIN && leitura <= VOL_UP_ADC_MAX) {
        return BotaoVolante::VolumeMais;
    }
    if (leitura >= VOL_DOWN_ADC_MIN && leitura <= VOL_DOWN_ADC_MAX) {
        return BotaoVolante::VolumeMenos;
    }
    if (leitura >= NEXT_TRACK_ADC_MIN && leitura <= NEXT_TRACK_ADC_MAX) {
        return BotaoVolante::ProximaFaixa;
    }
    if (leitura >= PREV_TRACK_ADC_MIN && leitura <= PREV_TRACK_ADC_MAX) {
        return BotaoVolante::FaixaAnterior;
    }
    if (leitura >= PLAY_PAUSE_ADC_MIN && leitura <= PLAY_PAUSE_ADC_MAX) {
        return BotaoVolante::PlayPause;
    }
    return BotaoVolante::Nenhum;
}

void BotoesVolante::enviarComando(BleKeyboard& teclado,
                                   BotaoVolante botao) const {
    switch (botao) {
    case BotaoVolante::VolumeMais:
        teclado.write(KEY_MEDIA_VOLUME_UP);
        break;
    case BotaoVolante::VolumeMenos:
        teclado.write(KEY_MEDIA_VOLUME_DOWN);
        break;
    case BotaoVolante::ProximaFaixa:
        teclado.write(KEY_MEDIA_NEXT_TRACK);
        break;
    case BotaoVolante::FaixaAnterior:
        teclado.write(KEY_MEDIA_PREVIOUS_TRACK);
        break;
    case BotaoVolante::PlayPause:
        teclado.write(KEY_MEDIA_PLAY_PAUSE);
        break;
    case BotaoVolante::Nenhum:
        break;
    }
}

void BotoesVolante::verificar(BleKeyboard& teclado, uint32_t agoraMs) {
    if (!iniciado_) {
        iniciar();
    }

    leituraFiltrada_ = lerMediaMovel();
    const BotaoVolante leituraAtual = mapearLeitura(leituraFiltrada_);

    if (leituraAtual != candidato_) {
        candidato_ = leituraAtual;
        inicioCandidatoMs_ = agoraMs;
        return;
    }

    if (candidato_ == estavel_ ||
        static_cast<uint32_t>(agoraMs - inicioCandidatoMs_) <
            TEMPO_DEBOUNCE_MS) {
        return;
    }

    const BotaoVolante estadoAnterior = estavel_;
    estavel_ = candidato_;

    if (estadoAnterior == BotaoVolante::Nenhum &&
        estavel_ != BotaoVolante::Nenhum && teclado.isConnected()) {
        enviarComando(teclado, estavel_);
    }
}

uint16_t BotoesVolante::leituraFiltrada() const {
    return leituraFiltrada_;
}

BotaoVolante BotoesVolante::botaoEstavel() const {
    return estavel_;
}
