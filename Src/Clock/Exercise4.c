/**
  ******************************************************************************
  * @file    Exercise4.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a example of PLL Configuration via HSI and HSE
  ******************************************************************************
  * @attention
  *     - Write an application which does PLL configuration to boost the HCLK to maximum capacity
  *       1. APB1: 36MHz
  *       2. APB2: 72MHz
  *     - Use HSE and HSI as PLL Source respectively
  *     - 使用USART3傳輸資料到電腦做測試 (baudrate set up 38400)
  */

#include <stdio.h>
#include "myusart.h"

#define APB1_CLK_36M        36000000U

#define RCC_CR              *(volatile uint32_t*)(0x40021000U) // Clock control register
#define RCC_CFGR            *(volatile uint32_t*)(0x40021004U) // Clock configuration register
#define FLASH_ACR           *(volatile uint32_t*)(0x40022000U) // Flash access control register

void HSE(void); // Use HSE as PLL Source
void HSI(void); // Use HSI as PLL Source

int main(void)
{   
    HSE();
    //HSI();
    while(1) {
        printf("Hello world\n");
    }
    return 0;
}

void HSI(void)
{
    /* step1: Enable required clock and wait until the clock is ready. 
        If application needs PLL, then configure the PLL and enable it */
    RCC_CFGR |= (1 << 15); // PLL entry clock source (HSI used as PREDIV1 entry)
    RCC_CFGR |= (1 << 18) | (1 << 19) | (1 << 20); // PLL multiplication factor (PLL input clock x 9)
    RCC_CR   |= (1 << 24);   // PLL enable
    while(!((RCC_CR & 0x02000000) >> 25)); // 等待PLL開啟完成
    
    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application requirements */
    /* APB2 AHB 使用預設*/
    RCC_CFGR  |=  (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)
    
    /* step3: Configure the flash latency */
    FLASH_ACR |= (1 << 1);  // Flash Latency (Zero wait state, if 48 < HCLK ≤ 72 MHz)
    
    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR  |= (1 << 1);  // System clock switch (PLL selected as system clock)
    while(((RCC_CFGR & 0xc) >> 2) != 2); // 等待System clock switch完成
    
    MYUSART_Init();
    USART3_BRR = APB1_CLK_36M / BAUDRATE_38400; // 重新設定baudrate，覆寫預設結果
}

void HSE(void)
{
    /* step1: Enable required clock and wait until the clock is ready. 
        If application needs PLL, then configure the PLL and enable it */
    RCC_CR |= (1 << 18);   // HSE crystal oscillator bypass
    RCC_CR |= (1 << 16);   // HSE clock enable
    while(!((RCC_CR & 0x20000) >> 17)); // 等待HSE準備完成
    RCC_CFGR |=  (1 << 16); // PLL entry clock source (HSE used as PREDIV1 entry)
    RCC_CFGR &= ~(1 << 17); // HSE divider for PLL input clock (HSE input to PLL not divided)    
    RCC_CFGR |= (1 << 18) | (1 << 19) | (1 << 20); // PLL multiplication factor (PLL input clock x 9)
    RCC_CR   |= (1 << 24); // PLL enable
    while(!((RCC_CR & 0x02000000) >> 25)); // 等待PLL開啟完成
    
    /* step2: Initialize CPU, AHB, APB busses clock prescalers to application requirements */
    /* APB2 AHB 使用預設*/
    RCC_CFGR  |= (1 << 10);  // APB1 high-speed prescaler (HCLK divided by 2)
    
    /* step3: Configure the flash latency */
    FLASH_ACR |= (1 << 1);  // Flash Latency (Zero wait state, if 48 < HCLK ≤ 72 MHz)
    
    /* step4: Select newly enabled clock as SYSCLK */
    RCC_CFGR  |= (1 << 1);  // System clock switch (PLL selected as system clock)
    while(((RCC_CFGR & 0x0c) >> 2) != 2); // 等待System clock switch完成

    /* step5: Disable other clock */
    RCC_CR &= ~(1 << 0);    //  HSI clock disable
    while((RCC_CR & 0x2) >> 1); // 等待HSI關閉完成
    
    MYUSART_Init();
    USART3_BRR = APB1_CLK_36M / BAUDRATE_38400; // 重新設定baudrate，覆寫預設結果
}