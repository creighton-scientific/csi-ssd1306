
#include <iostream>
#include <class/cdc/cdc_device.h>
#include <device/usbd.h>

#include "communication.hpp"
#include "serial-comm.hpp"


namespace CScmdctl {

    // Static member definitions
//    DisplayMode Communication::displayMode_ = DisplayMode::GAMMA_DISPLAY;
    bool Communication::bpState_ = false;

    /**
     * Extract potential commands from the buffer.
     */
    void Communication::handleInputBuffer() {
        static std::string inputBuffer;
        std::string commandString;

        switch (CScore::SerialComm::getActiveCommInterface()) {

            case CScore::CommInterface::USB:
                tud_task();
                while (tud_cdc_available()) {
                    const char  ch = tud_cdc_read_char();   // This test program just ignores commandline input.
                }
                break;

            case CScore::CommInterface::UART: {
                uart_inst_t* uart_hw = CScore::SerialComm::getCommandUartHardware();  // Could be nullptr
                // ReSharper disable once CppDFAConstantConditions
                while (nullptr != uart_hw && uart_is_readable(uart_hw)) { // compiler options could make this nullptr
                    const char ch = uart_getc(uart_hw); // This test program just ignores commandline input.
                }
            }
                break;

            case CScore::CommInterface::NONE: // no active interface - skip it.
                break;
        }
    }

    void Communication::serialOutput(std::string &msg) {
        // Send to USB if connected - ie if UART is not enabled.

        switch (CScore::SerialComm::getActiveCommInterface()) {
            case CScore::CommInterface::USB:
                std::cout << msg << std::flush;
                break;

            case CScore::CommInterface::UART:
                // ReSharper disable once CppDFAConstantConditions
                if (uart_inst_t* uart_hw = CScore::SerialComm::getCommandUartHardware(); nullptr != uart_hw) {
                    uart_puts(uart_hw, msg.c_str());
                }
                break;

            case CScore::CommInterface::NONE:
                // No output interface available - could log error or buffer
                break;
        }
    }

    void Communication::serialOutputLine(const std::string& msg) {
        std::string s = STX + msg + ETX;
        serialOutput(s);
    }


    void Communication::serialOutputLF160(std::string &msg) {
        const size_t newlineCount = std::ranges::count(msg, '\n');

        const auto reserve = msg.size() + newlineCount + 4;
        std::string framed;
        framed.reserve(reserve);   // +4 = STX + ETX + CR + LF

        framed += STX;                     // STX
        for (const char ch : msg) {
            if (LF == ch) {
                framed += CR;
            }
            framed += ch;
        }
        framed += ETX;                     // ETX
        framed += sCRLF;                   // MESSAGE_TERMINATOR

        serialOutput(framed);

    }
}
