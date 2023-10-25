#include <stdio.h>
#include "dac_cos_func.h"

esp_err_t dac_cos_init(dac_cosine_config_t *cos_cfg, dac_cosine_handle_t *ret_handle)
{
	dac_cosine_new_channel(cos_cfg, ret_handle);
    dac_cosine_start(*ret_handle);

	printf("DAC initialized with the following parameters:\n");
	printf("Frequency = %ld\n",cos_cfg->freq_hz);
	printf("Phase = %d\n",cos_cfg->phase);
	printf("Offset = %d\n",cos_cfg->offset);

	return ESP_OK;
}

esp_err_t dac_cos_freq_update(uint32_t freq ,dac_cosine_config_t *cos_cfg, dac_cosine_handle_t *ret_handle)
{
	dac_cosine_stop(*ret_handle);
	dac_cosine_del_channel(*ret_handle);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	cos_cfg->freq_hz = freq;
	cos_cfg->flags.force_set_freq = true;
	dac_cosine_new_channel(cos_cfg, ret_handle);
    dac_cosine_start(*ret_handle);

    printf("Frequency updated to %ld[hz]\n",freq);

	return ESP_OK;
}
