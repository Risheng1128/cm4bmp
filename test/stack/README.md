# Stack
## Exercise `1`: 觀察 `SP`
從 Linker script 的檔案可以算出一開始 stack 的起始位置 (`_estack`)
> _estack = 0x20000000 + 64 * 1024 = 0x20010000

```shell
/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200; /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Memories definition */
MEMORY
{
  CCMRAM    (xrw)    : ORIGIN = 0x10000000,   LENGTH = 16K
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 64K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 512K
}
```

可以看到在 startup file ， `SP` 的確為 `0x20010000` (畫面左下角)，而 `MSP` 和 `SP` 值相同，表示 ARM 的預設為 `MSP` ，以下為 `MSP` 的數值
> `msp` 0x20010000

![](https://i.imgur.com/DgCJEI8.png)

## Exercise `2`: `SP` 的切換練習
1. Thread Mode: 使用 `PSP`
2. Handler Mode: 固定為 `MSP`
3. STACK SPACE: `1KB`
4. `MSP` 和 `PSP` 各用一半， `MSP` 使用上半部， `PSP` 使用下半部
   ![](https://i.imgur.com/SO8lSMv.png)

以下根據 Cortex™-M4 Devices Generic User Guide 的說明，利用 Control Register 可以從預設 `MSP` 設定為 `PSP` (利用位元 `1`)
![](https://i.imgur.com/iaGXo3J.png)

參考 [`MRS` and `MSR`](https://www.itread01.com/content/1542308852.html) ，可利用指令 `MRS` 和 `MSR` 分別讀取和寫入 `PSP`

接著可以開始進行實驗
1. 將利用暫存器 `R0` 將初始值 `PSP_START` 寫入 `PSP` 暫存器
   ![](https://i.imgur.com/Fb0jazt.png)

2. 寫入 Control Register 設定為採用 `PSP`
   ![](https://i.imgur.com/NDGnfkI.png)

3. 觀察函式呼叫是否使用 `PSP`
   進入函式之前
   ![](https://i.imgur.com/WygZi0l.png)
   進入函式
   ![](https://i.imgur.com/mljo3SB.png)
   可以很明顯的觀察到 `PSP` 的數值從 `0x2000fe00` 變成了 `0x2000fde8`

4. 觀察進入 Handler mode 時使用 `MSP` ，可以發現 `MSP` 的值從 `0x2000fff0` 變成了 `0x2000ffe8`
   ![](https://i.imgur.com/a3x3Sdk.png)

5. 回到 Thread mode 後繼續使用 `PSP`
   ![](https://i.imgur.com/v5djfEv.png)
