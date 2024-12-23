
#include "core/logger.h"
#include <iostream>

#define DEBUGMODE

#ifdef DEBUGMODE
#define DEFINED_LOG_LEVEL soul::LOG_LEVEL::LOG_DEBUG
#else
#define DEFINED_LOG_LEVEL soul::LOG_LEVEL::LOG_INFO
#endif

int main(int argc, char** argv) {
    auto& logManager = soul::LoggerManager::getInstance();
    logManager.addLogger(make_shared<soul::LoggerConsole>(DEFINED_LOG_LEVEL));

    logManager.log("Starting Game Goku");

    return 0;
}