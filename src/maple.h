/**
 * Pop'n'Music controller
 * Dreamcast Maple Bus Transiever example for Raspberry Pi Pico (RP2040)
 * (C) Charlie Cole 2021
 *
 * Dreamcast controller connector pin 1 (Data) to 10 (PICO_PIN1_PIN)
 * Dreamcast controller connector pin 5 (Data) to 11 (PICO_PIN5_PIN)
 * Dreamcast controller connector pin 2 (5V) to VSYS (I did via diode so could power from either USB/VBUS or Dreamcast safely)
 * Dreamcast controller connector pins 3 (GND) and 4 (Sense) to GND
 * GPIO pins for buttons 0-9,14-21 (uses internal pullups, switch to GND. See ButtonInfos)
 * LEDs on pins 0-9,14-21 (to white LED anode through resistor (say 82ohms) to GND. See ButtonInfos)
 * 
 * Maple TX done completely in PIO. Sends start of packet, data and end of packet. Fed by DMA so fire and forget.
 *
 * Maple RX done mostly in software on core 1. PIO just waits for transitions and shifts in whenever data pins change.
 * For maximum speed the RX state machine is implemented in lookup table to process 4 transitions at a time
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/flash.h"
#include "pico/multicore.h"
#include "maple.pio.h"
#include "state_machine.h"
#include "format.h"

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

uint CalcCRC(const uint* Words, uint NumWords);
void BuildACKPacket();
void BuildInfoPacket();
void BuildSubPeripheralInfoPacket();
void BuildMemoryInfoPacket();
void BuildControllerPacket();
void BuildBlockReadResponsePacket();
int SendPacket(const uint* Words, uint NumWords);
void SendControllerStatus();
void SendBlockReadResponsePacket();
void BlockRead(uint Address);
void BlockWrite(uint Address, uint* Data, uint NumWords);
void BlockCompleteWrite(uint Address);
bool ConsumePacket(uint Size);
void SetupButtons();
void SetupMapleTX();
void SetupMapleRX();
void ReadFlash();
void dreamcast_main();

#ifdef __cplusplus
}
#endif