
# Create the executable
add_executable(${PROJECT_NAME}
    main.cpp
    # other source files here

)

# Include directories for this target
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against libraries AND Pico SDK
target_link_libraries(${PROJECT_NAME}
        cmdctl
        core
        devices
        drivers
        tests
        pico_stdlib
        hardware_gpio
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Enable USB output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
