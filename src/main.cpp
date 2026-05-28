#include <iostream>
#include "esp_log.h"
#include <freertos/mpu_wrappers.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define MOUNT_POINT "/sdcard"
#define MISO_PIN 4
#define MOSI_PIN 15
#define CS_PIN 13
#define SCK_PIN 14

extern "C" {
    static const char* TAG = "MAIN";

    esp_err_t writeFile(const char *path, char *data)
    {
        ESP_LOGI(TAG, "Opening file %s", path);
        FILE *f = fopen(path, "w");
        if(f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for writing");
            return ESP_FAIL;
        }
        fprintf(f, data);
        fclose(f);
        ESP_LOGI(TAG, "File written");

        return ESP_OK;
    }

    esp_err_t readFile(const char *path)
    {
        ESP_LOGI(TAG, "Reading file %s", path);
        FILE *f = fopen(path, "r");
        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for reading");
            return ESP_FAIL;
        }
        char line[64];
        fgets(line, sizeof(line), f);
        fclose(f);

        // strip newline
        char *pos = strchr(line, '\n');
        if (pos) {
            *pos = '\0';
        }
        ESP_LOGI(TAG, "Read from file: '%s'", line);

        return ESP_OK;
    }
    
    void app_main() {
        esp_err_t ret;

        esp_vfs_fat_sdmmc_mount_config_t mount_config {
            .format_if_mount_failed = true,
            .max_files = 5,
            .allocation_unit_size = 16 * 1024
        };
        sdmmc_card_t *card;
        ESP_LOGI(TAG, "Initializing sd card");
        sdmmc_host_t host = SDSPI_HOST_DEFAULT();
        host.unaligned_multi_block_rw_max_chunk_size = 8;

        spi_bus_config_t bus_config {
            bus_config.mosi_io_num = MOSI_PIN,
            bus_config.miso_io_num = MISO_PIN,
            bus_config.sclk_io_num = SCK_PIN,
            bus_config.quadwp_io_num = -1,
            bus_config.quadhd_io_num = -1,
            bus_config.max_transfer_sz = 4000,
        };

        ret = spi_bus_initialize((spi_host_device_t)host.slot, &bus_config, SDSPI_DEFAULT_DMA);
        if(ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize bus");
        }

        sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
        slot_config.gpio_cs = (gpio_num_t)CS_PIN;
        slot_config.host_id = (spi_host_device_t)host.slot;
    }
}