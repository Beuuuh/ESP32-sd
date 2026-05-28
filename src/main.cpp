#include <iostream>
#include "esp_log.h"
#include <freertos/mpu_wrappers.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

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
        
    }
}