#include "trem.h"

static float adc_sample_blocking(uint8_t channel);

// don't care about saadc events
static void saadc_event_callback(nrfx_saadc_evt_t const *_unused) {}

void adc_init(void) {
  // Initialize the SAADC
  nrfx_saadc_config_t saadc_config = {
      .resolution = NRF_SAADC_RESOLUTION_12BIT,
      .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
      .interrupt_priority = 4,
      .low_power_mode = false,
  };
  ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_event_callback);
  APP_ERROR_CHECK(error_code);

  // Initialize tremolo sensor channel
  nrf_saadc_channel_config_t trem_channel_configs[4] = {
      NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TREM_1_IN),
      NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TREM_2_IN),
      NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TREM_3_IN),
      NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_TREM_4_IN)};
  error_code =
      nrfx_saadc_channel_init(ADC_TREM_1_CHANNEL, &trem_channel_configs[0]);
  APP_ERROR_CHECK(error_code);
  error_code =
      nrfx_saadc_channel_init(ADC_TREM_2_CHANNEL, &trem_channel_configs[1]);
  APP_ERROR_CHECK(error_code);
  error_code =
      nrfx_saadc_channel_init(ADC_TREM_3_CHANNEL, &trem_channel_configs[2]);
  APP_ERROR_CHECK(error_code);
  error_code =
      nrfx_saadc_channel_init(ADC_TREM_4_CHANNEL, &trem_channel_configs[3]);
  APP_ERROR_CHECK(error_code);
}

void trem_state_read(trem_state_t *state) {
  float trem_volts = adc_sample_blocking(state->channel);
  state->on = trem_volts < STRING_PRESSED_THRESH;
  if (state->on) {
    state->scale_factor = ((trem_volts - 1.58) / 1.58);
  } else {
    state->scale_factor = 0;
  }
}

static float adc_sample_blocking(uint8_t channel) {
  // read ADC counts (0-4095)
  // this function blocks until the sample is ready

  nrf_saadc_channel_input_set(channel, NRF_SAADC_INPUT_DISABLED,
                              NRF_SAADC_INPUT_DISABLED);

  int16_t adc_counts = 0;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &adc_counts);
  APP_ERROR_CHECK(error_code);

  // convert ADC counts to volts
  // 12-bit ADC with range from 0 to 3.6 Volts
  float volts = adc_counts * (3.6 / 4096);

  // return voltage measurement
  return volts;
}
