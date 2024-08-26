#pragma once


#include "stm32l476xx.h"

#define LED_1		0
#define LED_2		1
#define LED_3		2
#define LED_4		3
#define LED_5		4
#define LED_6		5
#define LED_7		6

void ws2812b_init(void);

void ws2812b_set_color(uint32_t led, uint8_t red, uint8_t green, uint8_t blue);

void ws2812b_update(void);

void ws2812b_wait(void);


