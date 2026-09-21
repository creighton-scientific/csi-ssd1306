#pragma once
#ifndef CSI2C_V2_HPP_
#define CSI2C_V2_HPP_

#include <type_traits>

#include "hardware/i2c.h"
#include "logger.hpp"

namespace CSdevices {

    class CsI2C final {
    public:

        // Baud rates
        enum class BaudRate : uint32_t {
            MIN_KHZ          = (100 * 1000),        // Define them in terms of HZ
            ONE_HUNDRED_KHZ  = MIN_KHZ,
            FOUR_HUNDRED_KHZ = (400 * 1000),
            ONE_THOUSAND_KHZ = (1000 * 1000),
            ONE_GHZ          = ONE_THOUSAND_KHZ,
            MAX_KHZ          = ONE_GHZ
        };

        enum class ControllerId : uint8_t {  // Controller - AKA Bus
            I2C_CONTROLLER_0 = 0,
            I2C_CONTROLLER_1
        };

        static auto getControllerIdValue (const ControllerId controllerId) {
            return static_cast<std::underlying_type_t<ControllerId>>(controllerId);
        }

        #define CTL_ID_AS_IDX static_cast<std::underlying_type_t<ControllerId>>(controllerId) // get index from id

        static bool isInitialized (ControllerId controllerId) {
            return i2cInstances_[CTL_ID_AS_IDX].initialized;
        }

        static void setInitialized (ControllerId controllerId, const bool initialized) {
            i2cInstances_[CTL_ID_AS_IDX].initialized = initialized;
        }

        static uint32_t getRequestedBaudRate (ControllerId controllerId) {
            return i2cInstances_[CTL_ID_AS_IDX].requestedBaudRate;
        }

        static void setRequestedBaudRate (ControllerId controllerId, const uint32_t requestedBaudRate) {
            i2cInstances_[CTL_ID_AS_IDX].requestedBaudRate = requestedBaudRate;
        }

        static uint32_t getActualBaudRate (const ControllerId controllerId) {
            return i2cInstances_[CTL_ID_AS_IDX].actualBaudrate;
        }

        static void setActualBaudRate (const ControllerId controllerId, const uint32_t actualBaudRate) {
            i2cInstances_[CTL_ID_AS_IDX].actualBaudrate = actualBaudRate;
        }

        static void init (const ControllerId controllerId,
                            const BaudRate baudrateInKHz = BaudRate::FOUR_HUNDRED_KHZ) {
            if (isInitialized(controllerId)) {
                return;
            }
            i2c_inst_t* instance = getI2cInstance(controllerId);
            const auto requestedBaudRate = static_cast<std::underlying_type_t<BaudRate>>(baudrateInKHz);
            const auto actualBaudRate = i2c_init(instance, requestedBaudRate);

            i2cInstances_[CTL_ID_AS_IDX] = {requestedBaudRate, actualBaudRate, true};
        }

        static int readBuffer(  const ControllerId controllerId,
                                const uint8_t deviceAddress,
                                uint8_t *pBuffer,
                                const size_t length,
                                const bool nostop) {
            init(controllerId);
            return i2c_read_blocking(getI2cInstance(controllerId), deviceAddress, pBuffer, length, nostop);
        }

        static int readBuffer(  const ControllerId controllerId,
                                const uint8_t deviceAddress,
                                uint8_t *pBuffer,
                                const size_t length) {
            return readBuffer(controllerId, deviceAddress, pBuffer, length, false);
        }

