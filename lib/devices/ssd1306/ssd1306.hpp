#pragma once

#include <cstring>
#include <type_traits>

#include "../csi2c.hpp"
#include "font-base.hpp"
#include "fontContainer.hpp"

namespace CSdevices {

    class Ssd1306 final {
    public:

        enum class DeviceDimensions : uint8_t {
            W128xH64,
            W128xH32
        };
        static constexpr uint8_t deviceDimensionsToUint8_t (DeviceDimensions dd) {
            return static_cast<std::underlying_type_t<DeviceDimensions>>(dd);
        }
        static uint8_t getDevicePixelWidth (DeviceDimensions dd) {
            return 128;
        }
        static uint8_t getDevicePixelHeight (const DeviceDimensions dd) {
            return DeviceDimensions::W128xH64 == dd ? 64 : 32;
        }

        enum class WriteMode : uint8_t {
            ON,                 // call to setPixel turns on the pixel at x,y
            OFF,                // call to setPixel turns off the pixel at x,y
            INVERTED            // call to setPixel applies XOR of value at x,y and the set value
        };

        enum class TextRotation : uint8_t {
            NO_ROTATION,        // Normal setting : left to right
            ROTATE_90_DEGREES   // 90 rotation: bottom to top
        };

        enum class Ssd1306DeviceAddress : uint8_t {
            DEVICE_3C = 0x3C,
            DEVICE_3D = 0x3D
        };

        static uint8_t ssd1306DeviceAddressToUint8_t (Ssd1306DeviceAddress addr) {
            return static_cast<std::underlying_type_t<Ssd1306DeviceAddress>>(addr);
        }

        enum class Ssd1306Devices : uint8_t { // Four possible instances: i2c controller 0, 1; i2c address 0x3c, 0x3d.
            SSD1306_0_3C    = 0,
            SSD1306_0_3D,
            SSD1306_1_3C,
            SSD1306_1_3D,
        };

        struct FrameBuffer {    // This creates a fb large enough for 128x64 pixels. So we have to explicitly provide size.
            uint8_t cmd{};      // Putting the cmd right here makes it easy to send commands - no need to copy to a buffer.
            uint8_t buffer[128 * 64 / 8]{};
        };

        // command constants.
        static constexpr uint8_t SET_CONTRAST                   = 0x81;
        static constexpr uint8_t SET_SEGMENT_MAP_NORMAL         = 0xA0;
        static constexpr uint8_t SET_COM_SCAN_NORMAL            = 0xC0;
        static constexpr uint8_t SET_COM_OUTPUT_SCAN_FORWARD    = SET_COM_SCAN_NORMAL;// convenient alias
        static constexpr uint8_t SET_COM_SCAN_REMAPPED          = 0xC8;
        static constexpr uint8_t SET_COM_OUTPUT_SCAN_BACKWARD   = SET_COM_SCAN_REMAPPED;// convenient alias
        static constexpr uint8_t SET_SEGMENT_MAP_REVERSED       = 0xA1; // reversed is flipped.
        static constexpr uint8_t SET_RESUME_GDDRAM_DISPLAY      = 0xA4; // Sets it on with respect to the GDDRAM values.
        static constexpr uint8_t SET_FORCE_ENTIRE_DISPLAY_ON    = 0xA5; // Sets it on independent of the GDDRAM values. All pixels on.
        static constexpr uint8_t SET_DISPLAY_ON                 = 0xAF; // Normal operation. Powers up. Follows normal init sequence. VCC stable.
        static constexpr uint8_t SET_DISPLAY_OFF                = 0xAE; // Sleep mode. SEG/COM outputs go to VSS/high-Z.
        static constexpr uint8_t SET_NORMAL_DISPLAY             = 0xA6; // White text on black background.
        static constexpr uint8_t SET_INVERSE_DISPLAY            = 0xA7; // Black text on white background.
        static constexpr uint8_t SET_MEMORY_ADDRESSING_MODE     = 0x20; // 1 byte arg: 00 - Horizonal, 01 - Vertical, 10 - Page.
        static constexpr uint8_t SET_COLUMN_ADDRESS             = 0x21; // 2 bytes arg: start, end; 0 - 127.
        static constexpr uint8_t SET_PAGE_ADDRESS               = 0x22; // 2 bytes arg: start, end; 0 - 7.
        static constexpr uint8_t SET_CLOCK_DIVIDE               = 0xD5;
        static constexpr uint8_t SET_MUX_RATIO                  = 0xA8;
        static constexpr uint8_t SET_DISPLAY_OFFSET             = 0xD3;
        static constexpr uint8_t SET_COM_PINS_HW_CONFIG         = 0xDA; // 10.1.18;
        static constexpr uint8_t SET_START_LINE                 = 0x40; // The line indicator is in bits 0-5. Table 9-1.4
        static constexpr uint8_t SET_CHARGE_PUMP                = 0x8D; // argument sets it enabled or disabled
        static constexpr uint8_t SET_PRE_CHARGE_PERIOD          = 0xD9;
        static constexpr uint8_t SET_VCOMH_DESELECT_LEVEL       = 0xDB; // 10.1.19

