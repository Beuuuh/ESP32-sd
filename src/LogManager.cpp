#include "stdarg.h"
#include <stdio.h>
#include "LogManager.h"

LogManager &LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

void LogManager::process_log(const char* format, va_list args) {
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    printf("%s", buffer);
}

void LogManager::init() {
    printf("LogManager initialized\n");
}

