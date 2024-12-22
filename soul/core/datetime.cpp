
#include "datetime.h"

using namespace soul;

void DateTime::addSeconds(int seconds) {
    time_point_ += std::chrono::seconds(seconds);
}

void DateTime::subtractSeconds(int seconds) {
    time_point_ -= std::chrono::seconds(seconds);
}

void DateTime::addDays(int days) {
    time_point_ += std::chrono::hours(days * 24);
}

void DateTime::subtractDays(int days) {
    time_point_ -= std::chrono::hours(days * 24);
}

void DateTime::addMonths(int months) {
    std::tm tm = toTm();
    tm.tm_mon += months;
    normalizeTm(tm);
    updateFromTm(tm);
}

void DateTime::subtractMonths(int months) {
    addMonths(-months);
}

void DateTime::addYears(int years) {
    std::tm tm = toTm();
    tm.tm_year += years;
    normalizeTm(tm);
    updateFromTm(tm);
}

void DateTime::subtractYears(int years) {
    addYears(-years);
}

void DateTime::addBusinessDays(int days) {
    while (days > 0) {
        addDays(1);
        if (isBusinessDay()) {
            --days;
        }
    }
}

void DateTime::subtractBusinessDays(int days) {
    while (days > 0) {
        subtractDays(1);
        if (isBusinessDay()) {
            --days;
        }
    }
}

bool DateTime::isBusinessDay() const {
    std::tm tm = toTm();
    return tm.tm_wday != 0 && tm.tm_wday != 6; // 0 = Sunday, 6 = Saturday
}