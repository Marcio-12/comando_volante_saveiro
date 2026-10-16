#include <Arduino.h>
#include <BleKeyboard.h>

#include "botoes.h"
#include "configuracao.h"

BleKeyboard tecladoBluetooth("Saveiro Volante", "Comando Volante", 100);
BotoesVolante botoes;
uint32_t ultimaTentativaBluetoothMs = 0;
constexpr uint32_t INTERVALO_RECONEXAO_BLE_MS = 5000;

void enviarComando(BotaoVolante botao) {
    switch (botao) {
    case BotaoVolante::VolumeMais:
        tecladoBluetooth.write(KEY_MEDIA_VOLUME_UP);
        break;
    case BotaoVolante::VolumeMenos:
        tecladoBluetooth.write(KEY_MEDIA_VOLUME_DOWN);
        break;
    case BotaoVolante::ProximaFaixa:
        tecladoBluetooth.write(KEY_MEDIA_NEXT_TRACK);
        break;
    case BotaoVolante::FaixaAnterior:
        tecladoBluetooth.write(KEY_MEDIA_PREVIOUS_TRACK);
        break;
    case BotaoVolante::PlayPause:
        tecladoBluetooth.write(KEY_MEDIA_PLAY_PAUSE);
        break;
    case BotaoVolante::Nenhum:
        break;
    }
}

void manterBluetoothDisponivel(uint32_t agoraMs) {
    if (tecladoBluetooth.isConnected()) {
        return;
    }

    if (static_cast<uint32_t>(agoraMs - ultimaTentativaBluetoothMs) >=
        INTERVALO_RECONEXAO_BLE_MS) {
        ultimaTentativaBluetoothMs = agoraMs;
        // begin() inicia a publicidade; o Android pode reconectar sem
        // aplicativo em segundo plano quando o tablet aceitar o dispositivo.
        tecladoBluetooth.begin();
        Serial.println("Bluetooth desconectado; publicidade/reconexao reativada.");
    }
}

void setup() {
    Serial.begin(115200);
    botoes.iniciar();
    tecladoBluetooth.begin();
    ultimaTentativaBluetoothMs = millis();
    Serial.println("Comando de volante iniciado.");
}

void loop() {
    const uint32_t agoraMs = millis();
    manterBluetoothDisponivel(agoraMs);

    if (tecladoBluetooth.isConnected()) {
        enviarComando(botoes.atualizar(agoraMs));
    } else {
        // Continua filtrando os botoes enquanto desconectado para manter o
        // estado do ADC atualizado, sem transmitir comandos pendentes.
        botoes.atualizar(agoraMs);
    }

    delay(5);
}