        // command argument declarations. Some don't have constexpr since they are dynameic.
        static constexpr uint8_t ARG_HORIZONTAL_ADDRESSING_MODE = 0x00;
        static constexpr uint8_t ARG_VERTICAL_ADDRESSING_MODE   = 0x01;
        static constexpr uint8_t ARG_PAGE_ADDRESSING_MODE       = 0x02;
        static constexpr uint8_t ARG_MUX_16                     = 0x10 - 1; // See table 4 section 9 of the datasheet for valid values 0x0f-0x3f
        static constexpr uint8_t ARG_MUX_17                     = 0x11 - 1;
        static constexpr uint8_t ARG_MUX_18                     = 0x12 - 1;
        static constexpr uint8_t ARG_MUX_19                     = 0x13 - 1;
        static constexpr uint8_t ARG_MUX_20                     = 0x14 - 1;
        static constexpr uint8_t ARG_MUX_21                     = 0x15 - 1;
        static constexpr uint8_t ARG_MUX_22                     = 0x16 - 1;
        static constexpr uint8_t ARG_MUX_23                     = 0x17 - 1;
        static constexpr uint8_t ARG_MUX_24                     = 0x18 - 1;
        static constexpr uint8_t ARG_MUX_25                     = 0x19 - 1;
        static constexpr uint8_t ARG_MUX_26                     = 0x1a - 1;
        static constexpr uint8_t ARG_MUX_27                     = 0x1b - 1;
        static constexpr uint8_t ARG_MUX_28                     = 0x1c - 1;
        static constexpr uint8_t ARG_MUX_29                     = 0x1d - 1;
        static constexpr uint8_t ARG_MUX_30                     = 0x1e - 1;
        static constexpr uint8_t ARG_MUX_31                     = 0x1f - 1;
        static constexpr uint8_t ARG_MUX_32                     = 0x20 - 1; // This continues until 0x3f
        static constexpr uint8_t ARG_MUX_64                     = 0x40 - 1; // Fill in others as needed, or just send the literal.
        static constexpr uint8_t ARG_CHARGE_PUMP_ENABLED        = 0x14;
        static constexpr uint8_t ARG_CHARGE_PUMP_DISABLED       = 0x10;
        static constexpr uint8_t ARG_COM_PINS_HW_CONFIG         = 0x12; // RESET; Alternative COM pin configuration
        static constexpr uint8_t ARG_PRE_CHARGE_PERIOD          = 0xF1; // See note2 in ssd1306-cmd-ref.md
        static constexpr uint8_t ARG_VCOMH_DESELECT_RESET       = 0x20; // VCOMH deselect level 0.75 x VCC; Table 9-1.5

