/**
 * @file  inline-asm.c
 * @brief This file is an example of inline assembly coding
 * - Example 1: Load 2 values from memory, add them and store it to memory
 * - Example 2: Read control register
 * - Example 3: Move data from a variable to another variable
 * - Example 4: Move the data pointed by pointer to variable
 */

#include <stdint.h>
#include <stdio.h>
#include "usart.h"

static void Example1(void)
{
    *(uint32_t *) 0x20001000 = 6;
    *(uint32_t *) 0x20001004 = 4;
    /* move 0x20001000 to R1 */
    __asm volatile("LDR R1,=#0x20001000");
    /* move 0x20001004 to R2 */
    __asm volatile("LDR R2,=#0x20001004");
    /* load the data loactes at 0x20001000 to R0 */
    __asm volatile("LDR R0,[R1]");
    /* load the data loactes at 0x20001004 to R1 */
    __asm volatile("LDR R1,[R2]");
    /* add R0 and R1, then move to R0 */
    __asm volatile("ADD R0,R0,R1");
    /* store the result into 0x20001004 */
    __asm volatile("STR R0,[R2]");
    /* move the result into R2 */
    __asm volatile("LDR R2,[R2]");
}

static void Example2(void)
{
    /* read the CONTROL register */
    uint32_t control_reg = 0xff;
    __asm volatile("MRS %0, CONTROL" : "=r"(control_reg));
    printf("control = %ld\n", control_reg);
}

static void Example3(void)
{
    uint32_t var1 = 10, var2 = 0;
    __asm volatile("MOV %0,%1" : "=r"(var2) : "r"(var1));
    printf("var1 = %ld\tvar2 = %ld\n", var1, var2);
}

static void Example4(void)
{
    uint32_t ptr1 = 0, *ptr2 = (uint32_t *) 0x20000008;
    *ptr2 = 10;
    /* p1 = *p2 */
    __asm volatile("LDR %0,[%1]" : "=r"(ptr1) : "r"(ptr2));
    printf("ptr1 = %ld\n", ptr1);
}

int main(void)
{
    USART_Init();
    Example1();
    Example2();
    Example3();
    Example4();
    return 0;
}
