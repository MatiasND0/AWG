#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

#include <math.h>
#include "driver/dac_continuous.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"

#define CONST_PERIOD_2_PI           6.2832                  // 2 * PI

#define EXAMPLE_ARRAY_LEN           400                     // Length of wave array
#define EXAMPLE_DAC_AMPLITUDE       255

#define EXAMPLE_WAVE_FREQ_HZ        5000                      					// Default wave frequency 2000 Hz, it can't be too low
#define EXAMPLE_CONVERT_FREQ_HZ     (EXAMPLE_ARRAY_LEN * EXAMPLE_WAVE_FREQ_HZ)  // The frequency that DAC convert every data in the wave array


typedef enum {
    DAC_SINE_WAVE,
    DAC_TRIANGLE_WAVE,
    DAC_SAWTOOTH_WAVE,
    DAC_SQUARE_WAVE,
    DAC_WAVE_MAX,
} dac_example_wave_type_t;

typedef struct {
	dac_continuous_handle_t handle;
	dac_continuous_config_t handle_cfg;
	dac_example_wave_type_t wave;
} wave_config;

esp_err_t generate_wave(uint8_t func, uint8_t amplitude, uint8_t phase);
esp_err_t dac_cont_init(void);

#ifdef __cplusplus
}
#endif
