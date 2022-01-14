/**
  ******************************************************************************
  * @file    SVC_number.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a setting SVC number example
  ******************************************************************************
  *     @attention
  *         - 目標: Write a program to execute an SVC instruction from thread mode, implement the SVC handler to print the SVC number used. 
  *                 Also, increment the SVC number by 4 and return it to the thread mode code and print it.
  *         1. Write a main() function where you should execute the SVC instruction with an argument. Using SVC #0x05 in this case.
  *         2. Implement the SVC handler function
  *         3. In the SVC handler extract the SVC number and print it using printf
  *         4. Increment the SVC number by 4 and return it to the thread mode.
  */

#include <stdio.h>
#include "myusart.h"

void SVC_Get_Number(uint32_t* MSP_ptr) 
{
    uint32_t* RetAddr_ptr = (uint32_t*)*(MSP_ptr + 6); // Return Address
    RetAddr_ptr = (uint32_t*)((uint32_t)RetAddr_ptr - 2);
    printf("SVC_number = %ld\n", (*RetAddr_ptr & 0xff)); // SVC number為opcode的LSB(2 bit)
    // 根據AAPCS，回傳資料時順序為R0,R1
    *MSP_ptr = (*RetAddr_ptr & 0xff) + 4; // SVC number + 4，並存到r0(MSP_ptr[0])中
}

int main(void) 
{
    MYUSART_Init();
    __asm volatile("SVC #0x05"); // Trigger SVC exception
    // 回傳資料法一: register(不建議使用)
    //register uint32_t data __asm("r0"); // 將r0和暫存器data相關
    // 回傳資料法二: inline asm
    uint32_t data;
    __asm volatile("MOV %0, R0": "=r"(data) ::);

    printf("new SVC number = %ld\n", data);
    while(1);
    return 0;
}

__attribute__ ((naked)) void SVC_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // 把MSP的值存在MRS裡
    // 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
    __asm volatile("B SVC_Get_Number"); //Branch to SVC_Get_Number
}

