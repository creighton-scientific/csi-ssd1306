#pragma once

#include <hardware/gpio.h>

namespace CScore {

    /**
     * GPIO management class that handles board-specific pin configurations
     */
    class Gpio final {

    private:
        Gpio() = default;
        ~Gpio() = default;

    public:
        /**
         * Initialize GPIO pins based on the current board configuration
         * Note: UART pins are initialized separately in Communication::initUart()
         */
        static void init();

        /**
         * Initialize only I2C pins - can be called independently
         */
        static void initI2CPins();

        /**
         * Initialize only input pins - can be called independently
         */
        static void initInputPins();

        /**
         * Initialize only output pins - can be called independently
         */
        static void initOutputPins();

        static constexpr bool boardHasUART0();
        static constexpr bool boardHasUART1();
        /*
        static constexpr bool boardHasInterlocks();
        static constexpr bool boardHasHVControl();
        */

        // Utility functions for board information
        static constexpr bool boardHasUART();


    private:
        /**
         * Set initial safe states for all output pins
         */
//        static void setInitialOutputStates();
    };

} // namespace CScore

