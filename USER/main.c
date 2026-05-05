#include "delay.h"
#include "key.h"
#include "lcd.h"
#include "beep.h"
#include "adc.h"
#include "usart.h"
#include "led.h"
#include "sys.h"
#include "usmart.h"
#include "timer.h"
#include "dcmotor.h"
#include "rtc.h"
#include "string.h"


#define start_state 1
#define stop_state 2

uint16_t Num=0;
uint16_t count=0;
u8 current_state=start_state;

typedef enum {                        //时间格式
    TIME_FORMAT_12H,
    TIME_FORMAT_24H
} TimeFormat;
TimeFormat currentTimeFormat = TIME_FORMAT_24H;       //原来为24小时制

typedef struct 
{
	vu8 start_hour;
	vu8 start_min;
	vu8 start_sec;			
	 
	vu8 stop_hour;
	vu8 stop_min;
	vu8 stop_sec;	
}Set_Time;	

Set_Time currentsetTime;


void Display_Time(void);
void Display_Stopwatch(void);

void Adjust_Time(void){               //设置时间
	u8 hour, min, sec, key;
	RTC_Get();             //获取当前 RTC 时间
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;  
	LCD_ShowString(20,50,920,16,16,"Adjust time: 1:hour++,2:hour--,3:min++,4:min--");
	LCD_ShowString(20,70,420,16,16,"5:sec++,6:sec--,16:exit");
	while(1){
		key=keyhandle();
		switch (key)
		{
			case 1:       //上键，增加小时
				hour++;
				if (hour>=24)  hour=0;
				break;
			case 2:     //下键，减少小时
				if (hour==0)   hour = 23;
				else	hour--;
				break;
			case 3:      //左键，增加分钟
				min++;
				if (min>=60)  min=0;
				break;
			case 4:      //右键，减少分钟
				if (min==0)  min=59;
				else	min--;
				break;
			case 5:      //左键，增加分钟
				sec++;
				if (sec>=60)  sec=0;
				break;
			case 6:      //右键，减少分钟
				if (sec==0)  sec=59;
				else	sec--;
				break;
			case 16:      //退出这个模式
				RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,hour,min,sec);  // 设置新的RTC时间
			    return;
		}
		calendar.hour = hour;
        calendar.min = min;
        calendar.sec = sec;
        Display_Time();
		delay_ms(100);
	}
}


void Switch_Time_Format(void){            //切换时间格式
	if(currentTimeFormat==TIME_FORMAT_24H){
        currentTimeFormat=TIME_FORMAT_12H;
    } 
	else{
        currentTimeFormat=TIME_FORMAT_24H;
    }
}

void Control_Stopwatch(void){         //秒表
	TIM3_Int_Init(9999,7199);
	TIM_Cmd(TIM3, DISABLE); 
	u8 key;
	Num=0;
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE; 
	LCD_ShowString(20,50,550,16,16,"Control stopwatch: 1:start watch,2:stop watch");
	LCD_ShowString(20,70,420,16,16,"3:restart watch,16:exit");
	while(1){
		key=keyhandle();
		switch (key)
		{
			case 1:       //开始计数
				TIM_Cmd(TIM3, ENABLE); 
				current_state=start_state;
				break;
			case 2:     //暂停计数
				TIM_Cmd(TIM3, DISABLE); 
				current_state=stop_state;
				break;
			case 3:      //归零重新开始计数
				Num=0;
				TIM_Cmd(TIM3, ENABLE);
			    current_state=start_state;
				break;
			case 16:      //退出这个模式
				LCD_Clear(WHITE);
			    return;
		}
        Display_Stopwatch();
		delay_ms(100);
	}
}

void set_start_time(void){               //设置定时开启时间
	u8 hour, min, sec, key;
	RTC_Get();             
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE; 
	LCD_ShowString(20,50,420,16,16,"Set start time: 1:hour++,2:hour--,3:min++,4:min--");
	LCD_ShowString(20,70,420,16,16,"5:sec++,6:sec--,16:exit");
	LCD_ShowString(20,90,420,16,16,"set start time:");
	Display_Time();
	while(1){
		key=keyhandle();
		switch (key)
		{
			case 1:       
				hour++;
				if (hour>=24)  hour=0;
				break;
			case 2:     
				if (hour==0)   hour = 23;
				else	hour--;
				break;
			case 3:      
				min++;
				if (min>=60)  min=0;
				break;
			case 4:      
				if (min==0)  min=59;
				else	min--;
				break;
			case 5:      
				sec++;
				if (sec>=60) sec=0;
			    break;
			case 6:      
				if (sec==0)  sec=59;
				else	sec--;
				break;
			case 16:      
				return;
		}
	Display_Time();
	currentsetTime.start_hour = hour;
    currentsetTime.start_min = min;
    currentsetTime.start_sec = sec;
	LCD_ShowString(144,90,200,16,16,"  :  :  ");
    LCD_ShowNum(144,90,hour,2,16);									  
	LCD_ShowNum(168,90,min,2,16);									  
	LCD_ShowNum(192,90,sec,2,16);
	delay_ms(100);
	}
}

