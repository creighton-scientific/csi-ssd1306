#pragma once

#include <utility>

#include "csi2c.hpp"
#include "ssd1306/ssd1306.hpp"

namespace CSdevices {

    // Meyer's singletons:

    inline Ssd1306& getSsd1306_0_3C () {
        static Ssd1306 ssd1306_(CsI2C::ControllerId::I2C_CONTROLLER_0, Ssd1306::Ssd1306DeviceAddress::DEVICE_3C);
        return ssd1306_;
    }

    inline Ssd1306& getSsd1306_1_3C () {
        static Ssd1306 ssd1306_(CsI2C::ControllerId::I2C_CONTROLLER_1, Ssd1306::Ssd1306DeviceAddress::DEVICE_3C);
        return ssd1306_;
    }

    inline Ssd1306& getSsd1306_0_3D () {
        static Ssd1306 ssd1306_(CsI2C::ControllerId::I2C_CONTROLLER_0, Ssd1306::Ssd1306DeviceAddress::DEVICE_3D);
        return ssd1306_;
    }

    inline Ssd1306& getSsd1306_1_3D () {
        static Ssd1306 ssd1306_(CsI2C::ControllerId::I2C_CONTROLLER_1, Ssd1306::Ssd1306DeviceAddress::DEVICE_3D);
        return ssd1306_;
    }

    inline Ssd1306& getSsd1306 (const Ssd1306::Ssd1306Devices deviceId) {
        switch (deviceId) {
            case Ssd1306::Ssd1306Devices::SSD1306_0_3C: return getSsd1306_0_3C();
            case Ssd1306::Ssd1306Devices::SSD1306_0_3D: return getSsd1306_0_3D();
            case Ssd1306::Ssd1306Devices::SSD1306_1_3C: return getSsd1306_1_3C();
            case Ssd1306::Ssd1306Devices::SSD1306_1_3D: return getSsd1306_1_3D();
            default:    std::unreachable();
        }
    }

}

