#pragma once
#include <includes.h>

enum level
{
    info,
    debug,
    warning,
    error
};

class logger_t final
{
public:
    template <level level, typename... A>
    inline void log(const char* format, A&&... args)
    {
        const char* label;

        if constexpr (level == info)    label = "INFO";
        else if constexpr (level == debug)   label = "DEBUG";
        else if constexpr (level == warning) label = "WARN";
        else if constexpr (level == error)   label = "ERROR";

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &t);

        char timebuf[9];
        std::strftime(timebuf, sizeof(timebuf), "%H:%M:%S", &tm);

        char msgbuf[1024];
        snprintf(msgbuf, sizeof(msgbuf), format, std::forward<A>(args)...);

        printf("[%s] [%s] %s\n", timebuf, label, msgbuf);
        fflush(stdout);
    }
};

inline logger_t logger;