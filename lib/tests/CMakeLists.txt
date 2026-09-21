
# Create a library for tests

add_library(tests
    test.cpp
    test.hpp
    test-ssd1306.cpp
    test-ssd1306.hpp
)

target_include_directories(tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Base and Pico SDK libraries this module needs
target_link_libraries(tests PUBLIC
        core
        devices
        drivers
# pico interfaces here
#       hardware_adc
#        hardware_gpio
#        hardware_i2c
#       hardware_pwm
#       hardware_spi
#       hardware_timer
#       hardware_uart
#       pico_multicore
#        pico_stdlib
#       pico_sync
)
