#pragma once

/**
Introduced in C++11 and significantly improved in C++20, 
it provides a robust and flexible way to handle date and time.
*/
#include <chrono>
#include <iostream>
#include <format>  // for C++20

#include<ctime>
#include<regex>
#include<array>
#include<utility>
#include<cstdlib>
#include <iomanip>

using namespace std::literals; // enables literal suffixes, e.g. 24h, 1ms, 1s.

namespace soul {

class DateTime {
public:
    // Default constructor that initializes to the current time
    DateTime() : time_point_(std::chrono::system_clock::now()) {}

    // Constructor accepting year, month, and day
    DateTime(int year, int month, int day) {
        std::tm tm = {};
        tm.tm_year = year - 1900;  // std::tm uses years since 1900
        tm.tm_mon = month - 1;     // std::tm months are 0-11
        tm.tm_mday = day;
        
        // Convert std::tm to time_t, then to std::chrono::system_clock::time_point
        std::time_t time = std::mktime(&tm);
        if (time == -1) {
            throw std::invalid_argument("Invalid date");
        }
        time_point_ = std::chrono::system_clock::from_time_t(time);
    }

    // Copy constructor
    DateTime(const DateTime& dt) {
        time_point_ = dt.time_point_;
    }

    // Move constructor  - must be marked with `noexcept` to protect the move operation from interruption.
    DateTime(DateTime&& dt) noexcept {
        time_point_ = dt.time_point_;
    }

    virtual ~DateTime() {}

    // Assignment operator
    DateTime& operator=(const DateTime& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        time_point_ = dt.time_point_;
        return *this;
    }

    // Move assignment operator
    DateTime& operator=(DateTime&& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        time_point_ = dt.time_point_;
        return *this;
    }

    // Equality operator
    bool operator==(const DateTime& dt) const {
        return (time_point_ == dt.time_point_);
    }

    std::string toString() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);

        // seems not working - C++20 not setup coorectly?
        // return std::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&in_time_t));

        // Version for C++17
        std::tm tm = *std::localtime(&in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

     std::string timeToString() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);

        // seems not working - C++20 not setup coorectly?
        // return std::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&in_time_t));
        std::tm tm = *std::localtime(&in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    std::string year_month_day_h() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);

        // this does not seem to return the correct date
        // const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(time_point_)};

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }

    std::string year_month_day() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d");
        return oss.str();
    }

    std::string year_month_h() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m");
        return oss.str();
    }

    std::string year_month() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m");
        return oss.str();
    }

    std::string year() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y");
        return oss.str();
    }

    void currentTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::cout << "Current time: " << std::ctime(&now_time) << std::endl;
    }

    void addSeconds(int seconds);

    void subtractSeconds(int seconds);

    void addDays(int days);

    void subtractDays(int days);

    void addMonths(int months);

    void subtractMonths(int months);

    void addYears(int years);

    void subtractYears(int years);

    void addBusinessDays(int days);

    void subtractBusinessDays(int days);

    bool isBusinessDay() const;

private:
    // toTm Method: Converts the time_point_ to a std::tm structure.
    std::tm toTm() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        return tm;
    }

    // normalizeTm Method: Uses std::mktime to normalize the std::tm structure, 
    // which handles overflow and underflow of months and years.
    void normalizeTm(std::tm& tm) const {
        // Normalize the time structure (e.g., handle overflow of months into years)
        std::mktime(&tm);
    }

    // updateFromTm Method: Converts a normalized std::tm structure back to time_point_.
    void updateFromTm(const std::tm& tm) {
        // Convert the normalized tm back to time_point_
        std::time_t time = std::mktime(const_cast<std::tm*>(&tm));
        if (time == -1) {
            throw std::invalid_argument("Invalid date after normalization");
        }
        time_point_ = std::chrono::system_clock::from_time_t(time);
    }

private:
    std::chrono::system_clock::time_point time_point_;
};

} // namespace soul