void set_stop_time(void){               //设置定时关闭时间
	u8 hour, min, sec, key;
    RTC_Get();             
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE; 
	LCD_ShowString(20,50,420,16,16,"Set stop time: 1:hour++,2:hour--,3:min++,4:min--");
	LCD_ShowString(20,70,420,16,16,"5:sec++,6:sec--,16:exit");
	LCD_ShowString(20,90,420,16,16,"set stop time:");
	Display_Time();
	while(1){
		key=keyhandle();
		switch (key)
		{
			case 1:       
				hour++;
				if (hour>=24)  hour=0;
				break;
			case 2:     
				if (hour==0)   hour = 23;
				else	hour--;
				break;
			case 3:      
				min++;
				if (min>=60)  min=0;
				break;
			case 4:      
				if (min==0)  min=59;
				else	min--;
				break;
			case 5:      
				sec++;
				if (sec>=60) sec=0;
			    break;
			case 6:      
				if (sec==0)  sec=59;
				else	sec--;
				break;
			case 16:      
				return;
		}
	Display_Time();
	currentsetTime.stop_hour = hour;
    currentsetTime.stop_min = min;
    currentsetTime.stop_sec = sec;
	LCD_ShowString(144,90,200,16,16,"  :  :  ");
    LCD_ShowNum(144,90,hour,2,16);									  
	LCD_ShowNum(168,90,min,2,16);									  
	LCD_ShowNum(192,90,sec,2,16);
	delay_ms(100);
	}
}

void Control_Speed(void){                //设置速度档位  
	u8 key_value;
	u16 autoreload = 719;   			// 10kHz:72MHz /(psc + 1)/(arr + 1)
	u16 prescaler = 0;  
	DCMOTOR_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;  
	LCD_ShowString(20,50,920,16,16,"Control speed: 1:fast speed,2:middle speed,3:slow speed");
	LCD_ShowString(20,70,420,16,16,"16:exit");
	LCD_ShowString(20,160,180,16,16,"speed:");
	Display_Time();
	while(1){
		Display_Time();
		key_value = keyhandle();
		if(key_value){
			switch(key_value){
				case 1:  				//快速档位
					TIM1_PWM_Init(autoreload, prescaler);
					TIM_SetCompare3(TIM1, (autoreload + 1)*10/100);
					LCD_ShowString(70,160,200,16,16,"60%");
					break;
				case 2: 				//中速档位
					TIM1_PWM_Init(autoreload, prescaler);
					TIM_SetCompare3(TIM1, (autoreload + 1)*40/100);
					LCD_ShowString(70,160,200,16,16,"40%");
					break;
				case 3:         		//低速档位
					TIM1_PWM_Init(autoreload, prescaler);
					TIM_SetCompare3(TIM1, (autoreload + 1)*60/100);
					LCD_ShowString(70,160,200,16,16,"10%");
					break;
				case 16:        		//按16退出
					return;
			}
		}
	}

}

void Adjust_Speed(void){                      //通过旋钮调速
	u8 key_value= keyhandle();
	u16 adc_val;
	u16 pwm_val;
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;  
	LCD_ShowString(20,50,920,16,16,"Adjust speed: turn knob to adjust speed,16:exit");
	DCMOTOR_Init();
	Display_Time();
    TIM1_PWM_Init(1000, 71); 
    while(1){
		Display_Time();
		key_value=keyhandle();
		if(key_value!=16){
			adc_val = Get_Adc_Average(ADC_Channel_8,10);  
			pwm_val = adc_val * 1000 / 4095;
			TIM_SetCompare3(TIM1,pwm_val);
		}
		else if(key_value==16){
			return;
		}
			
	}
}		

