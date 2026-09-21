#pragma once

#include "devicesContainer.hpp"
#include "ssd1306/font-base.hpp"
#include "ssd1306/ssd1306.hpp"

namespace CSdrivers {

    class Ssd1306Text {
    public:
        explicit Ssd1306Text (  const CSdevices::Ssd1306::Ssd1306Devices deviceId,
                                const CSdevices::FontId activeFontId = CSdevices::FontId::FONT_12X16) :
                                                                                deviceId_(deviceId) {
            CSdevices::getSsd1306(deviceId_).setActiveFontId(activeFontId);
        }

        Ssd1306Text(const Ssd1306Text&) = delete;
        Ssd1306Text& operator=(const Ssd1306Text&) = delete;

        ~Ssd1306Text() = default;

        [[nodiscard]] CSdevices::Ssd1306& getSsd1306() const {return CSdevices::getSsd1306(deviceId_);}
        [[nodiscard]] CSdevices::FontId getActiveFontId() const {return getSsd1306().getActiveFontId();}
        void setActiveFontId(const CSdevices::FontId activeFontId) const {getSsd1306().setActiveFontId(activeFontId);}
        [[nodiscard]] CSdevices::Font& getActiveFont() const {return CSdevices::getFont(getActiveFontId());}
        [[nodiscard]] uint8_t getCharWidth() const {return getActiveFont().getWidth();}
        [[nodiscard]] uint8_t getCharHeight() const {return getActiveFont().getHeight();}
        [[nodiscard]] uint8_t getLineCount () const;        // How many lines for the device for active font.
        [[nodiscard]] uint8_t getCharacterCount () const;   // How many characters per line for active font.

        void clearScreen () const {
            getSsd1306().clearScreen();
        }
        void writeCharacterAt(const uint8_t line, const uint8_t column, const char textChar) const {
            const char charArray[] = {textChar, 0};
            const std::string_view text = charArray;
            writeTextAt(line, column, text);
        }
        void writeTextAt (uint8_t line, uint8_t column, std::string_view text) const;
        void writeTextLeftJustifiedAt  (const uint8_t line, const std::string_view text) const {
            writeTextAt(line, 0, text);
        }
        void writeTextRightJustifiedAt(const uint8_t line, const std::string_view text) const {
            writeTextAt(line, getCharacterCount() - text.size(), text);
        }
        void writeTextCenteredAt(const uint8_t line, const std::string_view text) const {
            writeTextAt(line, getCharacterCount() / 2 - text.size() / 2, text);
        }

    private:

        CSdevices::Ssd1306::Ssd1306Devices deviceId_;
    };
}
