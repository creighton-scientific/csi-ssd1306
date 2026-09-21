
#pragma once

#include <cstdint>
#include <string_view>
#include <utility>


namespace CSdevices {

    enum class FontId {
        FONT_5X8,
        FONT_8X8,
        FONT_12X16,
        FONT_16X32
    };

    inline std::string_view fontToStringView (const FontId f) {
        switch (f) {
            using enum FontId;

            case FONT_5X8: return "FONT_5X8";
            case FONT_8X8: return "FONT_8X8";
            case FONT_12X16: return "FONT_12X16";
            case FONT_16X32: return "FONT_16X32";
        }
        std::unreachable();
    }


    // The primary purpose of this is to allow all fonts to be listed in the font container.
    // As it makes sense, we'll pull common functionality down into here from the font declarations/implementations.

    class Font {
    public:
        Font (  const uint8_t width,
                const uint8_t height,
                const uint8_t minChar,
                const uint8_t maxChar,
                const uint8_t glyphElementCount):
                            width_(width),
                            height_(height),
                            minChar_(minChar),
                            maxChar_(maxChar),
                            glyphElementCount_(glyphElementCount) {}
        virtual ~Font () = default;

        [[nodiscard]] auto getWidth () const {return width_;}
        [[nodiscard]] auto getHeight () const {return height_;}
        [[nodiscard]] auto getMinChar () const {return minChar_;}
        [[nodiscard]] auto getMaxChar () const {return maxChar_;}
        [[nodiscard]] auto getGlyphElementCount () const {return glyphElementCount_;}

//        [[nodiscard]] uint16_t getCharIndex(const char c) const {return (c - minChar_) * (width_ * height_ / 8); }

        // glyphId is 0 : (0x7e - 0x21) or 0 : 0x5d; 0x5d == 93 decimal
        // An out of bounds character ( < 0x21 or > 0x7e) is represented as glyphId 0x7f.
        static constexpr uint8_t MAX_GLYPH_ID = 0x5d;
        static constexpr uint8_t MIN_GLYPH_ID = 0x21;
        static constexpr uint8_t OUT_OF_BOUNDS_GLYPH_ID = 0x7f;

        [[nodiscard]] uint8_t getGlyphId (const char c) const {
            if (c < minChar_ || c > maxChar_) return OUT_OF_BOUNDS_GLYPH_ID;
            return static_cast<uint8_t>(c - minChar_);
        }

        [[nodiscard]] virtual uint8_t getGlyphElement (uint8_t glyphId, uint8_t elementId) const = 0;

    private:
        uint8_t width_;
        uint8_t height_;
        uint8_t minChar_;
        uint8_t maxChar_;
        uint8_t glyphElementCount_;
    };


}
