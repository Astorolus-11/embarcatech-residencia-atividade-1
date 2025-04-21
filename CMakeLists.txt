# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)


set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(embarcatech-residencia-atividade-1 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(embarcatech-residencia-atividade-1 embarcatech-residencia-atividade-1.c ssd1306.c)

pico_generate_pio_header(embarcatech-residencia-atividade-1 ${CMAKE_CURRENT_LIST_DIR}/matriz_leds.pio)

pico_set_program_name(embarcatech-residencia-atividade-1 "embarcatech-residencia-atividade-1")
pico_set_program_version(embarcatech-residencia-atividade-1 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(embarcatech-residencia-atividade-1 1)
pico_enable_stdio_usb(embarcatech-residencia-atividade-1 1)

# Add the standard library to the build
target_link_libraries(embarcatech-residencia-atividade-1
        hardware_i2c
        pico_time
        hardware_pio
        hardware_clocks
        hardware_adc
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(embarcatech-residencia-atividade-1 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(embarcatech-residencia-atividade-1 
        
        )

pico_add_extra_outputs(embarcatech-residencia-atividade-1)

