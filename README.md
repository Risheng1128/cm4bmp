# CM4 Practice
###### tags: `ARM Cortex-M`
根據[線上課程](https://www.udemy.com/course/embedded-system-programming-on-arm-cortex-m3m4/)所做的一些小練習，使用開發版為 NUCLEO-F303ZE

## 目前做過的練習
- Bit Band 操作
- Inline assembly 練習
- Stack pointer
- Interrupt 
- System Exception
- Clock Configure

## How to use
- 只要改 `TARGET` 的目標檔案路徑，接著輸入命令 `make` 即可
  ```makefile=
    # 目標編譯檔案
    TARGET = test/main
    # 編譯檔案資料夾
    BUILD_DIR = Debug
  ```

## 實作中
- Scheduler 練習
