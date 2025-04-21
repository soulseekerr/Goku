
#include "time_elapsed.h"
#include "vector2.h"

#include "logger.h"
#include "datetime.h"

#include <iostream>

using namespace soul;

ILogger::ILogger(const LOG_LEVEL level) : level_(level) {}

LoggerConsole::LoggerConsole(const LOG_LEVEL level) : ILogger(level) {}

LoggerConsole::~LoggerConsole() {}

void LoggerConsole::write(const LOG_LEVEL level, const std::string& s) {

    std::string color_code;
    std::string reset_code = "\033[0m";

    // using the X Macro
    switch (level) {
#define X(name, color, str) case LOG_LEVEL::name: color_code = color; break;
        LOG_LEVELS
#undef X
        default: color_code = ""; break;
    }

    auto level_code = std::format("[{}, {}] ", getLogLevelString(level), DateTime().timeToString());

    auto str = color_code + level_code + s + reset_code;

    std::cout << str << std::endl;
}

LoggerFile::LoggerFile(const LOG_LEVEL level, const std::string& fileName) 
    : ILogger(level) {

    fileName_ = fileName;

    std::ofstream myLog(fileName_.c_str());
    file_ = std::move(myLog);

    if (!file_)
        throw LoggerException("Error: The log file was not opened.");

    // File is opened
    file_ << "EDEN Log file." << std::endl;
}

LoggerFile::~LoggerFile() {}

void LoggerFile::write(const LOG_LEVEL level, const std::string& s) {
    auto level_code = std::format("[{}, {}] ", getLogLevelString(level), DateTime().timeToString());
    auto str =  level_code + s;

    file_ << DateTime().toString() << ": " << str << std::endl;
}

void LoggerManager::addLogger(shared_ptr<ILogger> logger) {
    // loggers_.push_back(std::move(logger));
    loggers.push_back(logger);
}

void LoggerManager::showInstanceAddress() const {
    std::cout << "LoggerManager instance: " << this << std::endl;
}