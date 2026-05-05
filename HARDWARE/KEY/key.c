///////////////////////////////////////////////////////////////////////////////
//矩阵按键驱动  
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "sys.h" 
#include "delay.h"

u16 keyz=0;

void KEY_Init(void)               //初始化矩阵键盘所连接的GPIO引脚
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;   //GPIO_InitStructure来配置GPIO引脚
 	
	//RCC_APB2PeriphClockCmd是一个用于使能或失能APB2总线上外设时钟的函数。
    //RCC_APB2Periph_GPIOC表示GPIOC端口的时钟。
    //ENABLE表示使能该时钟。在使用GPIO端口之前，必须先使能其对应的时钟，否则GPIO端口无法正常工作。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
    
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//KEY0-KEY3 矩阵键盘的行
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //引脚工作在输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //引脚的输出速度为50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);             //完成对GPIOC端口0-3号引脚的初始化配置
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY4-KEY7 矩阵键盘的列
	
	//引脚工作在上拉输入模式。引脚在没有外部信号输入时会保持高电平，当有按键按下时，对应的引脚会被拉低
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	           //完成对GPIOC端口4-7号引脚的初始化配置
	
	//GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
	

}



u8 keyscan(void)
{   
	uint8_t LIE,HANG,k,i=0;        //LIE用于存储列信息，HANG用于存储行信息，k用于存储最终的键码，i用于循环计数
	//向指定的GPIO端口写入数据,0xF0=1111 0000
	GPIO_Write(GPIOC, 0xF0);                            //D0-D3拉低,D4-D7拉高
	
	//GPIO_ReadInputData函数用于读取指定GPIO端口的输入数据
	//&0xF0是按位与操作，只保留高4位（列信息）
	if((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)          //有哪一列按键按下
	{
	  delay_ms(40);                                     //去抖
	   if((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)       //再次判断是否按下
	   {
		   LIE=GPIO_ReadInputData(GPIOC);                 //读取按键按下后得到的代码
		   HANG=LIE;                                      //将代码复制给行
		   LIE=~LIE;                                      //将键码取反，例如：按下某个键得到0111 0000，取反后得到1000 1111
		   LIE=LIE&0XF0;                                  //得到列1000 1111&1111 0000得到1000 0000,得到列数
		   
		   //逐次拉高行，若原来是第二行第二列，那么i=1时第二列的会从低电平变为高
			 for(i=0;i<4&&((HANG&0xF0)!=0xF0);i++)          //逐次将行拉高，判断列数中原来变低的位是否变高
		   {                                              //读到之前检测到为低的列变高则推出
		       GPIO_Write(GPIOC, (HANG&0xF0)|(0x01<<i));  //进行行扫描，逐次将行口线拉高，列保持为按下的状态
		       HANG=GPIO_ReadInputData(GPIOC);            //读取IO口，用以判断是否扫描到行坐标		   
		   }
		   HANG&=0x0F;                                    //将行值取出
		   k=LIE|HANG;                                    //行列相加则得到键码
			 
			 
			 
		   GPIO_Write(GPIOC, 0xF0);                       //D0-D3拉低,D4-D7拉高,此处用来将行状态初始化为未按下时的状态
	     while((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)  //判释放
		   {
		        delay_ms(40);                             //后延消抖。时间需要长一点
		   }
		   return k;                                      //返回键码
	   }
	}	
	return (0);                                         //无键按下，返回0
}


 u8 keyhandle(void)
{ 
	uint8_t key_1=0;
	
	keyz=keyscan();                                       
	if(keyz!=0)
		{ 
			switch(keyz)
			{
				case 0x11: key_1=1;break; 
				case 0x21: key_1=5;break; 
				case 0x41: key_1=9;break; 
				case 0x81: key_1=13;break; 
				case 0x12: key_1=2;break; 
				case 0x22: key_1=6;break; 
				case 0x42: key_1=10;break; 
				case 0x82: key_1=14;break;
				case 0x14: key_1=3;break;
				case 0x24: key_1=7;break;
                case 0x44: key_1=11;break;	
		        case 0x84: key_1=15;break;				
				case 0x18: key_1=4;break;
				case 0x28: key_1=8;break;
                case 0x48: key_1=12;break;	
		        case 0x88: key_1=16;break;		
				default:break;
			}
		
	}
	return key_1;
	
}




