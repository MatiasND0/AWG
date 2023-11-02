#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "driver/dac_cosine.h"
#include "esp_check.h"

#include "driver/i2c.h"

#include "dac_cos_func.h"
#include "dac_cont_func.h"

#define I2C_SLAVE_ADDR	0x11
#define TIMEOUT_MS		1000
#define DELAY_MS		1000

void app_main(void)
{
/*
	uint8_t rx_data[5];

	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 21,
		.scl_io_num = 22,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 400000,
	};
	i2c_param_config(I2C_NUM_0, &conf);
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

	dac_cosine_handle_t chan0_handle;
	dac_cosine_config_t cos0_cfg = {
		.chan_id = DAC_CHAN_0,
		.freq_hz = 500, // It will be covered by 8000 in the latter configuration
		.clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
		.offset = 0,
		.phase = DAC_COSINE_PHASE_0,
		.atten = DAC_COSINE_ATTEN_DEFAULT,
		.flags.force_set_freq = false,
	};
*/

    //dac_cos_init(&cos0_cfg,&chan0_handle);
    generate_wave(DAC_SINE_WAVE,255,180);
    generate_wave(DAC_TRIANGLE_WAVE,125,30);
    generate_wave(DAC_SAWTOOTH_WAVE,30,60);
    generate_wave(DAC_SQUARE_WAVE,148,270);

    dac_cont_init();

    while(1)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
