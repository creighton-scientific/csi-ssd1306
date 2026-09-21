
#include <sstream>

#include "assertion.hpp"
#include "ssd1306/font-base.hpp"
#include "ssd1306/fontContainer.hpp"
#include "ssd1306/ssd1306.hpp"
#include "ssd1306-text.hpp"
#include "test-ssd1306.hpp"

#include <ranges>

#include "utilities.hpp"


namespace CStest {

    using CScore::Assertion;
    using CSdevices::CsI2C;
    using CSdevices::Ssd1306;

    void TestSsd1306::test1(CScore::Assertion::verbosity level) const { // Test settng pixels by any of the three modes.
        auto retVal = true;
        // ReSharper disable once CppTooWideScopeInitStatement
        Ssd1306 ssd1306(CsI2C::ControllerId::I2C_CONTROLLER_0, Ssd1306::Ssd1306DeviceAddress::DEVICE_3C);

        if (!ssd1306.isReady()) {
            displayMessage(getVerbosityLevel(), "In " + std::string(__FUNCTION__) + "; Ssd1306 failed to initialize.");
            return;
        }
        displayMessage(getVerbosityLevel(), "Test1: Testing line drawing.");

        ssd1306.clearScreen();

        for (int16_t x = 0, y = 0, z = 127; x < 128 && y < 64; ++x, ++y, --z)
        {
            ssd1306.setPixel(x,y);
            ssd1306.refreshDisplay();
            ssd1306.setPixel(z,y);
        }

        ssd1306.refreshDisplay();

        sleep_ms(2000);


        for (int16_t x = 0, y = 63, z = 127; x < 128 && y >= 0; ++x, --y, --z)
        {
            ssd1306.setPixel(x,y);
            ssd1306.refreshDisplay();
            ssd1306.setPixel(z,y);
        }
        ssd1306.refreshDisplay();

        sleep_ms(2000);

        // make dashed line
        for (int16_t x = 0, y = 0, z = 127; x < 128 && y < 64; x+= 2, y += 2, z -= 2)
        {
            ssd1306.invertPixel(x,y);
            ssd1306.refreshDisplay();
            ssd1306.invertPixel(z,y);
        }

        ssd1306.refreshDisplay();

        sleep_ms(2000);

        // make larger dashes
        for (int16_t x = 0, y = 63, z = 127; x < 128 && y >= 0; x+=3, y+=3, z-=3)
        {
            ssd1306.setPixelOff(x,y);
            ssd1306.refreshDisplay();
            ssd1306.setPixelOff(z,y);
        }
        ssd1306.refreshDisplay();

        sleep_ms(2000);

        ssd1306.clearScreen();
        ssd1306.refreshDisplay();

    }


    void TestSsd1306::test2(Assertion::verbosity level) const { // Test rendering text

        // ReSharper disable once CppTooWideScopeInitStatement
        Ssd1306 ssd1306(CsI2C::ControllerId::I2C_CONTROLLER_0,
                        Ssd1306::Ssd1306DeviceAddress::DEVICE_3C);

        if (!ssd1306.isReady()) {
            displayMessage(getVerbosityLevel(), "In " + std::string(__FUNCTION__) + "; Ssd1306 failed to initialize.");
            return;
        }

        displayMessage(getVerbosityLevel(), "Test2: Testing default font print.");

        ssd1306.clearScreen();

//        ssd1306.resetFramebuffer();

        constexpr auto hello = "Hello";
        constexpr auto world = "World!";

        const auto pixelWidth  = Ssd1306::getDevicePixelWidth(Ssd1306::DeviceDimensions::W128xH64);
        const auto pixelHeight = Ssd1306::getDevicePixelHeight(Ssd1306::DeviceDimensions::W128xH64);
        const auto fontWidth   = getFont(ssd1306.getActiveFontId()).getWidth();
        const auto fontHeight  = getFont(ssd1306.getActiveFontId()).getHeight();
        const auto charPerLine = pixelWidth / fontWidth;
        const auto charPerPage = pixelHeight / fontHeight;

        auto anchorX = static_cast<uint8_t>((charPerLine / 2 - strlen(hello) / 2) * fontWidth);
        auto anchorY = static_cast<uint8_t>(1 * fontHeight);

        ssd1306.drawText(hello, anchorX, anchorY);

        anchorX = static_cast<uint8_t>((charPerLine / 2 - strlen(world) / 2) * fontWidth + fontWidth / 2);
        anchorY = static_cast<uint8_t>(2 * fontHeight);

        ssd1306.drawText(world, anchorX, anchorY);

        ssd1306.refreshDisplay();
        sleep_ms(2000);
        ssd1306.clearScreen();
        ssd1306.refreshDisplay();

    }

    void TestSsd1306::testFont5x8(const Assertion::verbosity level) const {   // Test font 5x8
        displayMessage(getVerbosityLevel(), "Testing font 5x8 in five seconds...");
        sleep_ms(5000);
        testFont(level, CSdevices::FontId::FONT_5X8);
    }

