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
    {16, 17}, \
    {18, 19}, \
    {20, 21}, \
    {26, -1}, \
    {27, -1}, \
	{-1, -1}, \
	{-1, -1}, \
}

#define RGB_LED_PIN 23
#define RGB_LED_NUM 10
#define RGB_LOGO_LEDS {0, 1, 2}
#define RGB_RAINBOW_LEDS {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

// The PIO PSX driver is opt-in.
// This uses the remaining PIO, pio1.
#define PSX_DRIVER pio
#define PSX_PIO pio1
// The PIO driver requires no additional parts and has a couple caveats:
// * Three of the pins, DAT (MISO), ATT (SS), and ACK, can be any pin.
// * CMD will be the pin specified on PSX_CMD_CLK_PINBASE.
// * CLK will be the next pin after CLK (PSX_CMD_CLK_PINBASE + 1).
#define PSX_CMD_CLK_PINBASE  12 // for CMD, pin 13 will be CLK
#define PSX_ATT_PIN          11
#define PSX_DAT_PIN          10
#define PSX_ACK_PIN          22