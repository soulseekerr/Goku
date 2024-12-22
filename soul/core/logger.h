#pragma once

#include "log_levels.h"
#include "singleton.h"

#include <vector>
#include <memory>
#include <string_view>
#include <fstream>
#include <exception>
#include <format>

using namespace std;

namespace soul {

// X Macro for log levels with color
enum class LOG_LEVEL {
#define X(name, color, str) name,
    LOG_LEVELS
#undef X
};

inline const char* getLogLevelString(LOG_LEVEL level) {
    switch (level) {
#define X(name, color, str) case LOG_LEVEL::name: return str;
        LOG_LEVELS
#undef X
        default: return "UNKNOWN";
    }
}

class LoggerException : public std::exception {
public:
    explicit LoggerException(const string& message) 
        : message_(message) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    string message_;
};

// Abstract Logger class
class ILogger {

public:
    ILogger(const LOG_LEVEL level);
    virtual ~ILogger() = default;

    void setLevel(const LOG_LEVEL level) { level_ = level; }

    LOG_LEVEL getLevel() const { return level_; }

    virtual void write(const LOG_LEVEL level, const string& s) = 0;

private:
    LOG_LEVEL     level_; 
};

// Simple Logger for console output 
class LoggerConsole : public ILogger {

public:
    LoggerConsole(const LOG_LEVEL level);

    virtual ~LoggerConsole();

    void write(const LOG_LEVEL level, const string& s) override;
};

// Logger with simple file output 
class LoggerFile : public ILogger {

public:
    LoggerFile(const LOG_LEVEL level, const string& fileName);
    virtual ~LoggerFile();

    void write(const LOG_LEVEL level, const string& s) override;

private:
    ofstream   file_;
    string     fileName_;
};

/**
 * Manager of Loggers.
 */
class LoggerManager : public SingletonT<LoggerManager> {

    // make the current class friend of the Singleton class
    MAKE_SINGLETON(LoggerManager)

public:
    void showInstanceAddress() const;

    void addLogger(shared_ptr<ILogger> logger);

    template<typename... Args>
    string dynaWriteGet(string_view rt_fmt_str, Args&&... args) {
        
        return vformat(rt_fmt_str, std::make_format_args(args...));
    }

    template<typename... Args>
    void log(string_view rt_fmt_str, Args&&... args) {

        auto str = vformat(rt_fmt_str, std::make_format_args(args...));
        
        for (auto& logger : loggers)
            logger->write(LOG_LEVEL::LOG_INFO, str);
    }

    template<typename... Args>
    void logLevel(LOG_LEVEL level, string_view rt_fmt_str, Args&&... args) {

        auto str = vformat(rt_fmt_str, std::make_format_args(args...));

        for (auto& logger : loggers) {
            if (level >= logger->getLevel())
                logger->write(level, str);
        }
    }

    template<typename... Args>
    void logInfo(string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_INFO, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logDebug(string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_DEBUG, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logDebugVerbose(string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_DEBUGVERBOSE, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logWarn(string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_WARNING, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logError(string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_ERROR, rt_fmt_str, std::forward<Args>(args)...);
    }

private:
    vector<shared_ptr<ILogger>>  loggers;
};


} // namespace soul