        static constexpr size_t I2C_PAYLOAD_SIZE_H64 = sizeof(FrameBuffer::buffer) + sizeof(FrameBuffer::cmd);
        static constexpr size_t I2C_PAYLOAD_SIZE_H32 = sizeof(FrameBuffer::buffer)/2 + sizeof(FrameBuffer::cmd);
        // Control-byte constants from datasheet Figure 8-7
        static constexpr uint8_t CTRL_CMD_STREAM  = 0x00; // Co=0, D/C#=0
        static constexpr uint8_t CTRL_DATA_STREAM = 0x40; // Co=0, D/C#=1
        //
        static constexpr uint8_t MAX_ARGS = 16;     // This could be adjusted based on the specific use case.


        Ssd1306(const CsI2C::ControllerId controllerId,
                const Ssd1306DeviceAddress i2cDeviceAddress,
                const FontId activeFontId = FontId::FONT_12X16,
                const DeviceDimensions dimensions = DeviceDimensions::W128xH64,
                const WriteMode writeMode = WriteMode::ON,
                const TextRotation tr = TextRotation::NO_ROTATION) :
                                                    txSize_(dimensions == DeviceDimensions::W128xH64
                                                                ? I2C_PAYLOAD_SIZE_H64
                                                                : I2C_PAYLOAD_SIZE_H32),
                                                    controllerId_(controllerId),
                                                    i2cDeviceAddress_(i2cDeviceAddress),
                                                    activeFontId_(activeFontId),
                                                    dimensions_(dimensions),
                                                    height_(DeviceDimensions::W128xH64 == dimensions ? 64 : 32),
                                                    width_(128),
                                                    writeMode_(writeMode),
                                                    textRotation_(tr) {
            // Initialize the device
            ready_ = sendCommand(SET_DISPLAY_OFF);
            ready_ &= sendCommand(SET_CLOCK_DIVIDE, {0x80});   // Set default clock divide ratio.
            ready_ &= sendCommand(SET_MUX_RATIO, {height_ == 64 ? ARG_MUX_64 : ARG_MUX_32});// Default mux ratio is 63.
            ready_ &= sendCommand(SET_DISPLAY_OFFSET, {0x00});
            ready_ &= sendCommand(SET_START_LINE);// This starts at line 0. To start at line 1, pass SET_START_LINE + 1
            ready_ &= sendCommand(SET_CHARGE_PUMP, {ARG_CHARGE_PUMP_ENABLED});
            ready_ &= sendCommand(SET_MEMORY_ADDRESSING_MODE, {ARG_HORIZONTAL_ADDRESSING_MODE});
            ready_ &= sendCommand(SET_SEGMENT_MAP_NORMAL);
            ready_ &= sendCommand(SET_COM_OUTPUT_SCAN_FORWARD);
            ready_ &= sendCommand(SET_COM_PINS_HW_CONFIG, {ARG_COM_PINS_HW_CONFIG});
            ready_ &= sendCommand(SET_PRE_CHARGE_PERIOD, {ARG_PRE_CHARGE_PERIOD});
            ready_ &= sendCommand(SET_VCOMH_DESELECT_LEVEL, {ARG_VCOMH_DESELECT_RESET});
            resetFramebuffer();
            ready_ &= flush();
            ready_ &= setPositiveOrientation();
//            ready_ &= setNegativeOrientation();
            ready_ &= sendCommand(SET_RESUME_GDDRAM_DISPLAY);
            ready_ &= sendCommand(SET_NORMAL_DISPLAY);
            ready_ &= sendCommand(SET_DISPLAY_ON);

            ready_ &= selfTestAllOn(3000);
        }

        Ssd1306(const Ssd1306&) = delete;
        Ssd1306& operator=(const Ssd1306&) = delete;

        ~Ssd1306() = default;

        [[nodiscard]] bool isReady () const {return ready_;}

        // High-level API for basic pixel manipulation.
        [[nodiscard]] bool selfTestAllOn (uint32_t msDelay) const;