void simulate_mode1(void){                     //自然风模式的1档：慢快慢
	u16 autoreload = 719;  
    u16 prescaler = 0;
	u8 key_value;
	u8 speed;
	DCMOTOR_Init();
	Display_Time();
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;  
	LCD_ShowString(20,50,920,16,16,"Simulate mode 1: slow-fast-slow,16:exit");
	LCD_ShowString(20,160,200,16,16,"speed:");
	while(1){
		Display_Time();
		key_value =  keyhandle();
		if(key_value!=16){
			TIM1_PWM_Init(autoreload, prescaler);
			int j=60;
			for(;j>=10;j--){ 
				Display_Time();
				key_value =  keyhandle();
				if(key_value==16){
					return;
				}
				else{ 
					TIM_SetCompare3(TIM1, (autoreload + 1)*j/100);
					speed=100-j;
					LCD_ShowString(86,160,200,16,16,"%");
					LCD_ShowNum(70,160,speed,2,16);			
					delay_ms (200);
				}
			}
					
			for(;j<=60;j++){ 
				Display_Time();
				key_value =  keyhandle();
				if(key_value==16){
					return;
				}
				else{ 
					TIM_SetCompare3(TIM1, (autoreload + 1)*j/100);
					speed=100-j;
					LCD_ShowNum(70,160,speed,2,16);
					LCD_ShowString(86,160,200,16,16,"%");		
					delay_ms (200);
				}
			}
					
		}
		else if(key_value==16){
			return;
		}
	}
}

void simulate_mode2(void){                       //自然风模式的2档：保持一段时间快，再快变慢，保持一段时间慢，再慢变快
	u16 autoreload = 719;  
	u16 prescaler = 0;
	count=0;
	u8 key_value;
	u8 speed;
	Display_Time();
	DCMOTOR_Init();
	LCD_Clear(WHITE);
	POINT_COLOR=BLUE;  
	LCD_ShowString(20,50,920,16,16,"Simulate mode 2: fast-fast-slow-slow,16:exit");
	LCD_ShowString(20,160,200,16,16,"speed:");
	TIM2_Int_Init(9999,7199);
	TIM_Cmd(TIM2, DISABLE); 
	while(1){
		Display_Time();
		key_value =  keyhandle();
		if(key_value!=16){
			TIM1_PWM_Init(autoreload, prescaler);
			while(count!=5){
				Display_Time();
				TIM_Cmd(TIM2, ENABLE); 
				TIM_SetCompare3(TIM1, (autoreload + 1) *15/100);         	//快速转动,保持5s后变慢
				LCD_ShowString(70,160,200,16,16,"85%");		
				key_value =  keyhandle();
				if(key_value==16){
					return;
				}
			}                                          
			count=0;
			TIM_Cmd(TIM2, DISABLE);
			int i=15;
			for(;i<=65;i++){                                              //变慢
                Display_Time();                           
				TIM_SetCompare3(TIM1, (autoreload + 1) *i/100);
				key_value =  keyhandle();
				if(key_value==16){
					return;
				}
				else{ 
					TIM_SetCompare3(TIM1, (autoreload + 1)*i/100);
					speed=100-i;
					LCD_ShowNum(70,160,speed,2,16);
					LCD_ShowString(86,160,200,16,16,"%");		
					delay_ms (200);
				}
			}
			key_value =  keyhandle();
			if(key_value!=16){
				while(count!=5){
					Display_Time();
					TIM_Cmd(TIM2, ENABLE); 
					TIM_SetCompare3(TIM1, (autoreload + 1) *65/100);         	//慢速转动
					if(key_value==16){
						return;
					}
				}
				count=0;                                                   //保持5s后变慢
				TIM_Cmd(TIM2, DISABLE);
				for(;i>=15;i--){
					Display_Time();
					TIM_SetCompare3(TIM1, (autoreload + 1)*i/100);
					key_value =  keyhandle();
						if(key_value==16){
							return;
						}
						else{ 
							TIM_SetCompare3(TIM1, (autoreload + 1)*i/100);
							speed=100-i;
							LCD_ShowNum(70,160,speed,2,16);
							delay_ms (200);
						}
				}
			}
			else if(key_value==16){
				return;
			}
		}
			else if(key_value==16){
				return;
			}
	}
}
	 


