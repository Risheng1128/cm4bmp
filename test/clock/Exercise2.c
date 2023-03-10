/**
 ******************************************************************************
 * @file    Exercise2.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example of PLL Configuration via HSI
 ******************************************************************************
 * @attention
 *     - Write an application to generate below HCLK freqencies using PLL (APB
 *和 AHB 的除頻都設為1)
 *       1. 20MHz
 *          - APB1 : 20MHz
 *          - APB2 : 20MHz
 *       2. 40MHz
 *          - APB1 : 20MHz
 *          - APB2 : 40MHz
 *       3. 60MHz
 *          - APB1 : 30MHz
 *          - APB2 : 60MHz
 *     - Use HSI as PLL's input source and repeat the exercise using HSI as
 *input source
 *     - 使用USART3傳輸資料到電腦做測試 (baudrate set up 38400)
 */

#include <stdio.h>
#include "myusart.h"

#define APB1_CLK_20M 20000000U
#define APB1_CLK_30M 30000000U

#define RCC_CR *(volatile uint32_t *) (0x40021000U)  // Clock control register
#define RCC_CFGR \
    *(volatile uint32_t *) (0x40021004U)  // Clock configuration register
#define FLASH_ACR \
    *(volatile uint32_t *) (0x40022000U)  // Flash access control register

void HCLK20M(void);
void HCLK40M(void);
void HCLK60M(void);

int main(void)
{
    // HCLK20M();
    // HCLK40M();
    HCLK60M();
    while (1) {
        printf("Hello world\n");
    }
    return 0;
}

void HCLK20M(void)
{
    /* step1: Enable required clock and wait until the clock is ready.
        If application needs PLL, then configure the PLL and enable it */
    RCC_CFGR &= ~(1 << 15) &
                ~(1 << 16);  // PLL entry clock source ( HSI/2 used as PREDIV1
                             // entry and PREDIV1 forced to div by 2. )
    RCC_CFGR |= (1 << 18) |
                (1 << 19);  // PLL multiplication factor (PLL input clock x 5)
    RCC_CR |= (1 << 24);    // PLL enable
    while (!((RCC_CR & 0x02000000) >> 25))
        ;  // 等待PLL開啟完成

    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application
     * requirements */
    /* APB1 APB2 AHB 都使用預設 */

    /* step3: Configure the flash latency */
    FLASH_ACR &=
        ~(1 << 0) &
        ~(1 << 1);  // Flash Latency (Zero wait state, if 0 < HCLK ≤ 24 MHz)

    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR |= (1 << 1);  // System clock switch (PLL selected as system clock)
    while (((RCC_CFGR & 0xc) >> 2) != 2)
        ;  // 等待System clock switch完成

    MYUSART_Init();
    USART3_BRR =
        APB1_CLK_20M / BAUDRATE_38400;  // 重新設定baudrate，覆寫預設結果
}

void HCLK40M(void)
{
    /* step1: Enable required clock and wait until the clock is ready.
        If application needs PLL, then configure the PLL and enable it */
    RCC_CFGR &= ~(1 << 15) &
                ~(1 << 16);  // PLL entry clock source ( HSI/2 used as PREDIV1
                             // entry and PREDIV1 forced to div by 2. )
    RCC_CFGR |= (1 << 21);   // PLL multiplication factor (PLL input clock x 10)
    RCC_CR |= (1 << 24);     // PLL enable
    while (!((RCC_CR & 0x02000000) >> 25))
        ;  // 等待PLL開啟完成

    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application
     * requirements */
    /* APB2 AHB 使用預設*/
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)

    /* step3: Configure the flash latency */
    FLASH_ACR |=
        (1 << 0);  // Flash Latency (Zero wait state, if 24 < HCLK ≤ 48 MHz)

    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR |= (1 << 1);  // System clock switch (PLL selected as system clock)
    while (((RCC_CFGR & 0x0c) >> 2) != 2)
        ;  // 等待System clock switch完成

    MYUSART_Init();
    USART3_BRR =
        APB1_CLK_20M / BAUDRATE_38400;  // 重新設定baudrate，覆寫預設結果
}

void HCLK60M(void)
{
    /* step1: Enable required clock and wait until the clock is ready.
        If application needs PLL, then configure the PLL and enable it */
    RCC_CFGR &= ~(1 << 15) &
                ~(1 << 16);  // PLL entry clock source ( HSI/2 used as PREDIV1
                             // entry and PREDIV1 forced to div by 2. )
    RCC_CFGR |= (1 << 18) | (1 << 20) |
                (1 << 21);  // PLL multiplication factor (PLL input clock x 15)
    RCC_CR |= (1 << 24);    // PLL enable
    while (!((RCC_CR & 0x02000000) >> 25))
        ;  // 等待PLL開啟完成

    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application
     * requirements */
    /* APB2 AHB 使用預設*/
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)

    /* step3: Configure the flash latency */
    FLASH_ACR |=
        (1 << 1);  // Flash Latency (Zero wait state, if 48 < HCLK ≤ 72 MHz)

    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR |= (1 << 1);  // System clock switch (PLL selected as system clock)
    while (((RCC_CFGR & 0x0c) >> 2) != 2)
        ;  // 等待System clock switch完成

    MYUSART_Init();
    USART3_BRR =
        APB1_CLK_30M / BAUDRATE_38400;  // 重新設定baudrate，覆寫預設結果
}
