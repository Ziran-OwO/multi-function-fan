#ifndef __KEY_H  //检查 __KEY_H 这个宏是否已经被定义。如果没有定义，则执行后续的代码块。
#define __KEY_H	 //如果 __KEY_H 未定义，就定义这个宏。防止头文件被重复包含，避免重复定义的错误
#include "sys.h"



//定义了一个宏PortD，由GPIO_Pin_0到GPIO_Pin_7按位或运算得到的。用于一次性配置GPIOD端口的多个引脚。
#define  PortD   (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

//分别定义了8个按键对应的宏KEY0到KEY7
//GPIO_ReadInputDataBit函数读取指定GPIO端口的指定引脚的输入电平。
//取GPIOD端口不同引脚的输入状态，以此判断对应的按键是否被按下。
#define KEY0  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define KEY3  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)
#define KEY5  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define KEY6  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)
#define KEY7  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
//#define KEY8  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)//

 
//定义了8个宏KEY0_PRES到KEY7_PRES，分别用不同的整数来表示每个按键被按下的状态。
#define KEY0_PRES 1	
#define KEY1_PRES 2	
#define KEY2_PRES 3	
#define KEY3_PRES 4	
#define KEY4_PRES 5	
#define KEY5_PRES 6	
#define KEY6_PRES 7	
#define KEY7_PRES 8	


void KEY_Init(void);  //对按键所连接的GPIO引脚进行初始化配置，设置引脚为输入模式
u8 keyscan(void);     //扫描按键的状态，返回当前被按下的按键对应的状态值
u8 keyhandle(void);   //处理按键事件，根据按键的状态执行相应的操作，并返回处理结果


#endif

