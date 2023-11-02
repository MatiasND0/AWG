#include <stdio.h>
#include "dac_cont_func.h"

static const char *TAG = "dac continuous(DMA)";
static const char wav_name[DAC_WAVE_MAX][15] = {"sine", "triangle", "sawtooth", "square"};

uint8_t sin_wav[EXAMPLE_ARRAY_LEN];                      // Used to store sine wave values
uint8_t tri_wav[EXAMPLE_ARRAY_LEN];                      // Used to store triangle wave values
uint8_t saw_wav[EXAMPLE_ARRAY_LEN];                      // Used to store sawtooth wave values
uint8_t squ_wav[EXAMPLE_ARRAY_LEN];                      // Used to store square wave values


esp_err_t generate_wave(uint8_t func, uint8_t amplitude, uint8_t phase)
{
    uint32_t pnt_num = EXAMPLE_ARRAY_LEN;

    switch(func)
    {
    case 0:
    	for (int i = 0; i < pnt_num; i ++)
    		sin_wav[i] = (uint8_t)((sin(i * CONST_PERIOD_2_PI / pnt_num+phase*CONST_PERIOD_2_PI/360) + 1) * (double)(amplitude) / 2 + 0.5);
    	break;
    case 1:
    	for (int i = 0; i < pnt_num; i ++)
    		tri_wav[i] = (i > (pnt_num / 2)) ? (2 * amplitude * (pnt_num - i) / pnt_num) : (2 * amplitude * i / pnt_num);
    	break;
    case 2:
    	for (int i = 0; i < pnt_num; i ++)
    	{
    		saw_wav[i] = (i == pnt_num) ? 0 : (i * amplitude / pnt_num);
    	}
		break;
    case 3:
    	for (int i = 0; i < pnt_num; i ++)
            squ_wav[i] = (i < (pnt_num / 2)) ? amplitude : 0;
		break;
    default:
        for (int i = 0; i < pnt_num; i ++)
        {
            sin_wav[i] = (uint8_t)((sin(i * CONST_PERIOD_2_PI / pnt_num) + 1) * (double)(amplitude) / 2 + 0.5);
            tri_wav[i] = (i > (pnt_num / 2)) ? (2 * amplitude * (pnt_num - i) / pnt_num) : (2 * amplitude * i / pnt_num);
            saw_wav[i] = (i == pnt_num) ? 0 : (i * amplitude / pnt_num);
            squ_wav[i] = (i < (pnt_num / 2)) ? amplitude : 0;
        }
    	break;
    }



    return ESP_OK;

}

static void dac_dma_write_task(void *args)
{

	dac_continuous_handle_t handle = (dac_continuous_handle_t)args;
	dac_continuous_config_t cont_cfg = {
		.chan_mask = DAC_CHANNEL_MASK_ALL,
		.desc_num = 8,
		.buf_size = 2048,
		.freq_hz = EXAMPLE_ARRAY_LEN * 1000,
		.offset = 0,
		.clk_src = DAC_DIGI_CLK_SRC_DEFAULT,
		.chan_mode = DAC_CHANNEL_MODE_SIMUL,
	};

	dac_example_wave_type_t wave_sel = DAC_SINE_WAVE;
    size_t buf_len = EXAMPLE_ARRAY_LEN;

    ESP_LOGI(TAG, "%s wave start", wav_name[wave_sel]);

    ESP_ERROR_CHECK(dac_continuous_write_cyclically(handle, (uint8_t *)sin_wav, buf_len, NULL));
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	ESP_ERROR_CHECK(dac_continuous_disable(handle));
	ESP_ERROR_CHECK(dac_continuous_del_channels(handle));
	ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &handle));
	ESP_ERROR_CHECK(dac_continuous_enable(handle));

    while (1) {
        switch (wave_sel) {
        case DAC_SINE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(handle, (uint8_t *)sin_wav, buf_len, NULL));
            break;
        case DAC_TRIANGLE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(handle, (uint8_t *)tri_wav, buf_len, NULL));
            break;
        case DAC_SAWTOOTH_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(handle, (uint8_t *)saw_wav, buf_len, NULL));
            break;
        case DAC_SQUARE_WAVE:
            ESP_ERROR_CHECK(dac_continuous_write_cyclically(handle, (uint8_t *)squ_wav, buf_len, NULL));
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(CONFIG_EXAMPLE_WAVE_PERIOD_SEC * 1000));
    }

}

esp_err_t dac_cont_init(void)
{
	dac_continuous_handle_t cont_handle;
	dac_continuous_config_t cont_cfg = {
		.chan_mask = DAC_CHANNEL_MASK_ALL,
		.desc_num = 8,
		.buf_size = 2048,
		.freq_hz = EXAMPLE_ARRAY_LEN*5000,
		.offset = 0,
		.clk_src = DAC_DIGI_CLK_SRC_DEFAULT,
		.chan_mode = DAC_CHANNEL_MODE_SIMUL,
	};

	ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &cont_handle));
	ESP_ERROR_CHECK(dac_continuous_enable(cont_handle));

	xTaskCreate(dac_dma_write_task, "dac_dma_write_task", 4096, cont_handle, 5, NULL);

	return ESP_OK;
}
