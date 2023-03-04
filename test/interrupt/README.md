# Interrupt Priority Configuration
- 目的: 使用 NVIC interrupt pending register 產生以下中斷並分別觀察 priorities 相同及不同時，ISRs 的執行情況
  > Interrupt 1: TIM2 global interrupt
  > Interrupt 2: I2C1 event interrupt

## 情況 `1`: 當兩個 interrupt 的優先度相同時
假設 `TIM2` 和 `I2C1` 優先度都為 `8`
1. 完成 interrupt 設定
   ![](https://i.imgur.com/RQnhbvR.png)
2. 進入 `TIM2` interrupt
   ![](https://i.imgur.com/anwhLCQ.png)
3. 將 `I2C` Priority Register enable
   ![](https://i.imgur.com/kCmzevM.png)
4. 因為 `I2C` 優先度和 `TIM2` 相等，因此不進 `I2C` 中斷函式，停在 `TIM2` 函式裡
   ![](https://i.imgur.com/9Sn1KXf.png)
5. 結果
   ```shell
   TIM2 Interrupt Handler
   ```

## 情況 `2`: 當兩個 interrupt 的優先度不同時
假設 `TIM2` 優先度為 `8` ， `I2C1` 優先度為 `7`
1. 完成 interrupt 設定
   ![](https://i.imgur.com/RQnhbvR.png)
2. 進入 `TIM2` interrupt
   ![](https://i.imgur.com/anwhLCQ.png)
3. 將 `I2C` Priority Register enable
   ![](https://i.imgur.com/kCmzevM.png)
4. 因為 `I2C` 優先度比 `TIM2` 高，因此跳進 `I2C` 中斷函式
   ![](https://i.imgur.com/zkuqGM5.png)
5. 結果 (在 Putty 上顯示)
   ```shell
   TIM2 Interrupt Handler
   IC21 Interrupt Handler
   ```
