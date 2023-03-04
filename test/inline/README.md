# Inline Assembly Coding
## Example `1`: Load 2 values from memory, add them and store it to memory by using inline assembly statement
```
地址0x20001000初始值: 6
地址0x20001004初始值: 4
```  
將地址 `0x20001000` 及 `0x20001004` 分別放入 `R1` 及 `R2`
![](https://i.imgur.com/7PWQ7iV.png)

將 `R1` 及 `R2` 對應地址的資料分別放進 `R0` 及 `R1`
![](https://i.imgur.com/Bp7hF0M.png)

將 `R0` 及 `R1` 資料相加後放進 `R0`
![](https://i.imgur.com/pweeB9E.png)

將 `R0` 存進 `R2` 的地址裡
![](https://i.imgur.com/0IVLbN3.png)

將地址 `0x20001004` 讀出來並存到 `R2`
![](https://i.imgur.com/1hqpqwq.png)

## Example `2`: 讀取 Control Register
`=`: 表示這個指令為寫入
```c
uint32_t control_reg;
__asm volatile("MRS %0, CONTROL" : "=r"(control_reg));
```  
  
## Example `3`: 把一個變數的值複製到另一個變數的數值
```c
uint32_t va1 = 10, var2;
__asm volatile("MOV %0,%1": "=r"(var2): "r"(var1));
```
  
## Example `4`: 把一個位址的值寫到另一個變數的數值
```c
uint32_t ptr1, *ptr2;
ptr2 = (uint32_t*)0x20000008;
__asm volatile("LDR %0,[%1]": "=r"(ptr1): "r"(ptr2)); //p1 = *p2
```
  
