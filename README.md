# STM32F4xx_Buzzer_Example

用PWM实现，STM32F4的有源蜂鸣器驱动。

## 开发环境

* 适用芯片：STM32F4全部芯片
* 固件库：STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
* IDE：MDK517

## API

* void Buzzer_SetState(BuzzerState state)
* BuzzerState Buzzer_GetState(void)

## 注意

修改接口需要注意，蜂鸣器的驱动方式和使用的TIM。对应修改PWM输出电平和TIM时钟配置函数。
