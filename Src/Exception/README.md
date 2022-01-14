# Configurable fault exception
目標: Write a program to enable all configurable fault exceptions, implement the fault exception handlers and cause the fault by following method.
```
1. Execute an undefine instruction
2. Divide by zero
3. Try executing instruction from peripheral region
4. Executing SVC inside the SVC handler
5. Executing SVC instruction inside the interrupt handler whose priority whose priority is same or lesser than SVC handler
```

參考 Cortex-M4 Devices Generic User Guide Table 4-24 ， System Handler Control and State Register (`SHCSR`)
![](https://i.imgur.com/fyKGeGd.png)

### Example `1`: 執行未定義指令
首先在地址 `0x20000501` 放進未定義的指令 `0xFFFFFF`
![](https://i.imgur.com/nkNJXBe.png)

利用函數指標指到 `0x20000501` 並且執行，發現會進 Usage Fault
![](https://i.imgur.com/sPQB25d.png)

接著會發現有趣的現象，直接進到 `UsageFault_Handler` 裡
![](https://i.imgur.com/MUilZma.png)

接著開始分析 fault 產生的原因
1. 可以從 Fault Status and Fault Address Register 得知產生原因
   ![](https://i.imgur.com/JKnoouv.png)

2. 查看 UsageFault Status Register 的值可以知道發生什麼問題
   ![](https://i.imgur.com/Mnj8ir6.png)
   ![](https://i.imgur.com/9R8ssEA.png)
   ![](https://i.imgur.com/YE3LvjW.png)

3. 最後印出 UFSR 的值可以知道產生原因為 Undefined Instruction ，對照表格後可以看出為 `UNDEFINSTR` &rarr; Undefined instruction UsageFault
   ```shell
   In Usage fault
   UFSR = 1
   ```

4. 嘗試更改存放指令的地址為 `0x20000500` (`T` 位元為 `0`) ，對照表格後可以看出為 `INVSTATE` &rarr; Invalid state UsageFault
   ```shell
   In Usage fault
   UFSR = 2
   ```

開始分析 stack frame
![](https://i.imgur.com/LSCFpWI.png)

以下為輸出結果
```shell
In Usage fault
UFSR = 0x2
R0 = 0x20000000
R1 = 0x20000064
R2 = 0xffffffff
R3 = 0x20000500
R12 = 0x0
LR = 0x8000547
PC = 0x20000500
xPSR = 0x20000000
```

查看 memory view
![](https://i.imgur.com/YLIfElE.png)

繼續分析 `LR` 及 `PC`

首先是 `LR` 的部分，從上述的結果可以看到 `LR` 為 `0x8000547` ，查看組合語言可以發現指令如下表示
```shell
 8000540:	603b      	str	r3, [r7, #0]
 8000542:	683b      	ldr	r3, [r7, #0]
 8000544:	4798      	blx	r3
 8000546:	bf00      	nop
 8000548:	3708      	adds	r7, #8
```
可以得知 `nop` 為回傳之後要執行的指令

接著分析 `PC` ，從上述的結果可以看到 `PC` 為 `0x2000501` ，查看組合語言結果如下
&rarr; `0x2000501` 為 `0xFFFFFFFF` ，屬於未定義指令

### Example `2`: Divide by `0`
首先啟用 divide by 0 trap
1. 使用Configuration and Control Register (`CCR`)
   ![](https://i.imgur.com/x6rDduI.png)

2. 可利用 `CCR` 的 `bit[4]` 來trap divide by `0`
   ![](https://i.imgur.com/EWHvCD6.png)
   ![](https://i.imgur.com/gILKPqu.png)

以下為程式運行結束後，在終端機顯示的訊息
```shell
In Usage fault
UFSR = 0x200
R0 = 0x20000000
R1 = 0x20000064
R2 = 0x1
R3 = 0x0
R12 = 0x0
LR = 0x8000525
PC = 0x8000542
xPSR = 0x61000000
```

查看 `UFSR`(UsageFault Status Register) ，可以看出產生 Exception 的原因為除以 `0`
![](https://i.imgur.com/Vgi6fpt.png)

繼續分析 `LR` 及 `PC`

首先是 `LR` 的部分，從上述的結果可以看到 `LR` 為 `0x8000525` ，查看組合語言可以發現指令如下所示
```shell
 800051e:	6013      	str	r3, [r2, #0]
 8000520:	f000 f804 	bl	800052c <Example2>
 8000524:	e7fe      	b.n	8000524 <main+0x18>
 8000526:	bf00      	nop
 8000528:	e000ed24 	and	lr, r0, r4, lsr #26
```
執行結束後回傳的地址為 `b.n    8000524 <main+0x18>`

接著分析 `PC` ，從上面的結果可以看到 `PC` 為 `0x8000542` ，查看 assmembly 可以發現指令為以下表示:
```shell
 800053e:	2201      	movs	r2, #1
 8000540:	2300      	movs	r3, #0
 8000542:	fb92 f3f3 	sdiv	r3, r2, r3
 8000546:	607b      	str	r3, [r7, #4]
 8000548:	bf00      	nop
```
`sdiv	r3, r2, r3` 為進 Exception 前最後執行的指令，可以參考 [sdiv instruction](https://developer.arm.com/documentation/dui0473/m/arm-and-thumb-instructions/sdiv?lang=en)

最後嘗試把 trap 除以 0 的功能取消，分析會產生什麼變化，以下為程式執行的結果
```shell
In Hard fault
```
1. 查看 HardFault Status Register (`HFSR`)
   ![](https://i.imgur.com/zAKJvUX.png)
   ![](https://i.imgur.com/RQ8SB5S.png)
   ![](https://i.imgur.com/sZ0Fzlb.png)

2. 可以看到為 `bit[30]` 為 `1`

### Example `3`: 從 peripheral region 執行指令
Peripheral Region: 從下面的圖，可以看到外部設備的地址位於 `0x40000000` ~ `0x5FFFFFFF`
![](https://i.imgur.com/1ZbPknF.png)

以下為程式執行的結果
```shell
In Mem manage fault
MMSR = 0x1
R0 = 0x20000000
R1 = 0x20000064
R2 = 0xe000ed24
R3 = 0x40000000
R12 = 0x0
LR = 0x800053d
PC = 0x40000000
xPSR = 0x20000000
```

接著查看 `MMSR`
![](https://i.imgur.com/15bNQPH.png)
![](https://i.imgur.com/oMLtDwA.png)

查看後可以發現原因為 `bit[0]` &rarr; 對 XN region 進行 instruction fetch

繼續分析 `LR` 及 `PC`

首先是 `LR` 的部分，從上述的結果可以看到 `LR` 為 `0x800053d` ，查看 assmembly 可以發現指令為以下表示:
```shell
 8000536:	607b      	str	r3, [r7, #4]
 8000538:	687b      	ldr	r3, [r7, #4]
 800053a:	4798      	blx	r3
 800053c:	bf00      	nop
 800053e:	3708      	adds	r7, #8
```
`adds	r7, #8` 為異常回傳之後第一個執行的指令

接著是 `PC` 的部分，從上述的結果可以看到 `PC` 為 `0x40000000` ，表示 `PC` 已經跳到 `0x40000000` 要執行指令

### Example `4`: Executing `SVC` inside the `SVC` handler
以下為程式執行後，在終端機顯示的結果，很明顯得產生了 Hard Fault
```shell
In Hard fault
HFSR = 0x40000000
R0 = 0x20000000
R1 = 0x20000064
R2 = 0xe000ed24
R3 = 0x70000
R12 = 0x0
LR = 0xfffffff9
PC = 0x8000552
xPSR = 0x2100000b
```

繼續分析 `LR` 及 `PC`

首先是 `LR` 的部分，從上面的結果可以看到 `LR` 為 `0xfffffff9` ，為 `EXC` Return 值，表示返回 Thread mode 時使用 `MSP`

接著是 `PC` 的部分，從上面的結果可以看到 `PC` 為 `0x8000552` ，查看 assmembly 可以發現指令為以下表示:
```shell
 08000550 <SVC_Handler>:
 8000550:	df05      	svc	5
 8000552:	bf00      	nop
```
執行完 `SVC` 後準備執行 `nop` 就進 exception

### Example `5`: Executing `SVC` instruction inside the interrupt handler whose priority whose priority is same or lesser than `SVC` handler
使用 `I2C1_EV`(`I2C1` event interrupt & `EXTI` Line23 interrupt) 作範例
> IRQ number: 31
> Priority: 38

![](https://i.imgur.com/h3qzsXE.png)

`SVC` exception
> Priority: 3

![](https://i.imgur.com/BIvoM1q.png)

結果產生了 Hard Fault
```shell
In Hard fault
HFSR = 0x40000000
R0 = 0x20000000
R1 = 0x20000064
R2 = 0xe000e200
R3 = 0x80000000
R12 = 0x0
LR = 0xfffffff9
PC = 0x800058e
xPSR = 0x2100002f
```

繼續分析 `LR` 及 `PC`

首先是 `LR` 的部分，從上面的結果可以看到 `LR` 為 `0xfffffff9` ，為 `EXC` Return 值，表示返回 Thread mode 時使用 `MSP`

接著是 `PC` 的部分，從上面的結果可以看到 `PC` 為 `0x800058e` ，查看 assmembly 可以發現指令為以下表示
```shell
 0800058c <I2C1_EV_EXTI23_IRQHandler>:
 800058c:	df05      	svc	5
 800058e:	bf00      	nop
```
執行完 `SVC` 後準備執行 `nop` 就產生異常了
