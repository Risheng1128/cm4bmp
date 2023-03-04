/**
  ******************************************************************************
  * @file    SVC_math.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a using SVC exception example
  ******************************************************************************
  * @attention
  * - 目標: Write a program to add, subtract, multiply, and divide 2 operands 
  *   using SVC handler and return the result to the thread mode code and print 
  *   the result. Thread mode code should pass 2 operands via the stack frame.
  * 
  * 1. Use the SVC number to decide the operation
  *    | Service number | operation      |
  *    | -------------- | -------------- |
  *    | 36             | Addition       |
  *    | 37             | Subtraction    |
  *    | 38             | Multiplication |
  *    | 39             | Division       |
  */

#include <stdio.h>
#include "myusart.h"

void calculate(uint32_t *msp, uint32_t a, uint32_t b) 
{
    // get svc nunmber
    uint8_t svc_number = (uint8_t)((uint8_t *)msp[6])[-2]; 
    
    switch (svc_number) {
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
    // store a into r0
    *msp = a;
}

int main(void) 
{
    MYUSART_Init();
    uint32_t a = 6, b = 4, res;
    // Addition
    __asm volatile("MOV r1, %0" :: "r"(a));
    __asm volatile("MOV r2, %0" :: "r"(b));
    __asm volatile("SVC #36"); // trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Addition res = %ld\n", res);
    // Subtraction
    __asm volatile("MOV r1, %0" :: "r"(a));
    __asm volatile("MOV r2, %0" :: "r"(b));
    __asm volatile("SVC #37"); // trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Subtraction res = %ld\n", res);
    // Multiplication
    __asm volatile("MOV r1, %0" :: "r"(a));
    __asm volatile("MOV r2, %0" :: "r"(b));
    __asm volatile("SVC #38"); // trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Multiplication res = %ld\n", res);
    // Division
    __asm volatile("MOV r1, %0" :: "r"(a));
    __asm volatile("MOV r2, %0" :: "r"(b));
    __asm volatile("SVC #39"); // trigger SVC exception
    __asm volatile("MOV %0, R0": "=r"(res) ::);
    printf("Division res = %ld\n", res);
    while(1);
    return 0;
}

__attribute__ ((naked)) void SVC_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // store the value of MSP into R0
    __asm volatile("B calculate"); //Branch to caculate
}