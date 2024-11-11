#ifndef QUANTUMDEF_H
#define QUANTUMDEF_H

// LED Strip Definitions
#define N_LED     15   // Number of LEDs
#define STRIP_PIN 15   // LED strip pin

// Extra Definitions
#define SW_BOOT   0   // BOOT SW pin, is one by default
#define BTN       16  // BUTTON pin, is one by default
#define POTE      2
#define BAT_ADC   1
#define UART_TXD  43  // USB UART with CH343P
#define UART_RXD  44  // USB UART with CH343P

// MIC Definitions                   
#define LR            18
#define MIC_CLK       17
#define MIC_DOUT_AOUT 13
#define I2S_SCK       22 // Not used

// TFT Definitions
#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 240

#define FIRMWARE_VERSION 2.00

extern int mic_avg;

#endif