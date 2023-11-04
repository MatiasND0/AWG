#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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

static const char *TAG = "i2c";

#define I2C_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS
#define RX_BUFF_LEN 255
#define TX_BUFF_LEN 255

typedef struct {
	uint8_t wave;
	uint16_t freq;
	uint8_t ch;
	uint8_t amph;
	uint8_t ampl;
	int8_t offset;
	uint16_t phase;

}wave_config;
wave_config wave;
bool flag = false;

dac_continuous_handle_t cont_handle;
dac_continuous_config_t cont_cfg = {
	.chan_mask = DAC_CHANNEL_MASK_ALL,
	.desc_num = 8,
	.buf_size = 2048,
	.freq_hz = EXAMPLE_ARRAY_LEN*1000,
	.offset = 0,
	.clk_src = DAC_DIGI_CLK_SRC_DEFAULT,
	.chan_mode = DAC_CHANNEL_MODE_SIMUL,
};

uint8_t sin_wav[EXAMPLE_ARRAY_LEN];
uint8_t tri_wav[EXAMPLE_ARRAY_LEN];
uint8_t saw_wav[EXAMPLE_ARRAY_LEN];
uint8_t squ_wav[EXAMPLE_ARRAY_LEN];

esp_err_t generate_wave(uint8_t func, uint8_t amph, uint8_t ampl, uint16_t phase)
{
    uint32_t pnt_num = EXAMPLE_ARRAY_LEN;

    switch(func)
    {
    case 0:
    	for (int i = 0; i < pnt_num; i ++)
    		sin_wav[i] = (uint8_t)((sin(i * CONST_PERIOD_2_PI / pnt_num + (phase/sizeof(uint16_t))*CONST_PERIOD_2_PI/360) + 1) * (double)(amph) / 2 + 0.5);
    	break;
    case 1:
    	for (int i = 0; i < pnt_num; i ++)
    		tri_wav[i] = (i > (pnt_num / 2)) ? (2 * amph * (pnt_num - i) / pnt_num) : (2 * amph * i / pnt_num);
    	break;
    case 2:
    	for (int i = 0; i < pnt_num; i ++)
    	{
    		saw_wav[i] = (i == pnt_num) ? 0 : (i * amph / pnt_num);
    	}
		break;
    case 3:
    	for (int i = 0; i < pnt_num; i ++)
            squ_wav[i] = (i < (pnt_num / 2)) ? amph : 0;
		break;
    default:
        for (int i = 0; i < pnt_num; i ++)
        {
            sin_wav[i] = (uint8_t)((sin(i * CONST_PERIOD_2_PI / pnt_num) + 1) * (double)(amph) / 2 + 0.5);
            tri_wav[i] = (i > (pnt_num / 2)) ? (2 * amph * (pnt_num - i) / pnt_num) : (2 * amph * i / pnt_num);
            saw_wav[i] = (i == pnt_num) ? 0 : (i * amph / pnt_num);
            squ_wav[i] = (i < (pnt_num / 2)) ? amph : 0;
        }
    	break;
    }



    return ESP_OK;

}

esp_err_t update_dac_config()
{
	generate_wave(wave.wave,wave.amph,wave.ampl,wave.phase);
	flag = true;
	cont_cfg.freq_hz = wave.freq * EXAMPLE_ARRAY_LEN;
	cont_cfg.chan_mode = wave.ch;
	cont_cfg.offset = wave.offset;
	return ESP_OK;
}

static void dac_dma_write_task(void *args)
{
    size_t buf_len = EXAMPLE_ARRAY_LEN;

    while (1) {

    	if(flag == true)
    	{
    		ESP_ERROR_CHECK(dac_continuous_disable(cont_handle));
    		ESP_ERROR_CHECK(dac_continuous_del_channels(cont_handle));
    		ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &cont_handle));
    		ESP_ERROR_CHECK(dac_continuous_enable(cont_handle));
        	flag = false;
    	}

		switch (wave.wave) {
        case DAC_SINE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(cont_handle, (uint8_t *)sin_wav, buf_len, NULL));
            break;
        case DAC_TRIANGLE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(cont_handle, (uint8_t *)tri_wav, buf_len, NULL));
            break;
        case DAC_SAWTOOTH_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(cont_handle, (uint8_t *)saw_wav, buf_len, NULL));
            break;
        case DAC_SQUARE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(cont_handle, (uint8_t *)squ_wav, buf_len, NULL));
            break;
        default:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(cont_handle, (uint8_t *)sin_wav, buf_len, NULL));
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(CONFIG_EXAMPLE_WAVE_PERIOD_SEC * 1000));

    }

}


static esp_err_t set_i2c(void)
{
	i2c_config_t i2c_cfg = {
			.mode = I2C_MODE_SLAVE,
			.sda_io_num = 21,
			.scl_io_num = 22,
			.sda_pullup_en = true,
			.scl_pullup_en = true,
			.slave.addr_10bit_en  = 0,
			.slave.slave_addr = I2C_SLAVE_ADDR,
			.clk_flags = 0
	};

	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_cfg));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_SLAVE, RX_BUFF_LEN, TX_BUFF_LEN, ESP_INTR_FLAG_LEVEL1));

	return ESP_OK;
}

static void i2c_handler_task(void *pvParameters)
{
	ESP_LOGI(TAG,"i2c started");
	uint8_t rx_data[RX_BUFF_LEN] = {0};
	uint8_t tx_data[TX_BUFF_LEN] = "Hello world!\n";
	while(1)
	{
		if( i2c_slave_read_buffer(I2C_NUM_0, &rx_data, RX_BUFF_LEN, 100/portTICK_PERIOD_MS) )
		{
			wave.wave = rx_data[0];
			wave.ch = rx_data[1];
			wave.freq = (uint16_t)(rx_data[2] << 8) | rx_data[3];
			wave.amph = rx_data[4];
			wave.ampl = rx_data[5];
			wave.offset = rx_data[6];
			wave.phase = (uint16_t)(rx_data[7] << 8) | rx_data[8];

			ESP_LOGI(TAG,"Wave: %d",wave.wave);
			ESP_LOGI(TAG,"CH: %d",wave.ch);
			ESP_LOGI(TAG,"Frequency: %d",wave.freq);
			ESP_LOGI(TAG,"Amplitude High: %d",wave.amph);
			ESP_LOGI(TAG,"Amplitude Low: %d",wave.ampl);
			ESP_LOGI(TAG,"Offset: %d",wave.offset);
			ESP_LOGI(TAG,"Phase: %d",wave.phase/sizeof(uint16_t)*360);
			i2c_reset_rx_fifo(I2C_NUM_0);

			update_dac_config();

		}
		bzero(rx_data,RX_BUFF_LEN);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

}

void app_main(void)
{
	generate_wave(DAC_SINE_WAVE,255,0,0);

    ESP_ERROR_CHECK(set_i2c());

	ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &cont_handle));
	ESP_ERROR_CHECK(dac_continuous_enable(cont_handle));

	xTaskCreate(dac_dma_write_task, "dac_dma_write_task", 4096, NULL, 5, NULL);
	xTaskCreate(i2c_handler_task, "i2c_slave_task", 4096, NULL, 5, NULL);

}
