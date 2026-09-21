
# Create a library for devices

add_library(devices
        csi2c.hpp
        devicesContainer.hpp
        ssd1306/font5x8.hpp
        ssd1306/font8x8.hpp
        ssd1306/font12x16.hpp
        ssd1306/font16x32.hpp
        ssd1306/font-base.hpp
        ssd1306/fontContainer.hpp
        ssd1306/ssd1306.cpp
        ssd1306/ssd1306.hpp
)

target_include_directories(devices PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Pico SDK libraries this module needs
target_link_libraries(devices PUBLIC
        core
# pico interfaces here
        hardware_adc
#        hardware_gpio
        hardware_i2c
#       hardware_pwm
#       hardware_spi
#       hardware_timer
#       hardware_uart
#       pico_multicore
#       pico_stdlib
#       pico_sync
)
