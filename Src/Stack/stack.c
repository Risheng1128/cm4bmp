/**
  ******************************************************************************
  * @file    stack.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a example of stack point exercise
  ******************************************************************************
  * @attention
  * 	1. Thread Mode: 使用PSP
  *     2. Handler Mode: 固定為MSP
  *     3. STACK SPACE: 1KB
  *     4. MSP 和 PSP各用一半，MSP使用上半部，PSP使用下半部
  */

#include <stdio.h>
#include "myusart.h"

int add(int a, int b, int c, int d)
{
    return a + b + c + d;
}

// naked -> 減少函式體積(編譯器不產生保護和恢復的code, 需要使用者自行回覆)
__attribute__ ((naked)) void sp_to_psp(void) 
{
    __asm volatile(".equ SRAM_END, (0x20000000 + (64 * 1024))");
    __asm volatile(".equ PSP_START, (SRAM_END - 512)");
    __asm volatile("LDR R0, =PSP_START");   /* 初始化 R0 */
    __asm volatile("MSR PSP, R0");  /* Read R0 value and write it to the PSP */
    __asm volatile("MOV R0, #0x02"); 
    __asm volatile("MSR CONTROL, R0"); /* 寫入bit 1, 改用PSP */
    __asm volatile("BX LR"); /* Return */
}

void generate_exception(void) 
{
    __asm volatile("SVC #0x2");
}

int main(void)
{
    sp_to_psp();
    MYUSART_Init();
    int res = add(1, 4, 5, 6);
    printf("res = %d\n", res);
    generate_exception();
    while (1);
    return 0;
}

void SVC_Handler(void) 
{
    printf("exception!\n");
}