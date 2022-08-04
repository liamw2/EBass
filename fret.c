#include "fret.h"

#include <stdbool.h>

#include "nrf_gpio.h"
#include "microbit_v2.h"

#define NUM_BUTTONS 4
uint32_t buttons[NUM_BUTTONS] = {EDGE_P5, EDGE_P6, EDGE_P7, EDGE_P8};

void button_init(void) {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    nrf_gpio_pin_dir_set(buttons[i], 0);
  }
}

uint8_t button_status(void) {
    return ((nrf_gpio_pin_read(buttons[0]) << 0) |
            (nrf_gpio_pin_read(buttons[1]) << 1) |
            (nrf_gpio_pin_read(buttons[2]) << 2) |
            (nrf_gpio_pin_read(buttons[3]) << 3));
}
