#pragma once
#ifndef SERIAL_COMM_HPP_

#include <string>

#include "board-config.hpp"

namespace CScore {

    /**
     * Communication interface priorities:
     * 1. USB (if connected)
     * 2. Command Handler UART (if configured and enabled)
     * 3. Fallback to any enabled UART
     */
    enum class CommInterface : uint8_t {
        NONE,
        USB,
        UART
    };


    class SerialComm {
        // Private. All methods are static
        SerialComm() = default;
        ~SerialComm() = default;

    public:
        // No copy constructor - no constructor.
        SerialComm(const SerialComm& other) = delete;
        SerialComm& operator=(const SerialComm& other) = delete;

        // USB interface management
        static bool isUsbEnabled() { return usbEnabled_; }
        static void setUsbEnabled() { usbEnabled_ = true; }
        static void setUsbDisabled() { usbEnabled_ = false; }
        static uint16_t getUsbConnectTime() { return usbConnectTime_; }
        static bool initUsb();

        // UART interface management
        static bool initUart(UartId uartId = UartId::UART0);
        static void initUartGpio(UartId instance);

        static bool isUartEnabled() { return getActiveCommInterface() == CommInterface::UART; }
        static CommInterface getActiveCommInterface();
        static uint getCommandUartBaudRate();
        // Debug/diagnostic functions
        static std::string getActiveInterfaceInfo();
        static uart_inst_t* getCommandUartHardware();
        static const SingleUARTConfig* getCommandUartConfig() {
            return BOARD.uart.getCommandHandlerUART();
        }

    private:

        constexpr static uint16_t msDelayEachTry_ = 100;
        constexpr static uint16_t maxDelayTries_ = 30;  // should initialize in about 1500 milliseconds.

        static bool usbEnabled_;
        static uint16_t usbConnectTime_;

        // Per-UART state (indexed by UARTInstance)
        static bool uartEnabled_[2];
        static uint actualUartBaudRate_[2];


    };


}

#endif
