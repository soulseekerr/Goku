#pragma once

#include "logger.h"

namespace soul {

class AScene {
public:
    AScene() {}
    virtual ~AScene() {}

public:
    LoggerManager& logManager = LoggerManager::getInstance();
};

} // namespace soul