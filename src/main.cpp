

#include <iostream>
#include <sstream>
#include <string>

// Local project includes

#include "assertion.hpp"
#include "communication.hpp"
#include "logger.hpp"
#include "test.hpp"

using namespace CScmdctl;
using namespace CScore;

[[noreturn]] int main()
{


    // Ignore the return value. We are neither displaying it nor executing logic based on it.
    // This call will also initialize the UART0.
    if (!boardInit()) {
        exit(1000); // bad day!
    }
    auto connectTime = SerialComm::getUsbConnectTime(); // If disabled, should be 0.
    auto uartActualBaudRate = SerialComm::getCommandUartBaudRate();


#if defined SHOW_INFO_AT_STARTUP

    std::stringstream ss;
    ss << "\r\n\n****************************************************************\n";
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();

    if (SerialComm::isUsbEnabled()) {
        ss << "USB enabled.";
    }
    if (SerialComm::isUartEnabled()) {
        ss << "UART enabled @ "+ std::to_string(uartActualBaudRate) + " baud. ";
    }
    Communication::serialOutputLine(ss.str());

    ss = std::stringstream();
    ss << "\r\nTest Program For Creighton Scientific's SSD 1306 Driver\n";
    auto s = ss.str();
    Communication::serialOutputLine(ss.str());


    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    s = ss.str();
    Communication::serialOutputLine(ss.str());
#endif

    logger_.setLogLevel(LogLevel::Info);   // For prod set this to Fatal.
    CStest::runTests(CScore::Assertion::verbosity::VERBOSE);

    while (true) {
        constexpr uint16_t artificialLoopDelayMs = 0;

        Communication::handleInputBuffer();

    }

}

