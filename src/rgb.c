/*
 * Pico Popn Controller Buttons
 * WHowe <github.com/whowechina>
 * 
 * A button consists of a switch and an LED
 */

#include "buttons.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/timer.h"

#include "ws2812.pio.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#if defined BOARD_POPN_PICO
static const uint8_t ws2812_pin = 28;
#define RGB_LED_NUM 10
static const uint8_t logo_leds[] = {0, 1, 2};
static const uint8_t effect_leds[] = {3, 4, 5, 6, 7, 8, 9};
#else
static const uint8_t ws2812_pin = 18;
#define RGB_LED_NUM 60
static const uint8_t logo_leds[] = {18, 19, 20, 21};
static const uint8_t effect_leds[] = {46, 47, 48, 49, 50, 51, 52, 53};
#endif

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static const uint8_t level_val[] = {0, 32, 64, 96, 128, 160, 192, 224, 255};
static uint8_t rgb_level = 0;

static inline uint32_t urgb_u32(uint32_t r, uint32_t g, uint32_t b)
{
    r = r * level_val[rgb_level] / 255;
    g = g * level_val[rgb_level] / 255;
    b = b * level_val[rgb_level] / 255;

    /* better gamma */
    r = (r + 1) * (r + 1) - 1;
    g = (g + 1) * (g + 1) - 1;
    b = (b + 1) * (b + 1) - 1;
    return (g >> 8 << 16) | (r >> 8 << 8) | (b >> 8 << 0);
}

/* 6 segment regular hsv color wheel, better color cycle
 * https://www.arnevogel.com/rgb-rainbow/
 * https://www.instructables.com/How-to-Make-Proper-Rainbow-and-Random-Colors-With-/
 */
#define COLOR_WHEEL_SIZE (256 * 6)
static uint32_t color_wheel[COLOR_WHEEL_SIZE];
static void generate_color_wheel()
{
    for (int i = 0; i < COLOR_WHEEL_SIZE; i++) {
        uint32_t sector = i / 256 % 6;
        uint8_t incr = i % 256;
        uint8_t decr = 255 - incr;
        if (sector == 0) {
            color_wheel[i] = urgb_u32(incr, 0, 255);
        } else if (sector == 1) {
            color_wheel[i] = urgb_u32(255, 0, decr);
        } else if (sector == 2) {
            color_wheel[i] = urgb_u32(255, incr, 0);
        } else if (sector == 3) {
            color_wheel[i] = urgb_u32(decr, 255, 0);
        } else if (sector == 4) {
            color_wheel[i] = urgb_u32(0, 255, incr);
        } else {
            color_wheel[i] = urgb_u32(0, decr, 255);
        }
    }
}

void rgb_set_brightness(uint8_t level)
{
    if (rgb_level == level) {
        return;
    }
    rgb_level = level;
    generate_color_wheel();
}

static bool pio_paused = false;
void rgb_pause(bool pause)
{
    pio_paused = pause;
}

static uint32_t led_buf[RGB_LED_NUM] = {0};
void update_led()
{
    if (pio_paused) {
        return;
    }

    for (int i = 0; i < ARRAY_SIZE(led_buf); i++) {
        put_pixel(led_buf[i]);
    }
}

#define HID_EXPIRE_DURATION 1000000ULL
static uint32_t hid_expire_time = 0;

#define RAINBOW_SPEED_MAX 100
#define RAINBOW_SPEED_MIN 4
#define RAINBOW_PITCH 151
#define RAINBOW_SPEED_DOWN_INTERVAL 200000ULL
uint32_t speed = RAINBOW_SPEED_MIN;

void rainbow_update()
{
    static uint32_t rotator = 0;
    rotator = (rotator + speed) % COLOR_WHEEL_SIZE;

    for (int i = 0; i < sizeof(effect_leds); i++) {
        uint32_t index = (rotator + RAINBOW_PITCH * i) % COLOR_WHEEL_SIZE;
        led_buf[effect_leds[i]] = color_wheel[index];
    }
}

void rainbow_speed_up()
{
    if (speed < RAINBOW_SPEED_MAX) {
        speed++;
    }
}

void rainbow_speed_down()
{
    static uint64_t next_change_time = 0;
    uint64_t now = time_us_64();
    if (now >= next_change_time) {
        next_change_time = now + RAINBOW_SPEED_DOWN_INTERVAL;
        if (speed > RAINBOW_SPEED_MIN) {
            speed = speed * 95 / 100;
        }
    }
}

void rgb_update_logo(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = urgb_u32(r, g, b);
    for (int i = 0; i < sizeof(logo_leds); i++) {
        led_buf[logo_leds[i]] = color;
    }
    hid_expire_time = time_us_64() + HID_EXPIRE_DURATION;
}

static void hid_update()
{

}

void rgb_stimulate()
{
    rainbow_speed_up();
}

void rgb_entry()
{
    while (1) {
        rainbow_update();
        hid_update();
        rainbow_speed_down();
        update_led();
        sleep_ms(10);
    }
}

void rgb_init()
{
    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, ws2812_pin, 800000, false);
    multicore_launch_core1(rgb_entry);
    rgb_set_brightness(8);
}
