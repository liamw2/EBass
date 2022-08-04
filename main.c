// PWM Tone App
//
// Use PWM to play a tone over the speaker

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_timer.h"
#include "freq.h"
#include "sin_l.h"
#include "trem.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

// Global variables
#define SAMPLE_TIMER_PERIOD 500
APP_TIMER_DEF(sample_timer);

// Tremolo state variable
static trem_state_t states[4] = {0};

// PWM configuration
#define PWM_FREQUENCY 32000 // 16 kHz PWM frequency
#define BUFFER_SIZE 6400    // 1/10sec data
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_common_t sequence_data_1[BUFFER_SIZE] = {0};
nrf_pwm_values_common_t sequence_data_2[BUFFER_SIZE] = {0};
bool playing_seq_1;
bool filling_buf = false;
float last_phase_1;
float last_phase_2;

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence = {
    .length = BUFFER_SIZE,
    .repeats = 0,
    .end_delay = 0,
};

nrfx_pwm_config_t pwm_cfg = {
    .output_pins = {EDGE_P3, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED,
                    NRFX_PWM_PIN_NOT_USED},
    .irq_priority = 1,
    .base_clock = NRF_PWM_CLK_16MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .load_mode = NRF_PWM_LOAD_COMMON,
    .top_value = (16000000 / PWM_FREQUENCY),
    .step_mode = NRF_PWM_STEP_AUTO,
};

static void pwm_irq_handler(void *_unused) {
  if (playing_seq_1) {
    pwm_sequence.values.p_common = sequence_data_2;
    playing_seq_1 = false;
  } else {
    pwm_sequence.values.p_common = sequence_data_1;
    playing_seq_1 = true;
  }
  nrf_pwm_sequence_set(PWM_INST.p_registers, 0, &pwm_sequence);
  nrf_pwm_sequence_set(PWM_INST.p_registers, 1, &pwm_sequence);
}

static void pwm_init(void) {
  // Initialize the PWM
  // SPEAKER_OUT is the output pin, mark the others as NRFX_PWM_PIN_NOT_USED
  // Set the clock to 500 kHz, count mode to Up, and load mode to Common
  // The Countertop value doesn't matter for now. We'll set it in play_tone()

  nrfx_pwm_init(&PWM_INST, &pwm_cfg, pwm_irq_handler);
}

static void play_tone(void) {
  // Stop the PWM (and wait until its finished)
  //   nrfx_pwm_stop(&PWM_INST, true);

  // Start playback of the samples and loop indefinitely
  //   nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1,
  //   NRFX_PWM_FLAG_LOOP);
}

static void sample_timer_callback(void *_unused) {
  // Pull
  uint8_t button_state = button_status();

  trem_state_read(&states[0]);
  trem_state_read(&states[1]);
  trem_state_read(&states[2]);
  trem_state_read(&states[3]);
  nrf_pwm_values_common_t *data_buf;
  int base_offset[4] = {0, 4, 7, 12};
  float i_freq[2] = {0};
  int i_freq_index = 0;
  for (int i = 0; i < 4; i++) {
    if (states[i].on) {
      float note = freq[base_offset[i] + button_state] +
                   (states[i].scale_factor * TREM_FREQ_PER_SF);
      i_freq[i_freq_index] = 2 * M_PI * note / PWM_FREQUENCY;
      i_freq_index++;
      if (i_freq_index > 1) {
        break;
      }
    }
  }

  // Use data buffer not currently being played
  if (playing_seq_1) {
    data_buf = sequence_data_2;
  } else {
    data_buf = sequence_data_1;
  }

  if (i_freq_index > 1) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
      data_buf[i] =
          pwm_cfg.top_value * (sin_l(0.1, i_freq[0] * i + last_phase_1) +
                               sin_l(0.5, i_freq[1] * i + last_phase_2));
    }
    last_phase_1 += i_freq[0] * BUFFER_SIZE;
    last_phase_2 += i_freq[1] * BUFFER_SIZE;
  } else {
    for (int i = 0; i < BUFFER_SIZE; i++) {
      data_buf[i] =
          pwm_cfg.top_value * (sin_l(1, i_freq[0] * i + last_phase_1));
    }
      last_phase_1 += i_freq[0] * BUFFER_SIZE;
  }
}

int main(void) {
  printf("Board started!\n");

  // initialize ADC
  adc_init();
  states[0].channel = ADC_TREM_1_CHANNEL;
  states[1].channel = ADC_TREM_2_CHANNEL;
  states[2].channel = ADC_TREM_3_CHANNEL;
  states[3].channel = ADC_TREM_4_CHANNEL;

  // initialize app timers
  app_timer_init();
  app_timer_create(&sample_timer, APP_TIMER_MODE_REPEATED,
                   sample_timer_callback);

  button_init();

  // initialize PWM
  pwm_init();

  // start timer
  // change the rate to whatever you want
  app_timer_start(sample_timer, SAMPLE_TIMER_PERIOD, NULL);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    sequence_data_1[i] = pwm_cfg.top_value;
    sequence_data_2[i] = pwm_cfg.top_value;
  }

  // Play first buffer
  pwm_sequence.values.p_common = sequence_data_1;
  playing_seq_1 = true;
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1,
                           NRFX_PWM_FLAG_SIGNAL_END_SEQ0 | NRFX_PWM_FLAG_LOOP);

  // Play the A4 tone, modulated by trem
  while (true) {
    nrf_delay_ms(10);
  }
}