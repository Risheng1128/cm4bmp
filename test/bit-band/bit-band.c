/**
 * @file  bit-band.c
 * @brief This file is a example of bit band
 * - Give the specific bit band address and bit position, modify the
 *   alias address via bitwise method and bit band.
 * - target: address 0x20000200 bit[7]
 */
#include <stdint.h>
/* SRAM bit-band alias base address */
#define ALIAS_BASE 0x22000000U
/* SRAM bit-band base address */
#define BIT_BAND_BASE 0x20000000U
/* target address should be modified */
#define TARGET_ADDR 0x20000200U
#define BIT_POSITION 7

int main(void)
{
    uint8_t *ptr = (uint8_t *) TARGET_ADDR;
    /* reset value */
    *ptr = 0xff;
    /* bitwise operation */
    *ptr &= ~(1 << 7);
    /* reset value */
    *ptr = 0xff;
    /* bit band method */
    uint8_t *alias_addr =
        (uint8_t *) (ALIAS_BASE + (32 * (0x20000200 - BIT_BAND_BASE)) +
                     BIT_POSITION * 4);
    *alias_addr = 0;
    return 0;
}
