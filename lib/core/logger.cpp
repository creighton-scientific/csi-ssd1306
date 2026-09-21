
#include <map>
#include <iostream>

#include "logger.hpp"
#include "../cmdctl/pystring.h"

namespace CScore {

    Logger logger_; // Global definition.


    void Logger::logMethodEntry(const LogLevel level, const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log method entry. " << message << ";\n";
        }
    }

    void Logger::logMethodEntry(const LogLevel level, const std::string &className, const std::string &methodName) const {
        if (level >= logLevel_) {
            std::cout << "Log method entry: " << className << "::" << methodName << ". \n";
        }
    }

    void Logger::logMethodExit(const LogLevel level, const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log method exit. " << message << ";\n";
        }
    }

    void Logger::logMethodEntry(const LogLevel level, const std::string &className, const std::string &methodName,
                                const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log method entry: " << className << "::" << methodName << ". \nMessage: " << message << "\n";
        }
    }

    void Logger::logMethodExit(const LogLevel level, const std::string &className, const std::string &methodName,
                               const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log method exit: " << className << "::" << methodName << ".\nMessage:\t" << message << "\n";
        }
    }

    void Logger::logMethodExit(const LogLevel level, const std::string &className, const std::string &methodName) const {
        if (level >= logLevel_) {
            std::cout << "Log method exit: " << className << "::" << methodName << ".\n";
        }
    }

    void Logger::logMethodParameters(const LogLevel level, const std::string &className, const std::string &methodName,
                                     const std::string &parameterList, const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log method: " << className << "::" << methodName << "\n\t(" << parameterList << ");\n" <<
                                "Message:\t" << message << "\n" ;
        }
    }
    void Logger::log(const LogLevel level, const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "Log level: " << logLevelToLabel(level) << ", Message: " << message << "\n";
        }
    }

    void Logger::log(const LogLevel level, const std::string &className, const std::string &methodName,
                     const std::string &message) const {
        if (level >= logLevel_) {
            std::cout << "\nLog level: " << logLevelToLabel(level) << " In " << className << "::" <<
                                methodName << "\n\tMessage: " << message << "\n";
//            log(level, message);
        }
    }

    // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage
    std::string Logger::logLevelToLabel(const LogLevel level) {
        std::map<LogLevel, std::string> levelToLabel{
                {LogLevel::Trace,   "Trace"},
                {LogLevel::Debug,   "Debug"},
                {LogLevel::Info,    "Info"},
                {LogLevel::Warn,    "Warn"},
                {LogLevel::Error,   "Error"},
                {LogLevel::Fatal,   "Fatal"},
                {LogLevel::None,    "None"}
        };

        std::string retCode = "None"; // Theoretically not possible without working at it!
        if (const auto it = levelToLabel.find(level); it != levelToLabel.end()) {
            // This means it's a legitimate index - kinda hard not to be - but possible with effort, I think.
            retCode = it->second;
        }
        return retCode;
    }

    LogLevel Logger::labelToLogLevel(const std::string &label) {
        std::map<std::string, LogLevel> labelToLevel{
                {"trace",   LogLevel::Trace},         // Note the lower case - see below. We force case-insensitive.
                {"debug",   LogLevel::Debug},
                {"info",    LogLevel::Info},
                {"warn",    LogLevel::Warn},
                {"error",   LogLevel::Error},
                {"fatal",   LogLevel::Fatal},
                {"none",    LogLevel::None}
        };

        auto retCode = LogLevel::Error; // This is the default result.
        const std::string lower = pystring::lower(label);
        if (const auto it = labelToLevel.find(lower); it != labelToLevel.end()) {
            // Found a match! Use that label.
            retCode = it->second;
        }

        return retCode;
    }

} // namespace CScore
