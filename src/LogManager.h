#include "esp_log.h"
#include <freertos/mpu_wrappers.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

class LogManager {
    public:
        static LogManager &getInstance();
        void process_log(const char* format, va_list args);
        void init();
    private:
        //Singleton stuff
        LogManager() {
            
        }

        LogManager(const LogManager&) = delete;
        LogManager& operator=(const LogManager&) = delete;
        
        esp_err_t writeFile(const char *path, char *data);
        esp_err_t readFile(const char *path);
        void configSDCard();
};