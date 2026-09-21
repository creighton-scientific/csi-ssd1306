# Create a library for drivers

add_library(core
        assertion.hpp
        board-config.hpp
        board-config.cpp
        gpio.cpp
        gpio.hpp
        gpio-declarations.hpp
        logger.hpp
        logger.cpp
        serial-comm.hpp
        serial-comm.cpp
        utilities.cpp
        utilities.hpp
)


target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PRIVATE
        # pico interfaces here
#        hardware_gpio
#        hardware_i2c
#        pico_stdlib
        pico_stdio_usb
#        pico_unique_id
)
