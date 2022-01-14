# Bit Band
目的: 給定 bit band address 及 bit position ，並計算 bit band alias address ，再利用 bit band alias address 修改其值
- memory location: `0x20000200` ，初始值: `0xff`
- bit position: `7`

首先使用 bitwise method 的方法實作
![](https://i.imgur.com/2Jjxi3x.png)

再接著使用 bit band method
![](https://i.imgur.com/WVnp9jf.png)
