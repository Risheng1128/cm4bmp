/**
  ******************************************************************************
  * @file    SVC_number.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a setting SVC number example
  ******************************************************************************
  * @attention
  * - Goal: Write a program to execute an SVC instruction from thread mode, implement the SVC handler to print the SVC number used. 
  *         Also, increment the SVC number by 4 and return it to the thread mode code and print it.
  * 1. Write a main() function where you should execute the SVC instruction with an argument. Using SVC #0x05 in this case.
  * 2. Implement the SVC handler function
  * 3. In the SVC handler extract the SVC number and print it using printf
  * 4. Increment the SVC number by 4 and return it to the thread mode.
  */

#include <stdio.h>
#include "myusart.h"

void get_svc_number(uint32_t *msp) 
{
    uint8_t svc_number = ((uint8_t *)msp[6])[-2]; // get svc number
    printf("svc_number = %d\n", svc_number);
    
    // according to the AAPCS, the order of return register is r0, r1
    msp[0] = svc_number + 4; // SVC number + 4 and stored into r0 (msp[0])
}

int main(void)
{
    MYUSART_Init();
    __asm volatile("SVC #0x05"); // trigger SVC exception

    // return data method 1: Use register
    // register uint32_t data __asm("r0");
    
    // return data method 2: inline assembly
    uint8_t data;
    __asm volatile("MOV %0, r0": "=r"(data) ::);

    printf("new SVC number = %d\n", data);
    while(1);
    return 0;
}

__attribute__ ((naked)) void SVC_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // Store MSP value into r0
    // according to the AAPCS, the value of function parameter "msp" will be r0
    __asm volatile("B get_svc_number"); //Branch to get_svc_number
}

