#include <iostream>
#include <stdarg.h>
#include "LogManager.h"
#include "esp_log.h"
#include <freertos/mpu_wrappers.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define MOUNT_POINT "/sdcard"
#define MISO_PIN 2
#define MOSI_PIN 15
#define CS_PIN 13
#define SCK_PIN 14

extern "C" {
    LogManager& logManager = LogManager::getInstance();
    static const char* TAG = "MAIN";
    
    void app_main() {

        ESP_LOGI(TAG, "before using esp_log_set_vprintf");
        ESP_LOGI(TAG, "This is being shown in UART");

        esp_log_set_vprintf([](const char *format, va_list args) -> int {
            logManager.process_log(format, args);
            return 0;
        });

        ESP_LOGI(TAG, "This is now being shown in stdout instead of UART");

        ESP_LOGI(TAG, "This is now being shown in stdout instead of UART 2");

        ESP_LOGI(TAG, "This is now being shown in stdout instead of UART 3");

        ESP_LOGI(TAG, "This is now being shown in stdout instead of UART 4");

    }
}