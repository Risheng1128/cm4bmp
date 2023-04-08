/**
 * @file  exercise3.c
 * @brief This file is an example of PLL Configuration via HSE
 * - Write an application to generate below HCLK freqencies using PLL
 *   1. 20MHz
 *      - APB1 : 20MHz
 *      - APB2 : 20MHz
 *   2. 40MHz
 *      - APB1 : 20MHz
 *      - APB2 : 40MHz
 *   3. 60MHz
 *      - APB1 : 30MHz
 *      - APB2 : 60MHz
 * - Use HSE as PLL's input source and repeat the exercise
 * - Use USART to check clock setting success
 */

#include <stdio.h>
#include "clock.h"
#include "usart.h"

#define APB1_CLK_20M 20000000U
#define APB1_CLK_30M 30000000U

void HCLK20M(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    WAIT_HSE();

    // PLL entry clock source (HSE used as PREDIV1 entry)
    RCC_CFGR |= (1 << 16);
    // HSE divider for PLL input clock (HSE input to PLL divided by 2)
    RCC_CFGR |= (1 << 17);

    // PLL multiplication factor (PLL input clock x 5)
    RCC_CFGR |= (1 << 18) | (1 << 19);
    RCC_CR |= (1 << 24);  // PLL enable
    WAIT_PLL();

    /* step2: initialize AHB, APB busses clock prescalers to requirements
     * APB1, APB2 and AHB all use default setting
     */

    /* step3: configure the flash latency
     * flash latency (zero wait state, if 0 < HCLK ≤ 24 MHz)
     */
    FLASH_ACR &= ~(1 << 0) & ~(1 << 1);

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
    USART3_BRR = APB1_CLK_20M / BAUDRATE_38400;
    printf("HCLK 20MHz success\n");
}

void HCLK40M(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    WAIT_HSE();

    // PLL entry clock source (HSE used as PREDIV1 entry)
    RCC_CFGR |= (1 << 16);
    // HSE divider for PLL input clock (HSE input to PLL divided by 2)
    RCC_CFGR |= (1 << 17);
    // PLL multiplication factor (PLL input clock x 10)
    RCC_CFGR |= (1 << 21);
    RCC_CR |= (1 << 24);  // PLL enable
    WAIT_PLL();

    /* step2: initialize AHB, APB busses clock prescalers to requirements
     * APB2 and AHB use default setting
     */
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)

    /* step3: configure the flash latency
     * flash latency (zero wait state, if 24 < HCLK ≤ 48 MHz)
     */
    FLASH_ACR |= (1 << 0);

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
    USART3_BRR = APB1_CLK_20M / BAUDRATE_38400;
    printf("HCLK 40MHz success\n");
}

void HCLK60M(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    WAIT_HSE();

    // PLL entry clock source (HSE used as PREDIV1 entry)
    RCC_CFGR |= (1 << 16);
    // HSE divider for PLL input clock (HSE input to PLL divided by 2)
    RCC_CFGR |= (1 << 17);
    // PLL multiplication factor (PLL input clock x 15)
    RCC_CFGR |= (1 << 18) | (1 << 20) | (1 << 21);
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
    USART3_BRR = APB1_CLK_30M / BAUDRATE_38400;
    printf("HCLK 60MHz success\n");
}

int main(void)
{
    HCLK20M();
    // HCLK40M();
    // HCLK60M();
    return 0;
}
