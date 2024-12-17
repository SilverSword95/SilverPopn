/*
 * Pico Popn Controller Main
 * WHowe <github.com/whowechina>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "bsp/board.h"
#include "pico/bootrom.h"
#include "pico/stdio.h"

#include "tusb.h"
#include "usb_descriptors.h"

#include "buttons.h"
#include "rgb.h"
#include "config.h"

#include "maple.h" //Dreamcast mode
#include "controller_simulator.h" //PlayStation mode

#define BUTTON_LIGHT_MAX_NUM 32 /* Must be larger than number of buttons */
#define TUD_MOUNT_TIMEOUT	1500
bool button_lights[BUTTON_LIGHT_MAX_NUM] = {0};
bool tud_mount_status = false;
bool psx_enabled = false;

struct report
{
    uint16_t buttons;
} report;

void main_loop()
{
    if (tud_hid_ready()) {
        tud_hid_n_report(0x00, REPORT_ID_JOYSTICK, &report, sizeof(report));
    }
}

void boot_check()
{
    uint64_t key1 = (1 << 9);
    uint64_t key2 = (1 << 10);
    uint64_t buttons = button_read();
    if (buttons & key1) {
        reset_usb_boot(0, 2); //Update Mode Switch
    } else if (buttons & key2) {
		dreamcast_main(); //Dreamcast Mode Switch
	}
}

void init()
{
    board_init();
	stdio_init_all();
    button_init();
	boot_check();
	tusb_init();
    config_init();
}

int main(void)
{
    init();

    while (1)
    {
        tud_task();
        report.buttons = button_read();
		if(to_ms_since_boot(get_absolute_time()) > TUD_MOUNT_TIMEOUT && !tud_mount_status && !psx_enabled) { psx_init(); psx_enabled = true; } //PlayStation Mode Switch
		if (psx_enabled == true) { psx_task(report.buttons); }
        main_loop();
        button_update();
        config_loop();
    }

    return 0;
}

// Invoked when device is mounted
void tud_mount_cb(void) {
	tud_mount_status = true;
	rgb_init();
	config_init();
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen)
{
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize)
{
    if ((report_id == REPORT_ID_LIGHTS) &&
        (report_type == HID_REPORT_TYPE_OUTPUT)) {
        if (bufsize < 11 + 3) { /* including logo rgb */
            return;
        }
        for (int i = 0; i < 11; i++) {
            button_lights[i] = (buffer[i] > 0);
        }
        button_set_light(buffer, 11);

        uint8_t const *rgb = buffer + 11;
        rgb_update_logo(rgb[0], rgb[1], rgb[2]);
    }
}
