/**
  ******************************************************************************
  * @file    SVC_math.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a using SVC exception example
  ******************************************************************************
  *     @attention
  *         - 目標: Write a program to add, subtract, multiply, and divide 2 operands using SVC handler and return the result to the thread mode code and print the result. Thread mode code should pass 2 operands via the stack frame.
  *           1. Use the SVC number to decide the operation
  *              | Service number | operation      |
  *              | -------------- | -------------- |
  *              | 36             | Addition       |
  *              | 37             | Subtraction    |
  *              | 38             | Multiplication |
  *              | 39             | Division       |
  */

#include <stdio.h>
#include "myusart.h"

void Calculate(uint32_t* MSP_ptr, uint32_t a, uint32_t b) 
{
    uint32_t* RetAddr_ptr = (uint32_t*)*(MSP_ptr + 6); // Return Address
    RetAddr_ptr = (uint32_t*)((uint32_t)RetAddr_ptr - 2); // SVC instruction地址為進入SVC instruction -2 的地址
    // 根據AAPCS，回傳資料時順序為R0,R1
    switch (*RetAddr_ptr & 0xff) {
    case 36:
        a += b;
        break;
    case 37:
        a -= b;
        break;
    case 38:
        a *= b;
        break;
    case 39:
        a /= b;
        break;
    }
    *MSP_ptr = a; // 計算結果存到r0(MSP_ptr[0])中
}

int main(void) 
{
    MYUSART_Init();
    uint32_t a = 6, b = 4, res;
    // Addition
    __asm volatile("MOV r1, %0" :: "r"(a) ); // 將資料從a傳進r1 register
    __asm volatile("MOV r2, %0" :: "r"(b) ); // 將資料從b傳進r2 register
    __asm volatile("SVC #36"); // Trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Addition res = %ld\n", res);
    // Subtraction
    __asm volatile("MOV r1, %0" :: "r"(a) ); // 將資料從a傳進r1 register
    __asm volatile("MOV r2, %0" :: "r"(b) ); // 將資料從b傳進r2 register
    __asm volatile("SVC #37"); // Trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Subtraction res = %ld\n", res);
    // Multiplication
    __asm volatile("MOV r1, %0" :: "r"(a) ); // 將資料從a傳進r1 register
    __asm volatile("MOV r2, %0" :: "r"(b) ); // 將資料從b傳進r2 register
    __asm volatile("SVC #38"); // Trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Multiplication res = %ld\n", res);
    // Division
    __asm volatile("MOV r1, %0" :: "r"(a) ); // 將資料從a傳進r1 register
    __asm volatile("MOV r2, %0" :: "r"(b) ); // 將資料從b傳進r2 register
    __asm volatile("SVC #39"); // Trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::); // 回傳資料: inline asm
    printf("Division res = %ld\n", res);
    while(1);
    return 0;
}

__attribute__ ((naked)) void SVC_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // 把MSP的值存在MRS裡
    // 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
    __asm volatile("B Calculate"); //Branch to SVC_Get_Number
}