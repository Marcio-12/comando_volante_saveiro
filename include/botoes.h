#ifndef BOTOES_H
#define BOTOES_H

#include <Arduino.h>

#include "configuracao.h"

class BleKeyboard;

enum class BotaoVolante : uint8_t {
    Nenhum,
    VolumeMais,
    VolumeMenos,
    ProximaFaixa,
    FaixaAnterior,
    PlayPause
};

class BotoesVolante {
public:
    explicit BotoesVolante(uint8_t pino = PINO_VOLANTE);

    void iniciar();
    void verificar(BleKeyboard& teclado, uint32_t agoraMs = millis());

    uint16_t leituraFiltrada() const;
    BotaoVolante botaoEstavel() const;

private:
    static constexpr uint8_t MAX_AMOSTRAS = QUANTIDADE_LEITURAS_MEDIA;

    uint16_t lerMediaMovel();
    BotaoVolante mapearLeitura(uint16_t leitura) const;
    void enviarComando(BleKeyboard& teclado, BotaoVolante botao) const;

    uint8_t pino_;
    uint16_t amostras_[MAX_AMOSTRAS]{};
    uint32_t somaAmostras_ = 0;
    uint8_t indiceAmostra_ = 0;
    uint16_t leituraFiltrada_ = 0;
    BotaoVolante candidato_ = BotaoVolante::Nenhum;
    BotaoVolante estavel_ = BotaoVolante::Nenhum;
    uint32_t inicioCandidatoMs_ = 0;
    bool iniciado_ = false;
};

#endif // BOTOES_H
