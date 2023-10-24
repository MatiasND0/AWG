#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

#include "driver/dac_cosine.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t dac_cos_init(dac_cosine_config_t *cos_cfg, dac_cosine_handle_t *ret_handle);
esp_err_t dac_cos_freq_update(uint32_t freq ,dac_cosine_config_t *cos_cfg, dac_cosine_handle_t *ret_handle);

#ifdef __cplusplus
}
#endif
