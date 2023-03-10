/**
 ******************************************************************************
 * @file    main.c
 * @author  Ri-Sheng Chen
 * @brief   This file implements a simple tasks scheduler example.
 ******************************************************************************
 * @attention
 * 	 1. Implement a schedular which schedules multiple user tasks in a
 *round-robin fashion by carring out the context switch operation
 *      2. Round robin scheduling method is, time slices are assigned to each
 *task in equal portions and in circular order
 *      3. First will use systick handler to carry out the context switch
 *operation between multiple tasks
 *      4. Later will we change the code using PendSV handler
 */
#include "myscheduler.h"
int main(void)
{
    MYUSART_Init();
    Enable_processor_fault();                 // Enable System Exception
    Schedueler_MSP_Init(Handler_START_ADDR);  // MSP initial
    Task_Stack_Init();                        // Task Stack Space Initial
    SysTick_Timer_Init();                     // Timer initial
    sp_to_psp();                              // 換成使用PSP
    // task1_handler();      // Call Task1

    while (1)
        ;
    return 0;
}