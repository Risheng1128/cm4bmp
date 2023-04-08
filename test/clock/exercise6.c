/**
 * @file  exercise6.c
 * @brief This file is an example of PLL configuration via HSI and HSE
 * - Write a program to switch HSE as system clock and measure it
 * - Output frequency: 1MHz
 */

#include <stdio.h>
#include "clock.h"
#include "usart.h"

// GPIO port mode register
#define GPIOA_MODER *(volatile uint32_t *) 0x48000000U
// GPIO port output type register
#define GPIOA_OTYOER *(volatile uint32_t *) 0x48000004U
// GPIO port pull-up/pull-down register
#define GPIOA_PUPDR *(volatile uint32_t *) 0x4800000CU
// GPIO alternate function high register
#define GPIOA_AFRH *(volatile uint32_t *) 0x48000024U

int main(void)
{
    // HSE crystal oscillator bypass
    RCC_CR |= (1 << 18);
    // microcontroller clock output prescaler (MCO is divided by 8)
    RCC_CFGR |= (3 << 28);
    // microcontroller clock output (HSE clock selected)
    RCC_CFGR |= (1 << 25) | (1 << 26);
    // system clock switch (HSE selected as system clock)
    RCC_CFGR |= (1 << 0);
    // HSE clock enable
    RCC_CR |= (1 << 16);
    WAIT_HSE();

    // HSI clock disable
    RCC_CR &= ~(1 << 0);
    WAIT_HSI();

    // I/O port A clock enabled
    RCC_AHBENR |= (1 << 17);
    // PA8 (alternate function mode)
    GPIOA_MODER |= (1 << 17);
    // output push-pull (reset state)
    GPIOA_OTYOER &= ~(1 << 8);
    // no pull-up, pull-down
    GPIOA_PUPDR &= ~(3 << 16);
    // PA8 AF0; MCO
    GPIOA_AFRH &= ~(15 << 0);

    while (1)
        ;
    return 0;
}