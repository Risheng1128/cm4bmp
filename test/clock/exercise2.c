/**
 * @file  exercise2.c
 * @brief This file is an example of PLL configuration via HSI
 * - Write an application to generate below HCLK freqencies using PLL
 *   1. 20MHz
 *      - APB1: 20MHz
 *      - APB2: 20MHz
 *   2. 40MHz
 *      - APB1: 20MHz
 *      - APB2: 40MHz
 *   3. 60MHz
 *      - APB1: 30MHz
 *      - APB2: 60MHz
 * - Use HSI as PLL's input source and repeat the exercise
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
    /* PLL entry clock source
     * (HSI/2 used as PREDIV1 entry and PREDIV1 forced to div by 2)
     */
    RCC_CFGR &= ~(1 << 15) & ~(1 << 16);
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

    USART_Init();
    // overwrite baudrate setting
    USART3_BRR = APB1_CLK_20M / BAUDRATE_38400;
    printf("HCLK 20MHz success\n");
}

void HCLK40M(void)
{
    /* step1: enable required clock and wait until the clock is ready */
    /* PLL entry clock source
     * (HSI/2 used as PREDIV1 entry and PREDIV1 forced to div by 2)
     */
    RCC_CFGR &= ~(1 << 15) & ~(1 << 16);
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

    USART_Init();
    // overwrite baudrate setting
    USART3_BRR = APB1_CLK_20M / BAUDRATE_38400;
    printf("HCLK 40MHz success\n");
}

void HCLK60M(void)
{
    /* step1: Enable required clock and wait until the clock is ready */
    /* PLL entry clock source
     * (HSI/2 used as PREDIV1 entry and PREDIV1 forced to div by 2)
     */
    RCC_CFGR &= ~(1 << 15) & ~(1 << 16);
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
