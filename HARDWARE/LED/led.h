#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//LED驱动代码	   
//PA15  只要出现 LED0，预处理器就会将其替换成PAout(15),对GPIOA端口的第15号引脚的输出值进行访问或者控制
#define LED0 PAout(15)
#define LED1 PEout(5)// PE5
void LED_Init(void);//初始化
#endif
