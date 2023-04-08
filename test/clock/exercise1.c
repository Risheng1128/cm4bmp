/**
 * @file  exercise1.c
 * @brief This file is an example about clock configuration
 * - Use HSE to configure the SYSCLK 8MHz
 * - Set AHB clock 4MHz (HCLK)
 * - Set APB1 clock 2MHz (PCLK1)
 * - Set APB2 clock 2MHz (PCLK2)
 * - Use USART to check clock setting success
 */

#include <stdio.h>
#include "clock.h"
#include "usart.h"

// USART3 connect to APB1
#define APB1_CLK 2000000U

int main(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    WAIT_HSE();

    /* step2: initialize AHB, APB busses clock prescalers to requirements */
    RCC_CFGR |= (1 << 13);  // APB2 high-speed prescaler (HCLK divided by 2)
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)
    RCC_CFGR |= (1 << 7);   // HLCK prescaler (SYSCLK divided by 2)

    /* step3: configure the flash latency
     * flash latency (zero wait state, if 0 < HCLK ≤ 24 MHz)
     */
    FLASH_ACR &= ~(1 << 0) & ~(1 << 1);

    /* step4: select newly enabled clock as SYSCLK
     * system clock switch (HSE selected as system clock)
     */
    RCC_CFGR |= (1 << 0);

    /* step5: disable other clock */
    RCC_CR &= ~(1 << 0);  // HSI clock disable
    WAIT_HSI();

    USART_Init();
    // overwrite baudrate setting
    USART3_BRR = APB1_CLK / BAUDRATE_38400;
    printf("clock setting success\n");
    return 0;
}
