# Comando Volante Saveiro

Firmware PlatformIO para ESP32 dedicado à leitura da rede resistiva dos
comandos do volante e ao envio de teclas de mídia por Bluetooth HID.

## Estrutura

```text
comando_volante/
├── include/
│   ├── configuracao.h   # Pino do volante, calibração ADC e tempos
│   └── botoes.h         # Declaração do leitor e dos comandos HID
├── src/
│   ├── botoes.cpp       # Média móvel, debounce e envio das teclas
│   └── main.cpp         # Inicialização do Bluetooth e loop principal
├── platformio.ini       # Ambiente ESP32 e dependência BleKeyboard
└── README.md
```

## Arquivo de configuração

Em `include/configuracao.h`, ajuste as faixas `MIN` e `MAX` após medir os
valores da rede resistiva:

| Botão | Constantes | Tecla enviada |
|---|---|---|
| Aumentar volume | `VOL_UP_ADC_MIN/MAX` | Volume + |
| Diminuir volume | `VOL_DOWN_ADC_MIN/MAX` | Volume - |
| Próxima faixa | `NEXT_TRACK_ADC_MIN/MAX` | Próxima faixa |
| Faixa anterior | `PREV_TRACK_ADC_MIN/MAX` | Faixa anterior |
| Play/Pause | `PLAY_PAUSE_ADC_MIN/MAX` | Play/Pause |

O ADC utiliza resolução de 12 bits, com valores nominais de 0 a 4095. O
GPIO 36 é o pino VP do ESP32 e pertence ao ADC1. As faixas não devem se
sobrepor.

## Funcionamento

`BotoesVolante` mantém uma média móvel de 10 amostras e aplica debounce de
250 ms. Cada pressionamento confirmado gera somente um evento. Solturas e
leituras fora das faixas calibradas não enviam comandos.

O dispositivo HID aparece com o nome **Comando Volante Saveiro**. O tablet
Android deve ser pareado com esse dispositivo para receber os comandos de
mídia.

## Compilação e gravação

1. Instale a extensão PlatformIO no VS Code.
2. Abra a pasta `comando_volante`.
3. Execute **PlatformIO: Build**.
4. Conecte o ESP32 por USB.
5. Execute **PlatformIO: Upload**.
6. Abra o monitor serial em 115200.
7. Pareie o tablet com **Comando Volante Saveiro**.

## Segurança elétrica

Não conecte sinais automotivos de 12 V diretamente ao ESP32. Use uma rede
resistiva adequada, proteção contra surtos e limite máximo de 3,3 V no GPIO 36.
Faça a calibração com multímetro antes de conectar o chicote do veículo.

## Dependência

- `https://github.com/T-vK/ESP32-BLE-Keyboard.git`
