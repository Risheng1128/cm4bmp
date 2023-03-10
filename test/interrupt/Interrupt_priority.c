/**
 ******************************************************************************
 * @file    Interrupt_priority.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example of interrupt priority configuration
 ******************************************************************************
 * @attention
 * 	Exercise: 使用 NVIC interrupt pending register 產生以下中斷並分別觀察
 *priorities 相同及不同時，ISRs 的執行情況 Interrupt 1: TIM2 global interrupt
 * 			  Interrupt 2: I2C1 event interrupt
 */

#include <stdint.h>
#include <stdio.h>
#include "myusart.h"

/* From RM0316 Reference manual p.290 */
#define IRQ_NO_TIM2 28    /* TIM2 global interrupt IRQ number */
#define IRQ_NO_I2C1_EV 31 /* I2C1 event interrupt IRQ number  */

/* From ARMv7-M Architecture Reference Manual p.B3-682 */
/* Some Macros for NVIC register */
#define NVIC_ISER0 \
    *(uint32_t *) 0xE000E100U /* Interrupt Set-Enable Registers */
#define NVIC_ISPR0 \
    *(uint32_t *) 0xE000E200U /* Interrupt Set-Pending Registers */
#define NVIC_IPR7 *(uint32_t *) 0xE000E41CU /* Interrupt Priority Registers */

int main(void)
{
    /* USART initialize */
    MYUSART_Init();
    /* step1: configure the priority for the peripherals */
    NVIC_IPR7 &= ~(0xFF0000FF << 0); /* clear */
    /* IPR low 4 bits are unimplement */
    /* (x << 4): Set TIM2 interrupt priority is x(0 ~ 15) */
    /* (x << 28): Set I2C1 interrupt priority is x(0 ~ 15) */
    NVIC_IPR7 |= (0x8 << 4) | (0x8 << 28);
    /* step2: Set the interrupt pending bit int the NVIC_PR */
    NVIC_ISPR0 |= (1 << IRQ_NO_TIM2);
    /* step3: Enable the IRQs in NVIC ISER */
    NVIC_ISER0 |= (1 << IRQ_NO_TIM2) | (1 << IRQ_NO_I2C1_EV);
    while (1)
        ;
    return 0;
}

/* From startup file, the interrupt function of TIM2 global interrupt */
void TIM2_IRQHandler(void)
{
    printf("TIM2 Interrupt Handler\n");
    NVIC_ISPR0 |= (1 << IRQ_NO_I2C1_EV);
    while (1)
        ;
}

/* From startup file, the interrupt function of I2C1 event interrupt */
void I2C1_EV_EXTI23_IRQHandler(void)
{
    printf("IC21 Interrupt Handler\n");
}
