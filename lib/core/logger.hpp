#pragma once

#include <string>

#define LOGGER_ENABLED_     // comment this out to remove all logging from the system (production..)

namespace CScore {


#define SHOW_INFO_AT_STARTUP  // Show info at startup.

    //------------------------------------------------------------------

/**
* @brief - A simple logger class to handle messages.
 * Log levels are defined. Components can log at these levels.
 * When Logger is constructed, a log level is defined.
 * We need to make this a runtime choice as well.
*/

    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        None
    };

/**
 * This is a simple Logger. The usual levels are supported.
 * It was initially implemented as a Singleton, but to get the Singleton correct and thread-safe was becoming tiresome.
 * I've left some of the Singleton code around in comments.
 * This implementation is a 'nice guy' implementation. If all the classes in the app are nice guys, everthing is fine.
 * It's implemented as a single global declaration in this file after the class declaration.
 * The definition is in logger.cpp.
 * The initial value of logLevel_ is set in its declaration, but can be changed via setLogLevel(),
 * so be careful where that's done!
 * Typically I expect the actual log level to be set from the main function and simply accepted elsewhere.
 */
    class Logger {
        LogLevel logLevel_ = LogLevel::Error; // default private.

    public:

        Logger() = default;

        /**
         * @brief Singleton pattern.
         * getInstance will always return the value of instance since it is static.
         * And since static, the constructor is called only once.
         * @return
         */
        /*
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

        Logger(const Logger&) = delete;     // Delete copy constructor.
        Logger& operator=(const Logger&) = delete;  // Delete assignment operator.
        */

        void setLogLevel(const LogLevel level = LogLevel::Error) {
            logLevel_ = level;
        }

        [[nodiscard]] LogLevel getLogLevel() const {
            return logLevel_;
        }

        void logMethodEntry(LogLevel level, const std::string &message) const;

        void logMethodEntry(LogLevel level, const std::string &className, const std::string &methodName) const;

        void logMethodEntry(LogLevel level, const std::string &className, const std::string &methodName,
                    const std::string &message) const;

        void logMethodExit(LogLevel level, const std::string &message) const;

        void logMethodExit(LogLevel level, const std::string &className, const std::string &methodName) const;

        void logMethodExit(LogLevel level, const std::string &className, const std::string &methodName,
                   const std::string &message) const;

        void logMethodParameters(LogLevel level, const std::string &className, const std::string &methodName,
                                 const std::string &parameterList, const std::string &message) const;

        void log(LogLevel level, const std::string &message) const;

        void log (LogLevel level, const std::string &className, const std::string &methodName,
                                    const std::string &message) const;


        static std::string logLevelToLabel(LogLevel level);

        static LogLevel labelToLogLevel(const std::string &label);

    };

    extern Logger logger_;  // Global variable - one logger.

}   // namespace CScore

