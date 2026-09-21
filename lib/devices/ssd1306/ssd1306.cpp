
#include "font-base.hpp"
#include "ssd1306.hpp"

#include "fontContainer.hpp"

namespace CSdevices {
    /**
     * @brief This turns on all pixels without regard to GDDRAM. It then pauses ms milliseconds. Then goes to normal mode.
     * @param msDelay is the number of milliseconds to delay after all pixels are turned on.
     * @return true if commands succeed, else false.
     */
    bool Ssd1306::selfTestAllOn(uint32_t msDelay) const {
        auto retVal = true;
        retVal &= sendCommand(SET_FORCE_ENTIRE_DISPLAY_ON);
//        retVal &= sendCommand(SET_DISPLAY_ON);
        sleep_ms(msDelay);
        retVal &= sendCommand(SET_DISPLAY_OFF);
        retVal &= sendCommand(SET_RESUME_GDDRAM_DISPLAY);
        retVal &= sendCommand(SET_DISPLAY_ON);
        return retVal;
    }

    bool Ssd1306::clearScreen() {
        resetFramebuffer();
        auto retVal = flush();
        retVal &= sendCommand(SET_RESUME_GDDRAM_DISPLAY);
        retVal &= sendCommand(SET_DISPLAY_ON);
        return retVal;
    }

    bool Ssd1306::flush() {
        auto retVal = sendCommand (SET_COLUMN_ADDRESS, {0, 127});
        uint8_t maxPageAddress = DeviceDimensions::W128xH64 == dimensions_ ? 0x07 : 0x03;
        retVal &= sendCommand (SET_PAGE_ADDRESS, {0, maxPageAddress});
        if (retVal) {
            frameBuffer_.cmd = CTRL_DATA_STREAM;
            retVal = txSize_ ==
                        CsI2C::writeBuffer( controllerId_,
                                            ssd1306DeviceAddressToUint8_t(i2cDeviceAddress_),
                                            reinterpret_cast<const uint8_t*>(&frameBuffer_),
                                            txSize_);

        }
        return retVal;
    }

    bool Ssd1306::setPositiveOrientation() const {
        auto retVal = sendCommand(SET_SEGMENT_MAP_NORMAL); // 0xA0
        retVal &= sendCommand(SET_COM_SCAN_NORMAL);             // 0xC0
        return retVal;
    }

    bool Ssd1306::setNegativeOrientation() const {
        auto retVal = sendCommand(SET_SEGMENT_MAP_REVERSED);   // 0xA1
        retVal &= sendCommand(SET_COM_OUTPUT_SCAN_BACKWARD);        // 0xC8
        return retVal;
    }

    bool Ssd1306::refreshDisplay() {
        return flush() && sendCommand(SET_RESUME_GDDRAM_DISPLAY) && sendCommand(SET_DISPLAY_ON);
    }

    void Ssd1306::drawText(const FontId fontId, const char * cText, const uint8_t anchor_x, const uint8_t anchor_y) {
        const Font& font = getFont(fontId);
        uint16_t nextChar = 0;

        while (cText[nextChar] != '\0') {
            switch (getTextRotation()) {
                case TextRotation::NO_ROTATION:
                    drawChar(fontId, cText[nextChar], anchor_x + (nextChar * font.getWidth()), anchor_y);
                break;

                case TextRotation::ROTATE_90_DEGREES:   // TODO: Implement 90 degree rotation!
                    setTextRotation(TextRotation::NO_ROTATION);
                    drawText(fontId, cText, anchor_x, anchor_y);
                    return;

            }
            nextChar++;
        }
    }

