/**
 ******************************************************************************
 * @file    Exercise1.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a simple clock config example
 ******************************************************************************
 * @attention
 *     1. Using HSE Configure the SYSCLK as 8MHz
 *     2. AHB clock as 4MHz (HCLK)
 *     3. APB1 clock as 2MHz (PCLK1)
 *     4. APB2 clock as 2MHz (PCLK2)
 */

#include <stdio.h>
#include "myusart.h"

#define APB1_CLK 2000000U                            // USART 連接到 APB1
#define RCC_CR *(volatile uint32_t *) (0x40021000U)  // Clock control register
#define RCC_CFGR \
    *(volatile uint32_t *) (0x40021004U)  // Clock configuration register
#define FLASH_ACR \
    *(volatile uint32_t *) (0x40022000U)  // Flash access control register

int main(void)
{
    /* step1: Enable required clock and wait until the clock is ready */
    RCC_CR |= (1 << 18);  // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);  // HSE clock enable
    while (!((RCC_CR & 0x20000) >> 17))
        ;  // 等待HSE準備完成
    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application
     * requirements */
    RCC_CFGR |= (1 << 13);  // APB2 high-speed prescaler (HCLK divided by 2)
    RCC_CFGR |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)
    RCC_CFGR |= (1 << 7);   // HLCK prescaler (SYSCLK divided by 2)

    /* step3: Configure the flash latency */
    FLASH_ACR &=
        ~(1 << 0) &
        ~(1 << 1);  // Flash Latency (Zero wait state, if 0 < HCLK ≤ 24 MHz)

    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR |= (1 << 0);  // System clock switch (HSE selected as system clock)

    /* step5: Disable other clock */
    RCC_CR &= ~(1 << 0);  //  HSI clock disable
    while ((RCC_CR & 0x2) >> 1)
        ;  // 等待HSI關閉完成

    MYUSART_Init();
    USART3_BRR = APB1_CLK / BAUDRATE_38400;  // 重新設定baudrate，覆寫預設結果
    while (1) {
        printf("Hello world\n");
    }
    return 0;
}
