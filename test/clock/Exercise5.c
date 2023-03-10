/**
 ******************************************************************************
 * @file    Exercise5.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example of PLL Configuration via HSI and HSE
 ******************************************************************************
 * @attention
 *     - Write a program to output HSI clock on microcontroller pin and measure
 *it using oscilloscope or logic analyzer
 *     - 將HSI預設降頻作為練習(1MHz)，表示MCO要除以8
 */

#include <stdint.h>
#include <stdio.h>

#define RCC_CFGR \
    *(volatile uint32_t *) 0x40021004U  // Clock configuration register
#define RCC_AHBENR \
    *(volatile uint32_t *) 0x40021014U  // AHB peripheral clock enable register

#define GPIOA_MODER \
    *(volatile uint32_t *) 0x48000000U  // GPIO port mode register
#define GPIOA_OTYOER \
    *(volatile uint32_t *) 0x48000004U  // GPIO port output type register
#define GPIOA_PUPDR \
    *(volatile uint32_t *) 0x4800000CU  // GPIO port pull-up/pull-down register
#define GPIOA_AFRH \
    *(volatile uint32_t *) 0x48000024U  // GPIO alternate function high register

int main(void)
{
    RCC_CFGR |= (3 << 28);  // Microcontroller Clock Output Prescaler (MCO is
                            // divided by 8)
    RCC_CFGR |= (1 << 24) |
                (1 << 26);  // Microcontroller clock output (HSI clock selected)
    RCC_AHBENR |= (1 << 17);  // I/O port A clock enabled

    GPIOA_MODER |= (1 << 17);   // PA8 (Alternate function mode)
    GPIOA_OTYOER &= ~(1 << 8);  // Output push-pull (reset state)
    GPIOA_PUPDR &= ~(3 << 16);  // No pull-up, pull-down
    GPIOA_AFRH &= ~(15 << 0);   // PA8 AF0; MCO

    while (1)
        ;
    return 0;
}