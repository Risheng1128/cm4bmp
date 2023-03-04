/**
  ******************************************************************************
  * @file    Exception_cfg.c
  * @author  Ri-Sheng Chen
  * @brief   This file is a simple exception example 
  ******************************************************************************
  * @attention
  *     目標: Write a program to enable all configurable fault exceptions, 
  *     implement the fault exception handlers and cause the fault by following method.
  *           1. Execute an undefine instruction
  *           2. Divide by zero
  *           3. Try executing instruction from peripheral region
  *           4. Executing SVC inside the SVC handler
  *           5. Executing SVC instruction inside the interrupt handler whose priority
  *              whose priority is same or lesser than SVC handler
  */

#include <stdio.h>
#include "myusart.h"

// define register
#define SHCRS         *(uint32_t*)0xE000ED24U // System Handler Control and State Register
#define UFSR          *(uint32_t*)0xE000ED2AU // UsageFault Status Register
#define HFSR          *(uint32_t*)0xE000ED2CU // HardFault Status Register
#define MMSR          *(uint32_t*)0xE000ED28U // MemManage Fault Status Register
#define BFSR          *(uint32_t*)0xE000ED29U // BusFault Status Register
#define CCR           *(uint32_t*)0xE000ED14U // Configuration and Control Register
#define NVIC_ISER0    *(uint32_t*)0xE000E100U // Interrupt Set-enable Registers
#define NVIC_ISPR0    *(uint32_t*)0xE000E200U // Interrupt Set-pending Registers
// Register bit configuration
#define USGFAULTENA         18
#define BUSFAULTENA         17
#define MEMFAULTENA         16
#define DIV_0_TRP           4
#define I2C1_EV_IRQ_NO      31

void Example1(void); // 執行未定義的指令
void Example2(void); // 除以0
void Example3(void); // 從 peripheral region 執行指令
__attribute__ ((naked)) void Example4(void); // Executing SVC inside the SVC handler
void Example5(void); // Executing SVC instruction inside the interrupt handler
void HardFault_Info(uint32_t* MSP_ptr);
void UsageFault_Info(uint32_t* MSP_ptr);
void MemManageFault_Info(uint32_t* MSP_ptr);
void Stack_Info(uint32_t* MSP_ptr);

int main(void) 
{
    MYUSART_Init();

    // 1. Enable all configurable exceptions (usage, mem manage, bus fault)
    // 18: Usage fault
    // 17: Bus fault
    // 16: Mem manage fault
    SHCRS |= (1 << USGFAULTENA) | (1 << BUSFAULTENA) | (1 << MEMFAULTENA);
    // SHCRS |= (1 << BUSFAULTENA) | (1 << MEMFAULTENA);
    
    // 3. Different example
    // Example1();
    // Example2();
    // Example3();
    // Example4();
    Example5();

    while(1);
    return 0;
}

void Example1(void) // 執行未定義的指令
{ 
    uint32_t* SRAM_ptr = (uint32_t*)0x20000501; // 指到SRAM隨便一個地方 (bit0要1，要對應T bit)
    //uint32_t* SRAM_ptr = (uint32_t*)0x20000500; // T bit為0
    *SRAM_ptr = 0xFFFFFFFF; // 無效指令
    void (*func_ptr)(void) = (void*)SRAM_ptr; // 函數指標指到該SRAM地址
    func_ptr(); // 執行函數
}

void Example2(void) // 除以0
{ 
    // enable divide by 0 trap
    CCR |= (1 << DIV_0_TRP);
    // divdie 0
    int a = 1 / 0;
}

void Example3(void) // 從 peripheral region 執行指令
{
    void (*func_ptr)(void) = (void*)0x40000000; // 0x40000000位於peripheral region
    func_ptr(); // 執行函數
}

__attribute__ ((naked)) void Example4(void) // Executing SVC inside the SVC handler
{ 
    __asm volatile("SVC #5");
}

void Example5(void) // Executing SVC instruction inside the interrupt handler whose priority whose priority is same or lesser than SVC handler
{     
    NVIC_ISER0 |= (1 << I2C1_EV_IRQ_NO);
    NVIC_ISPR0 |= (1 << I2C1_EV_IRQ_NO);
}

// 2. Implement the fault handler
__attribute__ ((naked)) void HardFault_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // 把MSP的值存在MRS裡
    // 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
    __asm volatile("B HardFault_Info"); //Branch to SVC_Get_Number
}

__attribute__ ((naked)) void MemManage_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // 把MSP的值存在MRS裡
    // 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
    __asm volatile("B MemManageFault_Info"); //Branch to SVC_Get_Number
}

void BusFault_Handler(void) 
{
    printf("In Bus fault\n");
    while (1);
}

__attribute__ ((naked)) void SVC_Handler(void) 
{
    __asm volatile("SVC #5"); // 使用Example    5時要關掉，不然會有問題
}

__attribute__ ((naked)) void UsageFault_Handler(void) 
{
    __asm volatile("MRS r0, MSP");  // 把MSP的值存在MRS裡
    // 根據AAPCS，ARM在函數傳遞引數時，順序為r0, r1, r2, r3，因此r0的值會傳到MSP_ptr
    __asm volatile("B UsageFault_Info"); //Branch to SVC_Get_Number
}

__attribute__ ((naked)) void I2C1_EV_EXTI23_IRQHandler(void) 
{
    __asm volatile("SVC #5");
}

void HardFault_Info(uint32_t* MSP_ptr) 
{
    printf("In Hard fault\n");
    printf("HFSR = %lx\n", HFSR);
    Stack_Info(MSP_ptr);
    while(1);
}

void UsageFault_Info(uint32_t* MSP_ptr) 
{
    printf("In Usage fault\n");
    printf("UFSR = 0x%lx\n", UFSR);           // 查看產生Fault原因
    Stack_Info(MSP_ptr);
    while (1);
}

void MemManageFault_Info(uint32_t* MSP_ptr) 
{
    printf("In Mem manage fault\n");
    printf("MMSR = 0x%lx\n", (MMSR & 0xff));           // 查看產生Fault原因
    Stack_Info(MSP_ptr);
    while (1);
}

void Stack_Info(uint32_t* MSP_ptr) 
{
    printf("r0 = 0x%lx\n", *MSP_ptr);         // r0
    printf("r1 = 0x%lx\n", *(MSP_ptr + 1));   // r1
    printf("r2 = 0x%lx\n", *(MSP_ptr + 2));   // r2 
    printf("r3 = 0x%lx\n", *(MSP_ptr + 3));   // r3 
    printf("r12 = 0x%lx\n", *(MSP_ptr + 4));  // r12 
    printf("LR = 0x%lx\n", *(MSP_ptr + 5));   // LR
    printf("PC = 0x%lx\n", *(MSP_ptr + 6));   // PC
    printf("xPSR = 0x%lx\n", *(MSP_ptr + 7)); // xPSR
}