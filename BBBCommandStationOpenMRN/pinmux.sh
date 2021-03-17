#!/bin/bash
config-pin P8_10 gpio_pu;#(PIN_OUTPUT_PULLUP | MUX_MODE7) /* P8.10 (GPIO 2.04) EN 1  (Main) [Host GPIO Out] */
config-pin P8_11 gpio_input;#(PIN_INPUT_PULLUP  | MUX_MODE7) /* P8.11 (GPIO 1.13) ThermFlag 1 (Main) [Host GPIO In] */
config-pin P8_12 pruout;#(PIN_OUTPUT_PULLUP | MUX_MODE6) /* P8.12 pru 0 out 14 DCC 1 (Main) [PRU0 Out] */
config-pin P9_12 gpio_pu;#(PIN_OUTPUT_PULLUP  | MUX_MODE7) P9.12 (GPIO 1.28) Main Brake (Host GPIO output) 
config-pin P8_14 gpio_pu;#(PIN_OUTPUT_PULLUP | MUX_MODE7) /* P8.14 (GPIO 0.26) Fan          [Host GPIO Out] */
config-pin P8_16 gpio_pu;#(PIN_OUTPUT_PULLUP | MUX_MODE7) /* P8.16 (GPIO 1.14) EN 2  (Prog) [Host GPIO Out] */
config-pin P8_30 pruout;#(PIN_OUTPUT_PULLUP | MUX_MODE5) /* pru 1 out 11 DCC 2 (Prog) [PRU1 Out 11] */
config-pin P8_15 gpio_pu;#(PIN_OUTPUT_PULLUP ) RAILCOM-ENABLE
config-pin P8_16 gpio_input;#(PIN_INPUT_PULLUP  ) RAILCOM-SHORT
config-pin P8_13 gpio_input;#(PIN_INPUT_PULLUP  ) RAILCOM-DIR
config-pin P9_11 uart;#(UART ) RAILCOM-DATA
~heller/show-pins | grep 'P8_1[0123456]'
~heller/show-pins | grep 'P8_30'
~heller/show-pins | grep 'P9_1[12]'
