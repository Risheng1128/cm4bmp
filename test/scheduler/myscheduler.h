/**
 ******************************************************************************
 * @file    myscheduler.h
 * @author  Ri-Sheng Chen
 * @brief   This file contains the definition of scheduler
 ******************************************************************************
 */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdio.h>
#include "myusart.h"

// stack memory calculate
#define TASK_STACK_SIZE 1024U     // 1KB
#define HANDLER_Stack_SIZE 1024U  // 1KB

#define SRAM_START_ADDR 0x20000000U
#define SRAM_SIZE ((64) * (1024))  // 64KB
#define SRAM_END_ADDR ((SRAM_START_ADDR) + (SRAM_SIZE))
// define User task
#define TASK1_START_ADDR SRAM_END_ADDR
#define TASK1_END_ADDR ((TASK1_START_ADDR) - (TASK_STACK_SIZE))
#define TASK2_START_ADDR TASK1_END_ADDR
#define TASK2_END_ADDR ((TASK2_START_ADDR) - (TASK_STACK_SIZE))
#define TASK3_START_ADDR TASK2_END_ADDR
#define TASK3_END_ADDR ((TASK3_START_ADDR) - (TASK_STACK_SIZE))
#define TASK4_START_ADDR TASK3_END_ADDR
#define TASK4_END_ADDR ((TASK4_START_ADDR) - (TASK_STACK_SIZE))
// define Handler
#define Handler_START_ADDR TASK4_END_ADDR
#define Handler_END_ADDR ((Handler_START_ADDR) - (TASK_STACK_SIZE))

// define register
#define SYST_CSR \
    *(uint32_t *) 0xE000E010U  // SysTick Control and Status Register
#define SYST_RVR *(uint32_t *) 0xE000E014U  // SysTick Reload Value Register
#define SYST_CVR *(uint32_t *) 0xE000E018U  // SysTick Current Value Register
#define SHCRS \
    *(uint32_t *) 0xE000ED24U  // System Handler Control and State Register

// 一些設定的macros
#define USGFAULTENA 18
#define BUSFAULTENA 17
#define MEMFAULTENA 16
#define TICK_HZ 1000U  // 計時中斷頻率1000Hz
#define TASK_NUM 4     // 任務總數
#define DUMMY_XPSR 0x01000000U
#define DUMMY_LR 0xFFFFFFFDU

void SysTick_Timer_Init(void);                // SysTick Timer Initial
void Task_Stack_Init(void);                   // 實作dummy task stack pointer
void Enable_processor_fault(void);            // system exception致能
__attribute__((naked)) void sp_to_psp(void);  // 從MSP切換到PSP
__attribute__((naked)) void Schedueler_MSP_Init(
    uint32_t sched_top_of_stack);         // Schedueler Stack Pointer Initial;
void save_PSP_val(uint32_t current_psp);  // 保存目前PSP的値
void update_next_task(void);              // 切換到下個task
void task1_handler(void);                 // Task 1
void task2_handler(void);                 // Task 2
void task3_handler(void);                 // Task 3
void task4_handler(void);                 // Task 4
#endif