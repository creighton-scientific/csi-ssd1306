#pragma once

#include "assertion.hpp"
#include "csi2c.hpp"
#include "test.hpp"
#include "ssd1306/font-base.hpp"
#include "ssd1306/ssd1306.hpp"

namespace CStest {

    class TestSsd1306 final : public Test {
    public:

        TestSsd1306( const CScore::Assertion::verbosity level,
                     const std::string_view className) : Test(level, className) {
            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::TestSsd1306()");
            // Call addTestFunction for each test function. Then executeAll will run each one in order.
//            addTestFunction([this](const CScore::Assertion::verbosity v) { test1(v); });
//            addTestFunction([this](const CScore::Assertion::verbosity v) { test2(v); });
//            addTestFunction([this](const CScore::Assertion::verbosity v) { testFont5x8(v); });
//            addTestFunction([this](const CScore::Assertion::verbosity v) { testFont8x8(v); });
//            addTestFunction([this](const CScore::Assertion::verbosity v) { testFont12x16(v); });
//            addTestFunction([this](const CScore::Assertion::verbosity v) { testFont16x32(v); });
            addTestFunction([this](const CScore::Assertion::verbosity v) { test1_1306Driver(v); });

        }
        void setUp () override {Test::setUp();}
        void tearDown () override {Test::tearDown();}

        void test1 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;
        void test2 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;   // Hello World! via FONT_12X16
        void testFont5x8 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;   // Hello World! via FONT_5X8
        void testFont8x8 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;   // Hello World! via FONT_8X8
        void testFont12x16 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;   // Hello World! via FONT_12X16
        void testFont16x32 (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;   // Hello World! via FONT_16X32
        void test1_1306Driver (CScore::Assertion::verbosity level = CScore::Assertion::VERBOSE) const;

        // Support functions
        static void testFont (
            CScore::Assertion::verbosity level,
            CSdevices::FontId fontId,
            CSdevices::CsI2C::ControllerId controllerId = CSdevices::CsI2C::ControllerId::I2C_CONTROLLER_0,
            CSdevices::Ssd1306::Ssd1306DeviceAddress i2cDeviceAddress = CSdevices::Ssd1306::Ssd1306DeviceAddress::DEVICE_3C,
            CSdevices::Ssd1306::DeviceDimensions deviceDimensions = CSdevices::Ssd1306::DeviceDimensions::W128xH64);

        void testDriver1 (CScore::Assertion::verbosity level, CSdevices::FontId fontId) const;
    };
}
