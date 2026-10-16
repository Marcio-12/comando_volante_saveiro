#include <Arduino.h>
#include <BleKeyboard.h>

#include "botoes.h"

BleKeyboard tecladoBluetooth("Comando Volante Saveiro", "Saveiro", 100);
BotoesVolante botoes;

void setup() {
    Serial.begin(115200);
    botoes.iniciar();
    tecladoBluetooth.begin();
    Serial.println("Comando Volante Saveiro iniciado.");
}

void loop() {
    botoes.verificar(tecladoBluetooth, millis());
    delay(5);
}
