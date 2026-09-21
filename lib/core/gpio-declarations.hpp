#pragma once

#include <cstdint>

namespace CScore {

    // Basic GPIO pin definitions (physical mapping)
    // Physical pins are designated by P-xx for pico; RP-xx for RP2040 and RP2350 QFN-60
    constexpr uint8_t GPIO_00 =  0;    // Physical P-01, RP-02; SPIO RX, I2C0 SDA, UART0 TX.
    constexpr uint8_t GPIO_01 =  1;    // Physical P-02, RP-03; SPIO CSn, I2C0 SCL, UART0 RX.
                                       // Physical P-03 is Ground.
    constexpr uint8_t GPIO_02 =  2;    // Physical P-04, RP-04; SPIO SCK, I2C1 SDA
    constexpr uint8_t GPIO_03 =  3;    // Physical P-05, RP-05; SPIO TX, I2C1 SCL
    constexpr uint8_t GPIO_04 =  4;    // Physical P-06, RP-06; SPIO RX, I2C0 SDA, UART1 TX.
    constexpr uint8_t GPIO_05 =  5;    // Physical P-07, RP-07; SPIO CSn, I2C0 SCL, UART1 RX.
                                       // Physical P-08 is Ground.
    constexpr uint8_t GPIO_06 =  6;    // Physical P-09, RP-08; SPIO SCK, I2C1 SDA
    constexpr uint8_t GPIO_07 =  7;    // Physical P-10, RP-09; SPIO TX, I2C1 SCL
    constexpr uint8_t GPIO_08 =  8;    // Physical P-11, RP-11; SPI1 RX, I2C0 SDA, UART1 TX.
    constexpr uint8_t GPIO_09 =  9;    // Physical P-12, RP-12; SPI1 CSn, I2C0 SCL, UART1 RX.
                                       // Physical P-13 is Ground.
    constexpr uint8_t GPIO_10 = 10;    // Physical P-14, RP-13; SPI1 SCK, I2C1 SDA.
    constexpr uint8_t GPIO_11 = 11;    // Physical P-15, RP-14; SPI1 TX, I2C1 SCL.
    constexpr uint8_t GPIO_12 = 12;    // Physical P-16, RP-15; SPI1 RX, I2C0 SDA, UART0 TX.
    constexpr uint8_t GPIO_13 = 13;    // Physical P-17, RP-16; SPI1 CSn, I2C0 SCL, UART0 RX.
                                       // Physical P-18 is Ground.
    constexpr uint8_t GPIO_14 = 14;    // Physical P-19, RP-17; SPI1 SCK, I2C1 SDA.
    constexpr uint8_t GPIO_15 = 15;    // Physical P-20, RP-18; SPI1 TX, I2C1 SCL.
    constexpr uint8_t GPIO_16 = 16;    // Physical P-21, RP-27; SPI1 RX, I2C0 SDA, UART0 TX.
    constexpr uint8_t GPIO_17 = 17;    // Physical P-22, RP-28; SPI1 CSn, I2C0 SCL, UART0 RX.
                                       // Physical P-23 is Ground.
    constexpr uint8_t GPIO_18 = 18;    // Physical P-24, RP-29; SPI1 SCK, I2C1 SDA.
    constexpr uint8_t GPIO_19 = 19;    // Physical P-25, RP-30; SPI0 TX, I2C1 SCL.
    constexpr uint8_t GPIO_20 = 20;    // Physical P-26, RP-31;          I2C0 SDA.
    constexpr uint8_t GPIO_21 = 21;    // Physical P-27, RP-32;          I2C0 SCL.
                                       // Physical P-28 is Ground.
    constexpr uint8_t GPIO_22 = 22;    // Physical P-29, RP-33;
                                       // Physical P-30 is RUN (Controls RP2040 Reset)
    constexpr uint8_t GPIO_23 = 23;    // Physical P-xx, RP-35;
    constexpr uint8_t GPIO_24 = 24;    // Physical P-xx, RP-36;
    constexpr uint8_t GPIO_25 = 25;    // Physical P-xx, RP-37; LED default
    constexpr uint8_t GPIO_26 = 26;    // Physical P-31, RP-38; ADC0 default; I2C1 SDA
    constexpr uint8_t GPIO_27 = 27;    // Physical P-32, RP-39; ADC1 default; I2C1 SCL
                                       // Physical P-33 is Ground; AGND (analog ground)
    constexpr uint8_t GPIO_28 = 28;    // Physical P-34, RP-40; ADC2 default;
    constexpr uint8_t GPIO_29 = 39;    // Physical P-xx, RP41; ADC3
                                       // Physical P-35;       ADC_VREF
                                       // Physical P-36; 3V3(OUT)
                                       // Physical P-37; 3V3 EN (Enable 3.3V regulator)
                                       // Physical P-38 is Ground.
                                       // Physical P-39; VSYS (Power supply input)
                                       // Physical P-40; VBUS (USB power input)
    constexpr uint8_t GPIO_INVALID = 255;	// Used in places needing a value, but can't be valid.
}

