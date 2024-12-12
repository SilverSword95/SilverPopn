/*
 * Pico Popn Controller Board Definitions
 * WHowe <github.com/whowechina>
 */

/* A button consists of a switch and an LED
   9 main buttons + 2 aux buttons. */
#define BUTTON_DEF { \
    {0, 1}, \
    {2, 3}, \
    {4, 5}, \
    {6, 7}, \
    {8, 9}, \
    {14, 15}, \
    {17, 16}, \
    {19, 18}, \
    {21, 20}, \
    {26, -1}, \
    {27, -1}, \
}

#define RGB_LED_PIN 23
#define RGB_LED_NUM 1
#define RGB_LOGO_LEDS {0}
#define RGB_RAINBOW_LEDS {0}