//
// Created by _edd.ie_ on 22/01/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
class Logger {
    static unsigned int logLevel;
public:
    /* log if input log level is equal or smaller to log level set */
    template <typename... Args>
    static void log(unsigned int level, Args ... args) {
        if (level <= logLevel) {
            std::printf(args ...);
            /* force output, i.e. for Eclipse */
            std::fflush(stdout);
        }
    }
    static void setLogLevel(unsigned int inLogLevel) {
        inLogLevel <= 9 ? logLevel = inLogLevel :
        logLevel = 9;
    }
};

#endif //LOGGER_H
