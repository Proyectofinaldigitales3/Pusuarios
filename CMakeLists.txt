cmake_minimum_required(VERSION 3.13)

# Always include it
include(pico_sdk_import.cmake)


# Project's name (Replace practica_3 with your own project's name)
project(practica_3 C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# SDK Initialization - Mandatory
pico_sdk_init()

# C/C++ project files
add_executable(practica_3
    main.c
    tcl.c
    s_luminosa.c
)

# pico_stdlib library. You can add more if they are needed
target_link_libraries(practica_3 pico_stdlib)

#target_link_libraries(practica_3 pico_stdlib hardware_gpio pico_sync)

# Enable usb output, disable uart output
pico_enable_stdio_usb(practica_3 1)
pico_enable_stdio_uart(practica_3 0)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(practica_3)