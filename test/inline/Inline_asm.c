/**
  ******************************************************************************
  * @file    Inline_asm.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a example of Inline Assembly Coding
  ******************************************************************************
  * @attention
  * 	Example 1: Load 2 values from memory, add them and storage it to memory by using inline assembly statement
  * 	Example 2: 讀取 "Control Register"
  * 	Example 3: 把一個變數複製到另一個變數
  * 	Example 4: 把一個位址的值寫到另一個變數裡
  */

#include <stdint.h>
void Example1();
void Example2();
void Example3();
void Example4();

int main(void) 
{
    Example1();
    //Example2();
    //Example3();
    //Example4();
	return 0;
}

void Example1() 
{
    *(uint32_t*)0x20001000 = 6;
    *(uint32_t*)0x20001004 = 4;
    __asm volatile("LDR R1,=#0x20001000");  /* 把地址0x20001000存到R1裡 */
    __asm volatile("LDR R2,=#0x20001004");  /* 把地址0x20001004存到R2裡 */
    __asm volatile("LDR R0,[R1]");          /* 把R1所存地址對應的數值存到R0裡 */
    __asm volatile("LDR R1,[R2]");          /* 把R2所存地址對應的數值存到R1裡 */
    __asm volatile("ADD R0,R0,R1");         /* 把R0及R1的數值加起來後存到R0裡 */
    __asm volatile("STR R0,[R2]");          /* 把R0的值存到R2所存位址對應的數值裡 */
    __asm volatile("LDR R2,[R2]");          /* 把R2所存位址對應的數值存到R2裡 */
}

void Example2() 
{
    uint32_t control_reg;
    __asm volatile("MRS %0, CONTROL" : "=r"(control_reg));
}

void Example3() 
{
    uint32_t var1 = 10, var2;
    __asm volatile("MOV %0,%1": "=r"(var2): "r"(var1));
}

void Example4() 
{
    uint32_t ptr1, *ptr2;
    ptr2 = (uint32_t*)0x20000008;
    __asm volatile("LDR %0,[%1]": "=r"(ptr1): "r"(ptr2)); /* p1 = *p2 */
}