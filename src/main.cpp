#include <iostream>
#include <stdarg.h>
#include "esp_log.h"
#include <freertos/mpu_wrappers.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "LogManager.h"

#define MOUNT_POINT "/sdcard"
#define MISO_PIN 2
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

    int log_output_changer(const char* esp_log, va_list args) {
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), esp_log, args); 
        LogManager::getInstance();

        return vprintf(esp_log, args);
    }
    
    void app_main() {
        esp_err_t ret;

        ESP_LOGI(TAG, "before using esp_log_set_vprintf");
        esp_log_set_vprintf(log_output_changer);

        ESP_LOGI(TAG, "This is now being shown in stdout instead of UART");

    }
}