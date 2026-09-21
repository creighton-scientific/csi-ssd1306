
#include <hardware/gpio.h>
#include "gpio.hpp"
#include "board-config.hpp"

namespace CScore {

    void Gpio::init() {
        // Initialize different pin groups
        initI2CPins();
        initInputPins();
        initOutputPins();
        
        // Set safe initial states
        // setInitialOutputStates();

    }

    void Gpio::initI2CPins() {
        const auto& config = CScore::BOARD;
        
        // Initialize I2C pins
        gpio_init(config.i2c.c0_sda);
        gpio_init(config.i2c.c0_scl);
        gpio_init(config.i2c.c1_sda);
        gpio_init(config.i2c.c1_scl);

        // Set I2C function and pull-ups
        gpio_set_function(config.i2c.c0_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c0_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c0_sda);
        gpio_pull_up(config.i2c.c0_scl);
        
        gpio_set_function(config.i2c.c1_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c1_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c1_sda);
        gpio_pull_up(config.i2c.c1_scl);
    }

    void Gpio::initInputPins() {
        const auto& config = BOARD;
        
    }

    void Gpio::initOutputPins() {
        const auto& config = BOARD;

    }

    constexpr bool Gpio::boardHasUART0() {
        return BOARD.capabilities.hasUART0;
    }

    constexpr bool Gpio::boardHasUART1() {
        return BOARD.capabilities.hasUART1;
    }

    /*
    constexpr bool Gpio::boardHasInterlocks() {
        return BOARD.capabilities.hasInterlocks;
    }

    constexpr bool Gpio::boardHasHVControl() {
        return BOARD.capabilities.hasTubeControl;
    }
    */

    constexpr bool Gpio::boardHasUART() {
        // ReSharper disable once CppDFAUnreachableCode
        // ReSharper disable once CppRedundantBooleanExpressionArgument
        return boardHasUART0() || boardHasUART1();
    }

    /*
    constexpr BoardRevision Gpio::getBoardRevision() {
        return BOARD.revision;
    }
    */



} // namespace CScore