void Display_Time(void){
	u8 hour, min, sec;
	char format[3];          
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	RTC_Get(); 
	LCD_ShowString(20,140,920,16,16,"Time:");
	LCD_ShowString(70,140,200,16,16,"  :  :  ");
	if(currentTimeFormat==TIME_FORMAT_24H){
		LCD_ShowNum(70,140,hour,2,16);									  
		LCD_ShowNum(94,140,min,2,16);									  
		LCD_ShowNum(118,140,sec,2,16);
		LCD_ShowString(142,140,200,16,16,(u8*)"    ");
    } 
	else{
		if(hour>12){
			strcpy(format, "PM");
			hour=hour%12;
		}
		else if(hour==12){
			strcpy(format, "PM");
		}
		else{ 
			strcpy(format, "AM");
		    if(hour==0){
				hour=12;
			}
		}
		LCD_ShowNum(70,140,hour,2,16);									  
		LCD_ShowNum(94,140,min,2,16);									  
		LCD_ShowNum(118,140,sec,2,16);
		LCD_ShowString(142,140,200,16,16,(u8*)format);
    }
}

void Display_Stopwatch(void){    //显示秒表结果
	LCD_ShowString(60,132,40,16,16,"Num:");
	LCD_ShowNum(110,132,Num,2,16);	
	LCD_ShowString(130,132,10,16,16,"s");
}


void Check_Timer(void){          //检查是否到达预设时间
	u16 autoreload = 719;  
	u16 prescaler = 0;
	u8 hour, min, sec,hour1,min1,sec1,hour2,min2,sec2;       
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	hour1=currentsetTime.start_hour;
	min1=currentsetTime.start_min;
	sec1=currentsetTime.start_sec;
	hour2=currentsetTime.stop_hour;
	min2=currentsetTime.stop_min;
	sec2=currentsetTime.stop_sec;
	if(hour==hour1&&min==min1&&sec==sec1){
		BEEP=0;
		LED0=0;
		DCMOTOR_Init();
		TIM1_PWM_Init(autoreload, prescaler);
		TIM_SetCompare3(TIM1,20);  
		delay_ms(100);
		BEEP=1;
		LED0=1;
	}
	else if(hour==hour2&&min==min2&&sec==sec2){
		BEEP=0;
		LED0=0;
		DCMOTOR_Init();
		TIM1_PWM_Init(autoreload, prescaler);
		TIM_SetCompare3(TIM1,1000);  
		delay_ms(100);
		BEEP=1;
		LED0=1;
	}
	else{
		BEEP=1;
		LED0=1;
	}
}


void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		count++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
		Num++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		}
}

int main(void){
	
	u16 autoreload = 719;  
	u16 prescaler = 0;
	
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);	 	
 	LED_Init();			     
	LED0=1;
	BEEP_Init();
	BEEP=1;
	DCMOTOR_Init();
	Adc_Init();
	LCD_Init();	
	usmart_dev.init(SystemCoreClock/1000000);	
	RTC_Init();	  			
	KEY_Init();
	RTC_Set(2025,4,5,20,22,20);
	POINT_COLOR=BLUE;//设置字体为蓝色  
	LCD_ShowString(60,112,200,16,16,"  :  :  ");
	LCD_ShowString(20,50,920,16,16,"Fan System: 1:Adjust time,2:Switch time format         ");
	LCD_ShowString(20,70,920,16,16,"3:Control watch,4:Set start time,5:Set stop time");
	LCD_ShowString(20,90,920,16,16,"6:Control speed,7:Adjust speed,8:simulate mode1  ");
	LCD_ShowString(20,110,920,16,16,"9:simulate mode2,10:stop the fan  ");
    while (1){
		u8 key = keyhandle();
		if (key){  
		   switch(key){
	        case 1:
				Adjust_Time();
			    break;
			case 2:
				Switch_Time_Format();
				break;
			case 3:
				Control_Stopwatch();
				break;
			case 4:
				set_start_time();
				break;
			case 5:
				set_stop_time();
				break;
			case 6:
				Control_Speed();
				break;
			case 7:
				Adjust_Speed();
				break;
			case 8:
				simulate_mode1();
				break;
			case 9:
				simulate_mode2();
				break;
			case 10:
				TIM_SetCompare3(TIM1,1000);      //风扇停止
				break;
		   }
		}
		LCD_ShowString(20,50,920,16,16,"Fan System: 1:Adjust time,2:Switch time format          ");
		LCD_ShowString(20,70,920,16,16,"3:Control watch,4:Set start time,5:Set stop time");
		LCD_ShowString(20,90,920,16,16,"6:Control speed,7:Adjust speed,8:simulate mode1  ");
		LCD_ShowString(20,110,920,16,16,"9:simulate mode2,10:stop the fan  ");
		Display_Time();
		Check_Timer();
		delay_ms(100);
    }
}


