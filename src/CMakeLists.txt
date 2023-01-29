add_executable(${PROJECT_NAME} main.c buttons.c rgb.c config.c usb_descriptors.c)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wfatal-errors -O3)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_multicore pico_stdlib hardware_pio hardware_pwm hardware_flash
    tinyusb_device tinyusb_board)


# Make a UF2 binary
pico_add_extra_outputs(${PROJECT_NAME})
