

#include <class/cdc/cdc_device.h>
#include <pico/time.h>

#include "hardware/gpio.h"
#include "serial-comm.hpp"

namespace CScore {

    // Static member definitions
    bool SerialComm::usbEnabled_ = false;
    uint16_t SerialComm::usbConnectTime_ = 0;
    bool SerialComm::uartEnabled_[2] = {false, false};
    uint SerialComm::actualUartBaudRate_[2] = {0, 0};


    bool SerialComm::initUsb() {
        uint16_t msWaited = 0U;
        auto maxTries = maxDelayTries_;

        while (maxTries > 0U && !tud_cdc_connected()) {
            sleep_ms(msDelayEachTry_);
            maxTries--;
            msWaited += msDelayEachTry_;
        }

        if (tud_cdc_connected()) {
            setUsbEnabled();
        }

        usbConnectTime_ = msWaited; // Note the result. We might have quit before a connection was made. Doesn't matter!

        return isUsbEnabled(); // Note the result. We might have quit before a connection was made. Doesn't matter!

    }

    bool SerialComm::initUart(UartId uartId) {

        const auto& config = BOARD;

        // Get the specific UART configuration
        const auto& uart_config = (uartId == UartId::UART0) ?
                                    config.uart.uartConfig0 : config.uart.uartConfig1;

        // Check board capabilities first
        const bool board_supports_uart = (uartId == UartId::UART0) ?
                                            config.capabilities.hasUART0 : config.capabilities.hasUART1;

        if (!board_supports_uart || !uart_config.enabled) {
            uartEnabled_[static_cast<size_t>(uartId)] = false;
            return false;
        }

        // Validate configuration before proceeding
        if (!uart_config.isValid() ||
            isInvalidGPIOPin(uart_config.tx_pin) ||
            isInvalidGPIOPin(uart_config.rx_pin)) {
            uartEnabled_[static_cast<size_t>(uartId)] = false;
            return false;
            }

        // Get hardware instance at runtime
        uart_inst_t* uart_hw = getUartHardware(uart_config.uartId);

        // Initialize UART hardware
        actualUartBaudRate_[static_cast<size_t>(uartId)] =
            uart_init(uart_hw, uart_config.baud_rate);

        // Initialize GPIO pins for UART
        initUartGpio(uartId);

        // Configure UART settings
        uart_set_hw_flow(uart_hw, false, false);
        uart_set_format(uart_hw, 8, 1, UART_PARITY_NONE);
        uart_set_fifo_enabled(uart_hw, false);
        sleep_us(10);
        uart_set_fifo_enabled(uart_hw, true);

        // Flush buffers
        for (int count = 0; count < 100 && uart_is_readable(uart_hw); count++) {
            uart_getc(uart_hw);
        }

        while (uart_is_writable(uart_hw) && !uart_is_enabled(uart_hw)) {
            sleep_us(1);
        }

        uartEnabled_[static_cast<size_t>(uartId)] = true;
        return true;

    }

    void SerialComm::initUartGpio (UartId instance) {

        const auto& config = BOARD;

        // Get the specific UART configuration
        const auto& uart_config = (instance == UartId::UART0) ?
                                  config.uart.uartConfig0 : config.uart.uartConfig1;

        // Double-check that this UART should be initialized
        const bool board_supports_uart = (instance == UartId::UART0) ?
                                   config.capabilities.hasUART0 : config.capabilities.hasUART1;

        if (!board_supports_uart || !uart_config.enabled || !uart_config.isValid()) {
            return;
        }

        gpio_init(uart_config.rx_pin);
        gpio_init(uart_config.tx_pin);
        gpio_set_function(uart_config.tx_pin, GPIO_FUNC_UART);
        gpio_set_function(uart_config.rx_pin, GPIO_FUNC_UART);

    }

    CommInterface SerialComm::getActiveCommInterface() {
        // Priority 1: USB if connected
        if (isUsbEnabled()) {
            return CommInterface::USB;
        }

        // Priority 2: Command handler UART if available
        // ReSharper disable once CppDFAConstantConditions
        if (const auto* cmdUart = getCommandUartConfig(); nullptr != cmdUart && cmdUart->enabled) {
            if (UartId uartId = BOARD.uart.getCommandHandlerId();
                uartEnabled_[static_cast<size_t>(uartId)]) {
                return CommInterface::UART;
                }
        }

        // Priority 3: Any enabled UART (fallback for backward compatibility)
        if (uartEnabled_[0] || uartEnabled_[1]) {
            return CommInterface::UART;
        }

        return CommInterface::NONE;

    }

    uint SerialComm::getCommandUartBaudRate() {
        if (CommInterface::UART == getActiveCommInterface()) {
            return actualUartBaudRate_[static_cast<size_t>(BOARD.uart.getCommandHandlerId())];
        } else {
            return 0;
        }
    }

    std::string SerialComm::getActiveInterfaceInfo() {
        switch (getActiveCommInterface()) {
            case CommInterface::USB:
                return "USB";
            case CommInterface::UART: {
                UartId uartId = BOARD.uart.getCommandHandlerId();
                return "UART" + std::to_string(static_cast<int>(uartId));
            }
            case CommInterface::NONE:
                return "None";
        }
        return "Unknown";
    }

    uart_inst_t * SerialComm::getCommandUartHardware() {
        const auto* cmdUart = getCommandUartConfig();
        // ReSharper disable once CppDFAConstantConditions
        if (nullptr == cmdUart) {   // This is always true for board 0 and A. Can't predict others.
            return nullptr;         // Could become reachable based on compile options.
        }

        // Use runtime lookup instead of stored pointer
        return getUartHardware(cmdUart->uartId);
    }
}