        static int writeBuffer( const ControllerId controllerId,
                                const uint8_t deviceAddress,
                                const uint8_t *pBuffer,
                                const size_t length,
                                const bool nostop) {
            init(controllerId);

#if defined (LOG_GROUP_DAC) || defined (LOG_GROUP_9808)

            std::stringstream ss;
            ss << "controllerId: " <<
                std::to_string(static_cast<std::underlying_type_t<ControllerId>>(controllerId)) <<
                "; deviceAddress.addressByte: " << CSdevices::int_to_hex_0x(deviceAddress) << ", " <<
                "data length: " << std::to_string(length) << "; data: ";

            for (int i = 0; i < length; i++) {
                ss << CSdevices::int_to_hex_0x(pBuffer[i]) << " ";
            }

            ss << std::endl;

            CSdevices::logger_.log(CSdevices::LogLevel::Info, "Csi2c", std::string(__func__), ss.str());

#endif  // LOG_GROUP_DAC || LOG_GROUP_9808
//            CScmdctl::Communication::serialOutputLine("Csi2c.writeBuffer called.");

            const auto retVal = i2c_write_blocking(getI2cInstance(controllerId), deviceAddress, pBuffer, length, nostop);

//            CScmdctl::Communication::serialOutputLine("Csi2c.writeBuffer returned.");

#if defined (LOG_GROUP_DAC) || defined (LOG_GROUP_9808)

            ss = std::stringstream();
            ss << "i2c_write_blocking result: " << (retVal == length ? "true" : "false") << ";" << std::endl;
            CSdevices::logger_.log(CSdevices::LogLevel::Info, "Csi2c", std::string(__func__), ss.str());

#endif  // LOG_GROUP_DAC || LOG_GROUP_9808

            return retVal;
        }

        static int writeBuffer( const ControllerId controllerId,
                                const uint8_t deviceAddress,
                                const uint8_t *pBuffer,
                                const size_t length) {
            return writeBuffer(controllerId, deviceAddress, pBuffer, length, false);
        }

        static bool i2cDeviceReady( const ControllerId controllerId,
                                    const uint8_t deviceAddress) {
            constexpr uint8_t buffer = 0;
            const int result = writeBuffer(controllerId, deviceAddress, &buffer, 0);
            return (result >= 0);
        }

        static i2c_inst_t* getI2cInstance(const ControllerId controllerId) {
            return ControllerId::I2C_CONTROLLER_0 == controllerId ? i2c0 : i2c1;
        }


        /**
         * @brief Returns the timeout duration per byte for I2C communication.
         *        It is a generous timeout accommodating various I2C scenarios such as transaction times,
         *        clock stretching, and EEPROM write cycles, while maintaining a safety margin to catch
         *        genuine communication failures.
         * @return Timeout duration in microseconds per byte for I2C communication.
         */
        static uint32_t getI2CTimeoutPerByte_us() {
            // @ 100 kHz, 10us/bit; 8 bits data + 1 ACK: 90us. (maybe use 100)
            // @ 400 kHz: 90us/4 = 22us.
            // @ 1000 kHz: 90us/100 = 1us.

            /* This code could be used if we want fine-grained control over time per byte - not worth it.
            constexpr uint baseTimePerByte_us = 100;

            uint timeout = 100000;

            switch (getRequestedBaudRate()) {
                case BaudRate::MIN_KHZ:
                    timeout = baseTimePerByte_us / 1;
                    break;
                default:
                case BaudRate::FOUR_HUNDRED_KHZ:
                    timeout = baseTimePerByte_us / 4;
                    break;
                case BaudRate::ONE_THOUSAND_KHZ:
                    timeout = baseTimePerByte_us / 100;
                    break;
            }
            return timeout;
        */

            // Generous 100 ms timeout handles most I2C scenarios:
            // - Typical transactions complete in < 1 ms
            // - Clock stretching for sensor conversions: 1-50 ms typical
            // - EEPROM write cycles: 5-ert10 ms
            // This provides a comfortable safety margin for edge cases
            // while still catching genuine communication failures
            return 100000;  // 100 ms
        }

        // Prevent instantiation
        CsI2C() = delete;
        ~CsI2C() = delete;
        CsI2C(const CsI2C&) = delete;
        CsI2C& operator=(const CsI2C&) = delete;

    private:

        struct i2cInit {
            uint32_t requestedBaudRate{};
            uint32_t actualBaudrate{};
            bool  initialized{false};
        };

//        inline static auto requestedBaudRate_ = BaudRate::FOUR_HUNDRED_KHZ; // this is always in kHz. Actual is always in Hz.

        inline static i2cInit i2cInstances_[2] = {{0, 0, false}, {0, 0, false}};

    };

}

#endif