    void TestSsd1306::testFont8x8(const Assertion::verbosity level) const {
        displayMessage(getVerbosityLevel(), "Testing font 8x8 in five seconds...");
        sleep_ms(5000);
        testFont(level, CSdevices::FontId::FONT_8X8);
    }

    void TestSsd1306::testFont12x16(const Assertion::verbosity level) const {
        displayMessage(getVerbosityLevel(), "Testing font 12x16 in five seconds...");
        sleep_ms(5000);
        testFont(level, CSdevices::FontId::FONT_12X16);
    }

    void TestSsd1306::testFont16x32(const Assertion::verbosity level) const {
        displayMessage(getVerbosityLevel(), "Testing font 16x32 in five seconds...");
        sleep_ms(5000);
        testFont(level, CSdevices::FontId::FONT_16X32);
    }

    void TestSsd1306::test1_1306Driver(CScore::Assertion::verbosity level) const {
        displayMessage(getVerbosityLevel(), "Test1 of 1306 driver.");

        for (auto ix : std::views::iota(static_cast<int>(CSdevices::FontId::FONT_5X8),
                                           static_cast<int>(CSdevices::FontId::FONT_16X32) + 1)) {
            const auto fontId = static_cast<CSdevices::FontId>(ix);
            testDriver1(level, fontId);
        }
    }

    void TestSsd1306::testDriver1(Assertion::verbosity level, CSdevices::FontId fontId) const {
        const CSdrivers::Ssd1306Text ssd1306Text(Ssd1306::Ssd1306Devices::SSD1306_0_3C, fontId);
        const auto lineCount = ssd1306Text.getLineCount();
        const auto characterCount = ssd1306Text.getCharacterCount();
        auto currentFontId = ssd1306Text.getActiveFontId();

        std::stringstream ss;
        ss << "Using " << CSdevices::fontToStringView(ssd1306Text.getActiveFontId());
        ss << ". Lines per screen: " << +lineCount;
        ss << ". Characters per line: " << +characterCount;
        ss << "." << std::endl;
        displayMessage(getVerbosityLevel(), ss.str());

        for (auto i = 0; i < lineCount; ++i) {
            for (auto j = 0; j < characterCount; ++j) {
                ssd1306Text.writeCharacterAt(i, j, CScore::getHexDigit(j));
            }
        }

        sleep_ms(3000);
        ssd1306Text.clearScreen();

        for (auto i = 0; i < lineCount; ++i) {
            ss = std::stringstream();
            ss << "Line " << i << ".";
            ssd1306Text.writeTextLeftJustifiedAt(i, ss.str());
        }

        sleep_ms(3000);
        ssd1306Text.clearScreen();

        for (auto i = 0; i < lineCount; ++i) {
            ss = std::stringstream();
            ss << "Line " << i << ".";
            ssd1306Text.writeTextCenteredAt(i, ss.str());
        }

        sleep_ms(3000);
        ssd1306Text.clearScreen();

        for (auto i = 0; i < lineCount; ++i) {
            ss = std::stringstream();
            ss << "Line " << i << ".";
            ssd1306Text.writeTextRightJustifiedAt(i, ss.str());
        }

        sleep_ms(3000);
        ssd1306Text.clearScreen();

    }

    void TestSsd1306::testFont( const Assertion::verbosity level,
                                const CSdevices::FontId fontId,
                                const CsI2C::ControllerId controllerId,
                                const Ssd1306::Ssd1306DeviceAddress i2cDeviceAddress,
                                const Ssd1306::DeviceDimensions deviceDimensions) {


        // ReSharper disable once CppTooWideScopeInitStatement
        Ssd1306 ssd1306(controllerId, i2cDeviceAddress);

        if (!ssd1306.isReady()) {
            displayMessage(level, "In " + std::string(__FUNCTION__) + "; Ssd1306 failed to initialize.");
            return;
        }

        ssd1306.clearScreen();  // This will also clear the framebuffer
        /*
        ssd1306.setAllOn();

        ssd1306.setWriteMode(Ssd1306::WriteMode::INVERTED);
        */

        constexpr auto hello = "Hello";
        constexpr auto world = "World!";

        const auto pixelWidth  = Ssd1306::getDevicePixelWidth(deviceDimensions);
        const auto fontWidth   = CSdevices::getFont(fontId).getWidth();
        const auto fontHeight  = CSdevices::getFont(fontId).getHeight();
        const auto charPerLine = pixelWidth / fontWidth;

        auto anchorX = static_cast<uint8_t>((charPerLine / 2 - strlen(hello) / 2) * fontWidth);
        auto anchorY = static_cast<uint8_t>(0);


        ssd1306.drawText(fontId, hello, anchorX, anchorY);

        anchorX = static_cast<uint8_t>(anchorX - fontWidth/2);
        anchorY = static_cast<uint8_t>(fontHeight);

        ssd1306.drawText(fontId, world, anchorX, anchorY);


        ssd1306.refreshDisplay();


    }

}
