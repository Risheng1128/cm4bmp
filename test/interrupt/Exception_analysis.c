/**
 ******************************************************************************
 * @file    Exception_analysis.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example of exception entry and exit analyze.
 ******************************************************************************
 * @attention
 * 	Exercise: 1. 使用 RTC_WKUP
 *interrupt分析進入中斷後，觀察sp的變化及儲存之暫存為何?
 *             2. 分別使用MSP和PSP觀察EXC Return的差異
 */

#include <stdio.h>
#include "myusart.h"

#define IRQ_NO_RTC_WKUP 3
#define NVIC_ISER0 *(uint32_t *) 0xE000E100U
#define NVIC_STIR \
    *(uint32_t *) 0xE000EF00U  // Software Trigger Interrupt Register

int main(void)
{
    // use PSP (default is MSP)
    // int control_reg = 0x2;
    // uint32_t psp_value = 0x20008000; // PSP起始位址
    // __asm volatile("msr PSP, %0"::"r"(psp_value)); //
    // 將psp_valie寫進PSP暫存器
    // __asm volatile("msr CONTROL,%0"::"r"(control_reg)); // 設定CONTROL
    // reg使用PSP暫存器

    MYUSART_Init();
    printf("Now: In thread mode\n");

    // enable the RTC_WKUP interrupt
    NVIC_ISER0 |= (1 << IRQ_NO_RTC_WKUP);
    // generate a interrupt signal from software
    NVIC_STIR = (3 & 0xff);

    while (1)
        ;
    return 0;
}

void RTC_WKUP_IRQHandler(void)
{
    printf("Now: In handler mode\n");
}