        [[nodiscard]] bool xyAreValid (const uint8_t x, const uint8_t y) const {
            return (x < width_ && y < height_);
        }

        bool clearScreen ();
        [[nodiscard]] bool displayForceOn () const {return sendCommand(SET_FORCE_ENTIRE_DISPLAY_ON);}   // Idependent of GDDRAM
        [[nodiscard]] bool displayOff() const {return sendCommand(SET_DISPLAY_OFF);}
        [[nodiscard]] bool flush ();  // copy all of framebuffer to GDDRAM.
        [[nodiscard]] bool setPositiveOrientation () const; // Normal screen orientation.
        [[nodiscard]] bool setNegativeOrientation () const; // Upside-down screen orientation.
        bool refreshDisplay ();
        [[nodiscard]] uint8_t getWidth () const {return width_;}
        [[nodiscard]] uint8_t getHeight () const {return height_;}
        [[nodiscard]] uint8_t getByte (const uint8_t x, const uint8_t y) const { // x,y must be valid!
            if (x >= width_ || y >= height_) {
                return frameBuffer_.buffer[(y / 8) * width_ + x];
            }
            return 0;   // No way to really tell the caller they made a mistake.
        }
        void setAllBytes (const uint8_t value) {
            memset(frameBuffer_.buffer, value, sizeof(frameBuffer_.buffer));
        }

        // Text rendering interface

        [[nodiscard]] FontId getActiveFontId () const {return activeFontId_;}
        void setActiveFontId (const FontId id) {activeFontId_ = id;}
        [[nodiscard]] auto getActiveFontWidth () const -> uint8_t {return getFont(getActiveFontId()).getWidth();}
        [[nodiscard]] auto getActiveFontHeight () const -> uint8_t {return getFont(getActiveFontId()).getHeight();}

        void drawText (const char * cText, const uint8_t anchor_x, const uint8_t anchor_y) {
            drawText(getActiveFontId(), cText, anchor_x, anchor_y);
        }
        void drawText (FontId fontId, const char * cText, uint8_t anchor_x, uint8_t anchor_y);
        void drawChar (FontId fontId, char c, uint8_t anchor_x, uint8_t anchor_y);  /// blit: bit block transfer
        [[nodiscard]] TextRotation getTextRotation () const {return textRotation_;}
        void setTextRotation (const TextRotation tr) {textRotation_ = tr;}

        // Pixel handling interface
        [[nodiscard]] WriteMode getWriteMode () const {return writeMode_;}
        void setWriteMode (const WriteMode wm) {writeMode_ = wm;}

        void setPixel  (uint8_t x, uint8_t y);  // This sets the pixel ON, OFF, or INVERTED based on value of writeMode_

