# Clock Implementation
###### tags: `ARM Cortex-M` `ARM Practice`
STM32F303ZE Clock Tree

![](https://i.imgur.com/P1XjtyZ.png)

Flash latency &rarr; 根據 `HCLK` 不同，存取 flash 的 cycle 會不同
- Flash base address
  ![](https://i.imgur.com/ntSfkmY.png)

![](https://i.imgur.com/f9yqdBh.png)
![](https://i.imgur.com/oBm7jAx.png)

## Exercise `1`: `SYSTICK` configuration
1. Using `HSE` Configure the `SYSCLK` as `8MHz`
2. `AHB` clock as `4MHz` (`HCLK`)
3. `APB1` clock as `2MHz` (`PCLK1`)
4. `APB2` clock as `2MHz` (`PCLK2`)
5. 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

程式實作設定
1. `HSE` 預設為 `8Hz`
2. `HCLK` prescalar 設為 `2`
3. `APB1` Low-speed prescaler 設為 `2`
4. `APB2` Low-speed prescaler 設為 `2`

以下為程式結果，能夠印出資料表示時脈有設定成功
```shell
Hello world
Hello world
Hello world
Hello world
Hello world
...
```
  
## Exercise `2`: `PLL` configuration via `HSI`
1. Write an application to generate below `HCLK` freqencies using `PLL`
   - 20MHz
     - `APB1`: `20MHz`
     - `APB2`: `20MHz`
   - 40MHz
     - `APB1`: `20MHz`
     - `APB2`: `40MHz`
   - 60MHz
     - `APB1`: `30MHz`
     - `APB2`: `60MHz`
2. Use `PLL` as `PLL's` input source and repeat the exercise using `HSE` as input source
3. 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

以下為程式結果，能夠印出資料表示時脈有設定成功 (三者的結果都一樣)
```shell
Hello world
Hello world
Hello world
Hello world
Hello world
...
```
  
## Exercise `3`: `PLL` configuration via `HSE`
1. Write an application to generate below `HCLK` freqencies using `PLL` (`APB` 和 `AHB` 的除頻都設為 `1`)
   - 20MHz
     - `APB1`: `20MHz`
     - `APB2`: `20MHz`
   - 40MHz
     - `APB1`: `20MHz`
     - `APB2`: `40MHz`
   - 60MHz
     - `APB1`: `30MHz`
     - `APB2`: `60MHz`
2. Use `HSE` as `PLL's` input source and repeat the exercise using `HSE` as input source
3. 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

以下為程式結果，能夠印出資料表示時脈有設定成功 (三者的結果都一樣)
```shell
Hello world
Hello world
Hello world
Hello world
Hello world
...
```
  
## Exercise `4`: `PLL` configuration for `72MHz`
1. Write an application which does `PLL` configuration to boost the `HCLK` to maximum capacity
   - `APB1`: `36MHz`
   - `APB2`: `72MHz`
2. Use `HSE` and `HSI` as `PLL` Source respectively
3. 使用 `USART3` 傳輸資料到電腦做測試 (baudrate set up `38400`)

以下為程式結果，能夠印出資料表示時脈有設定成功 (兩者的結果一樣)
```shell
Hello world
Hello world
Hello world
Hello world
Hello world
...
```
  
## Exercise `5`: `HSI` measurment
1. Write a program to output `HSI` clock on microcontroller pin and measure it using oscilloscope or logic analyzer
2. 將 `HSI` 預設降頻作為練習 (`1MHz`) ，表示 `MCO` 要除以 `8`
  
參考 STM32F303ZE 的 alternate function mapping，其中 `MCO` 可以從 `PA8` 輸出(使用 `AF0`)

![](https://i.imgur.com/iwYAXMX.png)

以下為邏輯分析儀結果 (將 `MCO` 除以 `8` 後輸出，即 `1MHz`)

![](https://i.imgur.com/BjwlALt.png)
  
## Excercise `6`: `HSE` measurement
1. Write a program to switch `HSE` as system clock and measure it

以下為邏輯分析儀結果 (將 `MCO` 除以 `8` 後輸出，即 `1MHz`)
![](https://i.imgur.com/uY4tHXm.png)
