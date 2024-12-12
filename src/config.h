/*
 * Pico Popn Controller Config
 * WHowe <github.com/whowechina>
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

void config_init();
void config_loop();

void *config_alloc(size_t size, void *def, void (*after_load)());
void config_request_save();

#ifdef __cplusplus
}
#endif

#endif