        void setPixelOn (uint8_t x, uint8_t y);     // Convenience method sets mode to ON, calls setPixel, then restores mode.
        void setPixelOff (uint8_t x, uint8_t y);    // Convenience method sets mode to OFF, calls setPixel, then restores mode.
        void setPixelXor (uint8_t x, uint8_t y);    // Convenience method sets mode to INVERT, calls setPixel, then restores mode.
        void invertPixel (const uint8_t x, const uint8_t y) {return setPixelXor(x, y);} // Friendlier name.
        bool setAllOn () {
            setFramebuffer(0xff);
            return flush();
        }
        bool setAllOff () {
            setFramebuffer(0);
            return flush();
        }
        void setByte (const uint16_t ix, const uint8_t value) {
            if (ix < width_ * height_) {
                getTargetByteReference(ix) = value;
            }
        }
        void setByteOr (const uint16_t ix, const uint8_t value) {
            if (ix < width_ * height_) {
                getTargetByteReference(ix) |= value;
            }
        }
        void setByteAnd (const uint16_t ix, const uint8_t value) {
            if (ix < width_ * height_) {
                getTargetByteReference(ix) &= value;
            }
        }
        void setByteXor (const uint16_t ix, const uint8_t value) {
            if (ix < width_ * height_) {
                getTargetByteReference(ix) ^= value;
            }
        }
        void setByte (const uint8_t x, const uint8_t y, const uint8_t value) {
            if (xyAreValid(x,y)) {
                getTargetByteReference(x, y) = value;
            }
        }
        void setByteOr (const uint8_t x, const uint8_t y, const uint8_t value) {
            if (xyAreValid(x,y)) {
                setByte(x, y, getTargetByteReference(x, y) | value);
            }
        }
        void setByteAnd (const uint8_t x, const uint8_t y, const uint8_t value) {
            if (xyAreValid(x,y)) {
                setByte(x, y, getTargetByteReference(x, y) & value);
            }
        }
        void setByteXor (const uint8_t x, const uint8_t y, const uint8_t value) {
            if (xyAreValid(x,y)) {
                setByte(x, y, getTargetByteReference(x, y) ^= value);
            }
        }
        void resetFramebuffer () {
            frameBuffer_.cmd = CTRL_CMD_STREAM; // This doesn't really matter since it gets overwritten by flush(), etc.
            setFramebuffer(0);
        }
        void setFramebuffer (const uint8_t value) {
            memset(frameBuffer_.buffer, value, sizeof(frameBuffer_.buffer));
        }

    private:

        [[nodiscard]] bool sendCommand (const uint8_t command) const {
            const uint8_t buf[2] = { CTRL_CMD_STREAM, command };
            constexpr auto byteCount = sizeof(buf);
            const auto retVal = CsI2C::writeBuffer(  controllerId_,
                                                        ssd1306DeviceAddressToUint8_t(i2cDeviceAddress_),
                                                        buf,
                                                        byteCount);
            return retVal == byteCount;
        }

        [[nodiscard]] bool sendCommand (const uint8_t command,
                                        const std::initializer_list<uint8_t> args) const {
            uint8_t buf[MAX_ARGS + 1] = { CTRL_CMD_STREAM, command };
            size_t ix = 2;  // CTRL_CMD_STREAM at 0, command at 1. Data starts at 2
            for (const auto arg : args) {
                buf[ix++] = arg;
            }
            const auto byteCount = 2 + args.size();
            const auto retVal = CsI2C::writeBuffer(  controllerId_,
                                                        ssd1306DeviceAddressToUint8_t(i2cDeviceAddress_),
                                                        buf,
                                                        byteCount);
            return retVal == byteCount;
        }

        /*
        void setTargetByte (const uint8_t x, const uint8_t y, const uint8_t value) {
            if (x < width_ && y < height_) {
                getTargetByte(x, y) = value;
            }
        }

        void setTargetByte (const uint16_t ix, const uint8_t value) {
            if (ix < width_ * height_) {
                getTargetByte(ix) = value;
            }
        }
        */

        [[nodiscard]] uint8_t &getTargetByteReference (const uint8_t x, const uint8_t y) { // x,y must be valid!
            return frameBuffer_.buffer[(y / 8) * width_ + x];
        }

        uint8_t &getTargetByteReference (const uint16_t ix) {
            return frameBuffer_.buffer[ix];
        }

        static uint8_t getBitMask (const uint8_t y) { // y must be valid!
            // TODO: Handle 32X128 by doubling the y bits!
            return 1 << (y % 8);
        }

        bool ready_ = false; // Indicates whether the device is ready for communication.
        FrameBuffer frameBuffer_{};
        size_t txSize_{};
        CsI2C::ControllerId controllerId_;
        Ssd1306DeviceAddress i2cDeviceAddress_;
        FontId activeFontId_;
        DeviceDimensions dimensions_{DeviceDimensions::W128xH64};
        uint8_t height_{};
        uint8_t width_{};
        WriteMode writeMode_{WriteMode::ON};
        TextRotation textRotation_{TextRotation::NO_ROTATION};
    };

}
