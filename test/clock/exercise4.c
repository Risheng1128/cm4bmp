/**
 * @file  exercise4.c
 * @brief This file is an example of PLL configuration via HSI and HSE
 * - Write an application setting PLL to boost the HCLK to maximum capacity
 *   1. APB1: 36MHz
 *   2. APB2: 72MHz
 * - Use HSE and HSI as PLL source respectively
 * - Use USART to check clock setting success
 */

#include <stdio.h>
#include "clock.h"
#include "usart.h"

#define APB1_CLK_36M 36000000U

// use HSI as PLL source
void HSI(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    // PLL entry clock source (HSI used as PREDIV1 entry)
    RCC_CFGR |= (1 << 15);
    // PLL multiplication factor (PLL input clock x 9)
    RCC_CFGR |= (1 << 18) | (1 << 19) | (1 << 20);
    RCC_CR |= (1 << 24);  // PLL enable
    WAIT_PLL();

    /* step2: initialize AHB, APB busses clock prescalers to requirements
     * APB2 and AHB use default setting
     */
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)

    /* step3: configure the flash latency
     * flash latency (zero wait state, if 48 < HCLK ≤ 72 MHz)
     */
    FLASH_ACR |= (1 << 1);

    /* step4: select newly enabled clock as SYSCLK
     * system clock switch (PLL selected as system clock)
     */
    RCC_CFGR |= (1 << 1);
    WAIT_SYSTEM_CLOCK();

    USART_Init();
    // overwrite baudrate setting
    USART3_BRR = APB1_CLK_36M / BAUDRATE_38400;
    printf("HSI test success\n");
}

// use HSE as PLL source
void HSE(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    WAIT_HSE();

    // PLL entry clock source (HSE used as PREDIV1 entry)
    RCC_CFGR |= (1 << 16);
    // HSE divider for PLL input clock (HSE input to PLL not divided)
    RCC_CFGR &= ~(1 << 17);
    // PLL multiplication factor (PLL input clock x 9)
    RCC_CFGR |= (1 << 18) | (1 << 19) | (1 << 20);
    RCC_CR |= (1 << 24);  // PLL enable
    WAIT_PLL();

    /* step2: initialize AHB, APB busses clock prescalers to requirements
     * APB2 and AHB use default setting
     */
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)

    /* step3: configure the flash latency
     * flash latency (zero wait state, if 48 < HCLK ≤ 72 MHz)
     */
    FLASH_ACR |= (1 << 1);

    /* step4: select newly enabled clock as SYSCLK
     * system clock switch (PLL selected as system clock)
     */
    RCC_CFGR |= (1 << 1);
    WAIT_SYSTEM_CLOCK();

    /* step5: disable other clock */
    RCC_CR &= ~(1 << 0);  // HSI clock disable
    WAIT_HSI();

    USART_Init();
    // overwrite baudrate setting
    USART3_BRR = APB1_CLK_36M / BAUDRATE_38400;
    printf("HSE test success\n");
}

int main(void)
{
    HSE();
    // HSI();
    return 0;
}
