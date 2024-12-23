#pragma once

#include <chrono>
#include <iostream>
#include <format>

using namespace std::literals; // enables literal suffixes, e.g. 24h, 1ms, 1s.

namespace soul {

struct st_time_elapsed {
    std::chrono::time_point<std::chrono::steady_clock>      tpstart;

    void start() {
        tpstart = std::chrono::steady_clock::now();
    }

    void print_elapsed_ms() {
        const auto end = std::chrono::steady_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::microseconds>(end - tpstart);

        std::cout << "Calculations took " 
            << value << " ≈ "
            << (end - tpstart) / 1ms << "ms ≈ " // almost equivalent form of the above, but
            << (end - tpstart) / 1s << "s.\n";  // using milliseconds and seconds accordingly
    }

    void print_elapsed_ns() {
        const auto end = std::chrono::steady_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(end - tpstart);

        std::cout << "time elapsed: " << value 
            << " = " << (end - tpstart) / 1ms << "ms = "
            << (end - tpstart) / 1s << "s." <<  std::endl;
    }

    void print_elapsed_s() {
        const auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed_seconds(end - tpstart);

        std::cout << "Elapsed time: ";
        // std::cout << elapsed_seconds.count() << '\n';  // before C++20
        std::cout << elapsed_seconds << '\n';  // C++20
    }
};

struct st_datetime {

    std::string year_month_day_h() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
 
        // std::cout << "Current Year: " << static_cast<int>(ymd.year()) << ", "
        //             "Month: " << static_cast<unsigned>(ymd.month()) << ", "
        //             "Day: " << static_cast<unsigned>(ymd.day()) << "\n"
        //             "ymd: " << ymd << '\n';

        std::stringstream ss;
        ss << ymd;
        return ss.str();
    }

    std::string year() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
        
        std::stringstream ss;
        ss << ymd.year();
        return ss.str();
    }

    std::string year_month() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
        
        std::stringstream ss;
        // ss << ymd.year() << std::format("{%m}", static_cast<unsigned>(ymd.month()));
        ss << ymd.year() << static_cast<unsigned>(ymd.month());
        return ss.str();
    }

    std::string year_month_day() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
 
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};

        // return std::format(
        //     "{}{}{}", 
        //     static_cast<int>(ymd.year()),
        //     static_cast<int>(ymd.month()),
        //     static_cast<int>(ymd.day()));
        
        std::stringstream ss;
        ss << ymd.year() << static_cast<unsigned>(ymd.month()) << ymd.day();
        return ss.str();
    }
};

} // namespace soul

