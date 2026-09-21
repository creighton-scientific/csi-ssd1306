#pragma once

#include "font-base.hpp"
#include "font5x8.hpp"
#include "font8x8.hpp"
#include "font12x16.hpp"
#include "font16x32.hpp"

namespace CSdevices {

    inline Font5x8& getFont5x8 () {
        static Font5x8 font5X8_ {};
        return font5X8_;
    }

    inline Font8x8& getFont8x8 () {
        static Font8x8 font8X8_ {};
        return font8X8_;
    }

    inline Font12x16& getFont12x16 () {
        static Font12x16 font12X16_ {};
        return font12X16_;
    }

    inline Font16x32& getFont16X32 () {
        static Font16x32 font16X32_ {};
        return font16X32_;
    }

    inline Font& getFont (const FontId font) {
        switch (font) {
            default:
            case FontId::FONT_5X8:
                return getFont5x8();
            case FontId::FONT_8X8:
                return getFont8x8();
            case FontId::FONT_12X16:
                return getFont12x16();
            case FontId::FONT_16X32:
                return getFont16X32();
        }
    }

}