    void Ssd1306::drawChar(const FontId fontId, const char c, const uint8_t anchor_x, const uint8_t anchor_y) {
        const auto page = static_cast<uint8_t>(anchor_y / 8);
        const Font& font = getFont(fontId);
        const auto fontWidth = font.getWidth();
        const auto fontHeight = font.getHeight();
        const auto glyphHeight = static_cast<uint8_t>(fontHeight / 8);    // This assumes that the y component is multiple of 2.
        const auto glyphId = font.getGlyphId(c);                    // Index to first byte of glyph
        const auto glyphElementCount = font.getGlyphElementCount();

        // Check bounds!
        if (anchor_x + fontWidth > getWidth() || anchor_y + fontHeight > getHeight() || Font::OUT_OF_BOUNDS_GLYPH_ID == glyphId) {
            return; // This means we are about to overflow the page or the width of the screen. Or bad glyph index.
        }

        auto glyphElementId = 0;

        for (uint8_t x = 0; x < fontWidth && glyphElementId < glyphElementCount; x++) {
            for (uint8_t row = 0; row < glyphHeight; row++) {
                const uint16_t targetByte = (page + row) * 128 + (anchor_x + x);

                switch (getWriteMode()) {
                case WriteMode::ON:
                    setByteOr(targetByte, font.getGlyphElement(glyphId, glyphElementId));
                    glyphElementId++;
                    break;
                case WriteMode::OFF:
                    setByteAnd(targetByte, ~font.getGlyphElement(glyphId, glyphElementId));
                    glyphElementId++;
                    break;
                case WriteMode::INVERTED:
                    setByteXor(targetByte, font.getGlyphElement(glyphId, glyphElementId));
                    glyphElementId++;
                }
            }
        }
    }

    /**
     * @brief Sets the pixel at (x, y) to on. Ensures valid x,y.
     * This assumes horizontal or vertical orientation.
     * @param x must be >= 0 && < width_ (128)
     * @param y must be >= 0 && < height_ (32 or 64)
     */
    void Ssd1306::setPixel(const uint8_t x, const uint8_t y) {
        if (!xyAreValid(x, y)) return;

        uint8_t &targetByte = getTargetByteReference(x, y);
        const uint8_t bitMask = getBitMask(y);
        switch (getWriteMode()) {
            case WriteMode::ON:
                targetByte |= bitMask;
                return;
            case WriteMode::OFF:
                targetByte &= ~bitMask;
                return;
            case WriteMode::INVERTED:
                targetByte ^= bitMask;
        }
    }

    /**
     * @brief Sets the pixel at (x, y) to on. Ensures valid x,y.
     * This assumes horizontal or vertical orientation.
     * Maintains instance-level value of writeMode_.
     * @param x must be >= 0 && < width_ (128)
     * @param y must be >= 0 && < height_ (32 or 64)
     */
    void Ssd1306::setPixelOn(uint8_t x, uint8_t y) {
        const WriteMode mode = getWriteMode();
        setWriteMode(WriteMode::ON);
        setPixel(x, y);
        setWriteMode(mode);
    }

    /**
     * @brief Sets the pixel at (x, y) to off. Ensures valid x,y.
     * This assumes horizontal or vertical orientation.
     * Maintains instance-level value of writeMode_.
     * @param x must be >= 0 && < width_ (128)
     * @param y must be >= 0 && < height_ (32 or 64)
     */
    void Ssd1306::setPixelOff(uint8_t x, uint8_t y) {
        const WriteMode mode = getWriteMode();
        setWriteMode(WriteMode::OFF);
        setPixel(x, y);
        setWriteMode(mode);
    }

    /**
     * @brief Toggles the pixel at (x, y). Ensures valid x,y.
     * This assumes horizontal or vertical orientation.
     * Maintains instance-level value of writeMode_.
     * @param x must be >= 0 && < width_ (128)
     * @param y must be >= 0 && < height_ (32 or 64)
     */
    void Ssd1306::setPixelXor(uint8_t x, uint8_t y) {
        const WriteMode mode = getWriteMode();
        setWriteMode(WriteMode::INVERTED);
        setPixel(x, y);
        setWriteMode(mode);
    }
}

