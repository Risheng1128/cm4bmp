#pragma once
#include <stdint.h>

// clock control register
#define RCC_CR *(volatile uint32_t *) (0x40021000U)
// clock configuration register
#define RCC_CFGR *(volatile uint32_t *) (0x40021004U)
// flash access control register
#define FLASH_ACR *(volatile uint32_t *) (0x40022000U)

// wait PLL setting complete
#define WAIT_PLL()                             \
    do {                                       \
        while (!((RCC_CR & 0x02000000) >> 25)) \
            ;                                  \
    } while (0)

// wait HSE setting complete
#define WAIT_HSE()                          \
    do {                                    \
        while (!((RCC_CR & 0x20000) >> 17)) \
            ;                               \
    } while (0)

// wait HSI setting complete
#define WAIT_HSI()                  \
    do {                            \
        while ((RCC_CR & 0x2) >> 1) \
            ;                       \
    } while (0)

// wait system clock switch complete
#define WAIT_SYSTEM_CLOCK()                  \
    do {                                     \
        while (((RCC_CFGR & 0xc) >> 2) != 2) \
            ;                                \
    } while (0)
