
#include "board-config.hpp"

#include <cstdlib>

#include "gpio.hpp"
#include "serial-comm.hpp"

namespace CScore {
    bool boardInit() {
        auto retCode = true;

        if (!stdio_init_all()) {    // get set to handle stdio
            exit(-1);
        }

        Gpio::init();

        if (!SerialComm::initUsb()) { // if no usb, try uart; only return one or the other
            retCode |= SerialComm::initUart();
        }

        return retCode;
    }

}
