/**
  ******************************************************************************
  * @file    myscheduler.c
  * @author  Ri-Sheng Chen
  * @brief   This file contains the functions of scheduler
  ******************************************************************************
  */

#include "myscheduler.h"
// 用來儲存每個task的PSP值
uint32_t task_PSP[TASK_NUM] = {TASK1_START_ADDR, TASK2_START_ADDR, TASK3_START_ADDR, TASK4_START_ADDR}; 
// 儲存每個task_handler的地址
uint32_t task_handler[TASK_NUM];

uint8_t current_task = 0; // 0表示正在執行task1

void SysTick_Handler(void) 
{
    /* Save the context of current task */
    // 1. 得到當前Task的PSP値
    __asm volatile("MRS r0, PSP");
    // 2. 使用PSP値儲存Stack Frame(R4 ~ R11)
    __asm volatile("STMDB r0!, {r4-r11}");
    // 3. 保存PSP的値
    __asm volatile("BL save_PSP_val");

    /* Retrieve the context of next task */
    // 1. Decide next task to run
    __asm volatile("BL update_next_task");
    // 2. get its past PSP value
    __asm volatile("MSR PSP, %0" ::"r"(*(task_handler + current_task)):); // 將TASK1的PSP值複製到PSP裡
    // 3. Using that PSP value retrieve Stack Frame (R4 ~ R11)
    __asm volatile("LDMIA r0!, {r4-r11}");
    // 4. Update PSP and exit
    __asm volatile("MSR PSP,R0");
}

void HardFault_Handler(void) 
{
    printf("In Hard Fault\n");
    while (1);
}

void MemManage_Handler(void) 
{
    printf("In MemMange Fault\n");
    while (1);
}

void BusFault_Handler(void) 
{
    printf("In Bus fault\n");
    while (1);
}

void UsageFault_Handler(void) 
{
    printf("In Usage fault\n");
    while (1);
}

void SysTick_Timer_Init(void) 
{
    uint32_t reload_val = DEFAULT_F_CLK / TICK_HZ - 1;
    SYST_RVR = reload_val; // set reload_val to SYST_RVR
    SYST_CSR |= (1 << 0) | (1 << 1) | (1 << 2); // enable ENABLE TICLINT CLKSOURCE
}

void Task_Stack_Init(void) 
{
    uint32_t* PSP_ptr;
    task_handler[0] = (uint32_t)task1_handler;
    task_handler[1] = (uint32_t)task2_handler;
    task_handler[2] = (uint32_t)task3_handler;
    task_handler[3] = (uint32_t)task4_handler;

    for(int i = 0; i < TASK_NUM; i++) {
        PSP_ptr = (uint32_t*) *(task_PSP + i);
        *(--PSP_ptr) = DUMMY_XPSR;       // xPSR = 0x01000000
        *(--PSP_ptr) = task_handler[i];  // PC = task_handler的地址
        *(--PSP_ptr) = DUMMY_LR;         // xPSR = 0x01000000
        for(int j = 0; j < 13; j++)      // R0 ~ R12
            *(--PSP_ptr) = 0;
        *(task_PSP + i) = (uint32_t)PSP_ptr;       // 將PSP的值存起來
    }
}

void Enable_processor_fault(void) 
{
    // 1. Enable all configurable exceptions (usage, mem manage, bus fault)
    // 18: Usage fault
    // 17: Bus fault
    // 16: Mem manage fault
    SHCRS |= (1 << USGFAULTENA) | (1 << BUSFAULTENA) | (1 << MEMFAULTENA);
}

__attribute__ ((naked)) void sp_to_psp(void) 
{
    // 1. Initialize the PSP with task1 stack start
    __asm volatile("MSR PSP, %0" ::"r"(*task_handler):); // 將TASK1的PSP值複製到PSP裡
    // 2. change SP to PSP using CONTROL register
    __asm volatile("MOV R0, #0x02"); 
    __asm volatile("MSR CONTROL, R0"); /* 寫入bit 1, 改用PSP */
    __asm volatile("BX LR"); /* Return */
}

__attribute__ ((naked)) void Schedueler_MSP_Init(uint32_t sched_top_of_stack)
{
    __asm volatile("MSR MSP, %0" :: "r"(sched_top_of_stack):);
    __asm volatile("Bx LR"); /* Return */
}

void save_PSP_val(uint32_t current_psp) 
{
    task_PSP[current_task] = current_psp;
}

void update_next_task(void) 
{
    current_task++;
    current_task = current_task % TASK_NUM;
}

void task1_handler(void) 
{
    while(1) {
        printf("In task1\n");
    }

}

void task2_handler(void) 
{
    while(1) {
        printf("In task2\n");
    }
}

void task3_handler(void) 
{
    while(1) {
        printf("In task3\n");
    }
}

void task4_handler(void) 
{
    while(1) {
        printf("In task4\n");
    }
}