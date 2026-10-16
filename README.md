# Comando Volante - Volkswagen Saveiro

Firmware para ESP32 que lê uma rede de botões resistivos no GPIO 36 e
apresenta o dispositivo ao tablet Android como teclado Bluetooth HID. Os
comandos de mídia são enviados sem aplicativo residente no tablet.

## Índice detalhado por arquivo e linha

As linhas abaixo correspondem à versão inicial deste projeto. Use-as como
índice rápido no VS Code (`Ctrl+G`).

### `platformio.ini`

| Linhas | Conteúdo |
|---|---|
| 1-4 | Ambiente `esp32dev`, plataforma Espressif32 e framework Arduino |
| 5 | Velocidade do monitor serial |
| 7-8 | Dependência `ESP32-BLE-Keyboard` |

### `include/configuracao.h`

| Linhas | Conteúdo |
|---|---|
| 1-2 | Include guard do arquivo |
| 4 | GPIO 36 / VP / ADC1_CH0 |
| 6-8 | Seção de calibração dos botões |
| 11-12 | Faixa ADC do Volume + |
| 15-16 | Faixa ADC do Volume - |
| 19-20 | Faixa ADC da próxima faixa |
| 23-24 | Faixa ADC da faixa anterior |
| 27-28 | Faixa ADC do Play/Pause/Mute |
| 31-34 | Debounce de 250 ms e média de 10 leituras |
| 36 | Fim do include guard |

### `include/botoes.h`

| Linhas | Conteúdo |
|---|---|
| 1-3 | Includes e dependência da configuração |
| 5-12 | Enumeração dos estados dos botões |
| 14-26 | Interface pública de `BotoesVolante` |
| 28-31 | Métodos privados do filtro e mapeamento |
| 33-44 | Estado interno da média móvel e do debounce |

### `src/botoes.cpp`

| Linhas | Conteúdo |
|---|---|
| 3-4 | Construtor do leitor |
| 6-20 | Inicialização do ADC e preenchimento inicial do filtro |
| 22-33 | Média móvel de 10 amostras |
| 35-55 | Mapeamento das faixas ADC para os botões |
| 57-86 | Debounce, detecção de pressionamento e eventos únicos |
| 88-95 | Acesso à leitura filtrada e ao estado estável |

### `src/main.cpp`

| Linhas | Conteúdo |
|---|---|
| 1-6 | Includes, instância HID e leitor dos botões |
| 8-28 | Conversão dos botões para teclas de mídia HID |
| 30-47 | Verificação de conexão e nova publicidade BLE |
| 49-56 | Inicialização serial, ADC e Bluetooth |
| 58-73 | Loop principal de leitura e envio dos comandos |

## Estrutura

```text
comando_volante/
├── include/
│   ├── configuracao.h
│   └── botoes.h
├── src/
│   ├── botoes.cpp
│   └── main.cpp
├── platformio.ini
└── README.md
```

## Calibração dos botões

Os valores em `include/configuracao.h` são apenas exemplos. Para calibrar:

1. Monte o divisor/rede resistiva do volante com o ESP32 desligado.
2. Garanta que o sinal aplicado ao GPIO 36 nunca ultrapasse 3,3 V.
3. Abra o Monitor Serial ou use uma leitura temporária de `analogRead()`.
4. Registre o ADC sem botão e com cada botão pressionado.
5. Defina uma faixa `MIN` a `MAX` com margem para ruído para cada botão.
6. Confirme que as faixas não se sobrepõem.
7. Faça o upload novamente e teste cada tecla no tablet.

O ADC configurado tem resolução de 12 bits, portanto o intervalo nominal é de
0 a 4095. O GPIO 36 é somente entrada e pertence ao ADC1, que é apropriado
para uso simultâneo com Bluetooth.

## Instalação e pareamento

1. Instale a extensão PlatformIO no VS Code.
2. Abra a pasta `comando_volante`.
3. Execute **PlatformIO: Build**.
4. Grave o firmware com **PlatformIO: Upload**.
5. Abra o Bluetooth do Android e pareie com `Saveiro Volante`.
6. Reproduza mídia e teste os cinco comandos.

O Android recebe os eventos como teclas de mídia HID padrão. Não é necessário
instalar ou manter um aplicativo em segundo plano.

## Reconexão Bluetooth

Enquanto o tablet estiver desconectado, o loop continua executando e verifica
o estado do Bluetooth. A cada 5 segundos, a publicidade é reativada por
`BleKeyboard::begin()`, permitindo novo pareamento ou reconexão do host.
Depois que o tablet estiver conectado, os eventos só são enviados após o
debounce e somente uma vez por novo pressionamento.

## Segurança elétrica

Não conecte diretamente sinais automotivos de 12 V ao ESP32. Use uma rede
resistiva apropriada, proteção contra surtos e limite de tensão compatível com
o GPIO. Valide o circuito com multímetro antes de conectar a alimentação e o
chicote do veículo.

## Dependências

- ESP32 com suporte Arduino pela plataforma `espressif32`.
- Biblioteca `ESP32-BLE-Keyboard`, declarada em `platformio.ini`.
- Tablet Android com suporte a teclas de mídia Bluetooth HID.
