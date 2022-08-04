#pragma once

#include <stdint.h>

#include "nrfx_saadc.h"

#include "microbit_v2.h"

// Analog pin input
#define ANALOG_TREM_1_IN NRF_SAADC_INPUT_AIN0
#define ANALOG_TREM_2_IN NRF_SAADC_INPUT_AIN1
#define ANALOG_TREM_3_IN NRF_SAADC_INPUT_AIN2
#define ANALOG_TREM_4_IN NRF_SAADC_INPUT_AIN4

// Analog channel input
#define ADC_TREM_1_CHANNEL 0
#define ADC_TREM_2_CHANNEL 1
#define ADC_TREM_3_CHANNEL 2
#define ADC_TREM_4_CHANNEL 3

// Tremolo frequency range
#define TREM_FREQ_PER_SF 20

// Voltage Threshold for strings to be pressed
#define STRING_PRESSED_THRESH 3.05

typedef struct {
    bool on;
    float scale_factor;
    uint8_t channel;
} trem_state_t;

void adc_init(void);
void trem_state_read(trem_state_t *state);
