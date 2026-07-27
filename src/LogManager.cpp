#include "stdarg.h"

LogManager &LogManager::getInstance() {
    static LogManager instance; 
    return instance;
}

void LogManager::process_log(const char* fmt, va_list args) {

}