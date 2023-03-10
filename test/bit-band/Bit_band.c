/**
 ******************************************************************************
 * @file    Bit_band.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example of Bit Band
 ******************************************************************************
 * @attention
 * 	Example: 給定bit band address 及 bit position ，並計算 bit band alias
 *address，再利用 bit band alias address 修改其值 所選bit:
 *地址0x2000_0200的第7個bit
 */
#include <stdint.h>
#define ALIAS_BASE 0x22000000U
#define BIT_BAND_BASE 0x20000000U
#define BIT_POSITION 7

int main(void)
{
    uint8_t *ptr = (uint8_t *) 0x20000200;
    *ptr = 0xff;       /* reset value */
    *ptr &= ~(1 << 7); /* bitwise method */
    *ptr = 0xff;       /* reset value */
                       /* bit band method */
    uint8_t *alias_addr =
        (uint8_t *) (ALIAS_BASE + (32 * (0x20000200 - BIT_BAND_BASE)) +
                     BIT_POSITION * 4);
    *alias_addr = 0;
    return 0;
}
