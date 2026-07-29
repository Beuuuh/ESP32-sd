#include "stdarg.h"
#include <stdio.h>
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

#define MOUNT_POINT "/sdcard"

LogManager &LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

const char LOG_TAG[12] = "LogManager";

void LogManager::process_log(const char* format, va_list args) {
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    const char *filename = MOUNT_POINT"/log.txt";

    //write to file:
    esp_err_t ret = writeFile(filename, buffer);
    if (ret != ESP_OK) {
        printf("SOMETHING WENT WRONG");
        return;
    }
}

void LogManager::init() {
    printf("LogManager initialized\n");

    configSDCard();
}

esp_err_t LogManager::writeFile(const char *path, char *data)
{
    printf("Opening file %s", path);
    FILE *f = fopen(path, "w");
    if(f == NULL) {
        printf("Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    printf("File written");

    return ESP_OK;
}

esp_err_t LogManager::readFile(const char *path)
{
    printf("Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("Failed to open file for reading");
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
    printf("Read from file: '%s'", line);

    return ESP_OK;
}

void LogManager::configSDCard() {
    
}