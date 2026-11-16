/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"

#include "esp_check.h"

static const char *TAG = "wifi_mcu_example";

void app_main(void)
{
    ESP_LOGI(TAG, "ESP-Hosted MCU WiFi Example Starting...");

    /* Cancel any pending bootloader rollback ASAP. The factory C5 image
     * may enable CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE; esp_ota_set_boot_
     * partition then marks this new app PENDING_VERIFY, and a reset before
     * it is marked valid silently reverts to the previous firmware. The
     * stock esp-hosted CP never calls this, so an OTA can fail to stick.
     * Mark valid immediately at boot (no-op if rollback is disabled). */
    esp_err_t mv = esp_ota_mark_app_valid_cancel_rollback();
    ESP_LOGI(TAG, "mark_app_valid_cancel_rollback: %s", esp_err_to_name(mv));

    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* create default event loop */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}
