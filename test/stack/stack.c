/**
 * @file  stack.c
 * @brief This file is an example of stack pointer configuration
 * - Thread mode: use PSP
 * - Handler mode: use MSP
 * - stack size: 1KB
 * - memory layout
 *   +-------------+ MSP_START (0x20010000)
 *   |  512 bytes  |
 *   +-------------+ MSP_END = PSP_START (0x2000fe00)
 *   |  512 bytes  |
 *   +-------------+ PSP_END (0x2000fc00)
 */

#include <stdio.h>
#include "usart.h"

__attribute__((naked)) void sp_to_psp(void)
{
    __asm volatile(".equ SRAM_END, (0x20000000 + (64 * 1024))");
    __asm volatile(".equ PSP_START, (SRAM_END - 512)");
    __asm volatile("LDR R0, =PSP_START");
    /* move R0 register value to the PSP register */
    __asm volatile("MSR PSP, R0");
    __asm volatile("MOV R0, #0x02");
    /* set bit[1] to switch to PSP */
    __asm volatile("MSR CONTROL, R0");
    /* return */
    __asm volatile("BX LR");
}

int add(int a, int b, int c, int d)
{
    return a + b + c + d;
}

void generate_exception(void)
{
    __asm volatile("SVC #0x2");
}

int main(void)
{
    sp_to_psp();
    USART_Init();
    /* check PSP is successful */
    printf("res = %d\n", add(1, 4, 5, 6));
    /* generate an exception to switch to MSP */
    generate_exception();
    return 0;
}

void SVC_Handler(void)
{
    printf("in handler mode\n");
}