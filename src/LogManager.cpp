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
    //ver se existe uma forma de pular os logs por TAG (spi)
    //Funcao bloqueante
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    const char *filename = MOUNT_POINT"/log.txt";

    printf("Processing log: %s", buffer);

    //write to file:
    esp_err_t ret = writeFile(filename, buffer);
    if (ret != ESP_OK) {
        printf("SOMETHING WENT WRONG");
        return;
    }
}

void LogManager::init() {
    printf("LogManager initialized, and SD card about to be configured\n");

    configSDCard();
}

esp_err_t LogManager::writeFile(const char *path, char *data)
{
    printf("Opening file %s \n", path);
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
    if(f == NULL) {
        printf("Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if(pos) {
        *pos = '\0';
    }
    printf("Read from file: '%s'", line);

    return ESP_OK;
}

void LogManager::configSDCard() {
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
        .disk_status_check_enable = false,
        .use_one_fat = false
    };

    sdmmc_card_t *card;
    printf("Mounting SD card...");
    printf("using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    
    spi_bus_config_t bus_config {};
    bus_config.mosi_io_num = MOSI_PIN;
    bus_config.miso_io_num = MISO_PIN;
    bus_config.sclk_io_num = SCK_PIN;
    bus_config.quadhd_io_num = -1;
    bus_config.quadwp_io_num = -1;
    bus_config.max_transfer_sz = 4000 * 4;

    ret = spi_bus_initialize((spi_host_device_t)host.slot, &bus_config, SPI_DMA_CH_AUTO);
    if(ret != ESP_OK) {
        printf("Failed to initialize bus.");
        return;
    }

    printf("Bus initialized successfully.");

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = (gpio_num_t)CS_PIN;
    slot_config.host_id = (spi_host_device_t)host.slot;

    printf("Mounting filesystem...");
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if(ret != ESP_OK) {
        if(ret == ESP_FAIL) {
            printf("Failed to mount filesystem.");
        } else {
            printf("Failed to initialize the card (%s).", esp_err_to_name(ret));
            return;
        }
    }

    printf("Filesystem mounted successfully. Theoretically, you can now read/write files under %s", MOUNT_POINT);
}