
#include "ssd1306-text.hpp"


namespace CSdrivers {
    uint8_t Ssd1306Text::getLineCount() const {
        const auto pixelHeight = getSsd1306().getHeight();
        const auto charHeight = getCharHeight();
        return pixelHeight/charHeight;
    }

    uint8_t Ssd1306Text::getCharacterCount() const {
        const auto pixelWidth = getSsd1306().getWidth();
        const auto charWidth = getCharWidth();
        return pixelWidth/charWidth;
    }

    void Ssd1306Text::writeTextAt(const uint8_t line, const uint8_t column, const std::string_view text) const {
        if (line >= getLineCount() || column >= getCharacterCount()) return; // ignore out of range.

        const auto anchorX = column * getCharWidth();
        const auto anchorY = line * getCharHeight();
        getSsd1306().drawText(text.data(), anchorX, anchorY);
        getSsd1306().refreshDisplay();
    }

}
