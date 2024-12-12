/*
 * Pico Popn Controller Buttons
 * WHowe <github.com/whowechina>
 */

#ifndef RGB_H
#define RGB_H

#include <stdint.h>

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

void rgb_init();
void rgb_update_logo(uint8_t r, uint8_t g, uint8_t b);
void rgb_stimulate();
void rgb_set_brightness(uint8_t level);
void rgb_pause(bool pause);

#ifdef __cplusplus
}
#endif

#endif
