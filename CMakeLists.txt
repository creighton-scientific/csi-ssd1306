
# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13...3.30.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 0)
message("Pico home: ${PICO_SDK_PATH}")
include(pico_sdk_import.cmake)

project(csi-ssd1306 C CXX ASM)	# CHANGE ME!

# Configure before sdk init!
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(PICO_DEOPTIMIZED_DEBUG 1)
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	message("Building with asserts enabled.")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	message("Building with asserts disabled.")
endif()

pico_sdk_init()

message(STATUS "Starting configuration.")
include(CMakePrintHelpers)
cmake_policy(SET CMP0054 NEW)	# ensures that quoted strings inside if() are always treated as literal strings, not as variable names.

add_subdirectory(lib/cmdctl)
add_subdirectory(lib/core)
add_subdirectory(lib/devices)
add_subdirectory(lib/drivers)
add_subdirectory(lib/tests)
add_subdirectory(src)

# Handle board revision via a single variable
set(BOARD_REV "A" CACHE STRING "Board revision (0, A, B)")
set_property(CACHE BOARD_REV PROPERTY STRINGS 0 A B)

# Set the version of c++ to use.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

if(BOARD_REV STREQUAL "0")
	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_CSI_SSD1306_REV_0)
	message(STATUS "Building for csi_ssd1306 Rev 0")
#elseif(BOARD_REV STREQUAL "A")
#	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_CSI_SSD1306_REV_A)
#	message(STATUS "Building for csi_ssd1306 Rev A")
#elseif(BOARD_REV STREQUAL "B")
#	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_CSI_SSD1306_REV_B)
#	message(STATUS "Building for csi_ssd1306 Rev B")
#else()
#	message(FATAL_ERROR "Unknown BOARD_REV: ${BOARD_REV}")
endif()

message(STATUS "\nAfter add_executable and targets\n")
cmake_print_variables(PROJECT_NAME)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
cmake_print_variables(CMAKE_CXX_FLAGS)

cmake_print_variables($ENV{PICO_SDK_PATH})


# Create map/bin/hex/uf2 files
#pico_add_extra_outputs(${PROJECT_NAME})


# Enable usb and uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
