#pragma once

#include <string>
#include <sstream>

#include "hashset.h"

namespace soul {

template<typename T> std::string toString(T t) 
{ 
    std::stringstream ss;
    ss << t;
    return ss.str();
}

enum class Status { OK = 1, ERROR = -1 };

struct Response {
    Status status;
    std::string msg;

    explicit Response(const Status s, const std::string m) : status(s), msg(m) {}
    ~Response() = default;
};

} // namespace soul