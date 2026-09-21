
#pragma once
#ifndef ASSERTION_HPP_
#define ASSERTION_HPP_

#include <iostream>

namespace CScore {
	
    class Assertion {

    public:

        enum verbosity {
            SILENT,         // No messages
            ON_FAILURE,     // Only error messages
            ASSERTIONS,     // Assertions true or false
            VERBOSE         // Everything
        };

        // use macros __FILE__ and __LINE__
        // For function you need to create the string yourself
        // hardAssertTrue will exit(100) after printing!

        static void printAssertionMessage(const std::string &message) {
            std::cout << message << std::endl;
        }

        static void printAssertionMessage(const verbosity displayLevel, const std::string &message) {
            if (VERBOSE == displayLevel) {
                printAssertionMessage(message);
            }
        }

        static bool assertTrue(const bool value,
                               const std::string &className,
                               const std::string &function,
                               const std::string &file,
                               const int line) {
            const std::string msg = "\nAssertion in " + className + "::" + function +
                              "\nin file " + file +
                              "\nat line " + std::to_string(line) +
                              (value ? " succeeded!" : " failed!\n");
            printAssertionMessage(msg);
            return value;
        }
        static bool hardAssertTrue (bool value,
                                    const std::string &className,
                                    const std::string &function,
                                    const std::string &file,
                                    const int line) {
            value = assertTrue(value, className, function, file, line);
            if (! value) {
                exit(100);
            }

            return value;
        }

        static bool assertTrue(const bool value, const std::string &function, const std::string &file, const int line) {
            std::string msg = "\nAssertion in " + function +
                              "\nin file " + file +
                              "\nat line " + std::to_string(line) +
                              (value ? " succeeded!" : " failed!\n");
            printAssertionMessage(msg);
            return value;
        }
        static bool hardAssertTrue (bool value, const std::string &function, const std::string &file, const int line) {
            value = assertTrue(value, function, file, line);
            if (! value) {
                exit(100);
            }

            return value;
        }

        static bool assertTrue(const bool value, const std::string &function, const int line) {
            const std::string msg = "Assertion in " + function +
                              " at line " + std::to_string(line) +
                              (value ? " succeeded!" : " failed!\n");
            printAssertionMessage(msg);
            return value;
        }
        static bool hardAssertTrue (bool value, const std::string &function, const int line) {
            value = assertTrue(value, function, line);
            if (! value) {
                exit(100);
            }

            return value;
        }

        static bool assertTrue(const verbosity displayLevel, bool value, const std::string &function, const int line) {
            if ((ON_FAILURE == displayLevel && !value) || displayLevel >= ASSERTIONS) {
                value = assertTrue(value, function, line);
            }

            return value;
        }
        static bool hardAssertTrue (const verbosity displayLevel,
                                    bool value,
                                    const std::string &function,
                                    const int line) {
            value = assertTrue(displayLevel, value, function, line);
            if (! value) {
                exit(100);
            }

            return value;
        }

        static bool assertTrue(const verbosity displayLevel,
                               bool value,
                               const std::string &className,
                               const std::string &function,
                               const std::string &file,
                               const int line) {
            if ((ON_FAILURE == displayLevel && !value) || displayLevel >= ASSERTIONS) {
                value = assertTrue(value, className, function, file, line);
            }

            return value;
        }
        static bool hardAssertTrue (const verbosity displayLevel,
                                    bool value,
                                    const std::string &className,
                                    const std::string &function,
                                    const std::string &file,
                                    const int line) {
            value = assertTrue(displayLevel, value, className, function, file, line);
            if (! value) {
                exit(100);
            }

            return value;
        }

    };

}


#endif // ASSERTION_HPP_

