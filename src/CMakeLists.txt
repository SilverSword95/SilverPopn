function(make_firmware board board_def)
    add_executable(${board} main.cpp buttons.c rgb.c config.c usb_descriptors.c maple.c state_machine.c format.c controller_simulator.cpp)
    target_compile_definitions(${board} PUBLIC ${board_def})
    pico_enable_stdio_usb(${board} 1)
    pico_enable_stdio_uart(${board} 1)

	pico_set_linker_script(${board} ${CMAKE_CURRENT_LIST_DIR}/memmap.ld)
    pico_generate_pio_header(${board} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
	pico_generate_pio_header(${board} ${CMAKE_CURRENT_LIST_DIR}/maple.pio)
	pico_generate_pio_header(${board} ${CMAKE_CURRENT_LIST_DIR}/psxSPI.pio)
    target_compile_options(${board} PRIVATE -Wall -Werror -Wfatal-errors -O3)
    target_include_directories(${board} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(${board} PRIVATE
        pico_multicore pico_stdlib hardware_pio hardware_pwm hardware_flash
        tinyusb_device tinyusb_board hardware_dma)

    # Make a UF2 binary
    pico_add_extra_outputs(${board})
endfunction()

make_firmware(popn_pico BOARD_POPN_PICO)