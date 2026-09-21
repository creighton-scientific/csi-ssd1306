# Create a library for drivers

add_library(cmdctl
        communication.hpp
        communication.cpp
        pystring.cpp
        pystring.h
)


target_include_directories(cmdctl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cmdctl PRIVATE
        core
        devices
        drivers
        # pico interfaces here
        hardware_adc
#        hardware_gpio
#        hardware_i2c
#        hardware_timer
        hardware_uart
        pico_stdlib
        pico_stdio_usb
)
