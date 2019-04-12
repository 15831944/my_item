#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "KEYPAD4x4.h"
#include "NVIC.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,正点原子MiniSTM32开发板,主频72MHZ
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//CS		接PB12	//片选信号
//CLK	    接PB13	//SPI时钟信号
//SDI(DIN)	接PB15	//SPI总线数据信号
//=======================================液晶屏控制线接线==========================================//
//RS(D/C)   接PB1	//寄存器/数据选择信号(RS=0数据总线发送的是指令；RS=1数据总线发送的是像素数据)
/////////////////////////////////////////////////////////////////////////////////////////////////
//==================================如何精简到只需要3个IO=======================================//
//1.CS信号可以精简，不作SPI复用片选可将CS接地常低，节省1个IO
//2.LED背光控制信号可以接高电平3.3V背光常亮，节省1个IO
//3.RST复位信号可以接到单片机的复位端，利用系统上电复位，节省1个IO
//==================================如何切换横竖屏显示=======================================//
//打开lcd.h头文件，修改宏#define USE_HORIZONTAL 值为0使用竖屏模式.1,使用横屏模式
//===========================如何切换模拟SPI总线驱动和硬件SPI总线驱动=========================//
//打开lcd.h头文件，修改宏#define USE_HARDWARE_SPI  值为0使用模拟SPI总线.1,使用硬件SPI总线
**************************************************************************************************/	
int main(void)
{	
	u8 s,a=6,b,S;
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化
	LCD_Init();	   //液晶屏初始化
	LCD_Clear(BLACK); //清屏

	POINT_COLOR=WHITE; 
	
	INT_MARK=0;//标志位清0

	NVIC_Configuration();//设置中断优先级
	KEYPAD4x4_Init();//阵列键盘初始化
	KEYPAD4x4_INT_INIT();//阵列键盘的中断初始化
	LCD_DrawRectangle(0,0,128-1,128-1);	//画矩形 
	Show_Str(36,5,BLUE,YELLOW,"取件码",16,0);
		
//		Show_Str(5,25,RED,YELLOW,"温度     ℃",24,1);
//		LCD_ShowNum2412(5+48,25,RED,YELLOW,":32",24,1);
				
	LCD_DrawLine(5,80,20,80); 
	LCD_DrawLine(25,80,40,80); 
	LCD_DrawLine(45,80,60,80); 
	LCD_DrawLine(65,80,80,80); 
	LCD_DrawLine(85,80,100,80); 
	LCD_DrawLine(105,80,120,80);  
	
	while(1)
	{
		if(INT_MARK){ //中断标志位为1表示有按键中断
			INT_MARK=0;//标志位清0
			s=KEYPAD4x4_Read();//读出按键值
			if((s!=0)&&(s<=10)&&(s!=36)&&(a>=0)){ //如按键值不是0，也就是说有按键操作，则判断为真
			a--;
			if(s==10)s-=10;//因为没按键按下时s就是等于零，所以要输出0必须使用这种方法
			S=s;
			if(a==5){LCD_ShowNum(9,65,s,1,18);b+=s*100000;}
			if(a==4){LCD_ShowNum(29,65,s,1,18);b+=s*10000;}
			if(a==3){LCD_ShowNum(49,65,s,1,18);b+=s*1000;}
			if(a==2){LCD_ShowNum(69,65,s,1,18);b+=s*100;}
			if(a==1){LCD_ShowNum(89,65,s,1,18);b+=s*10;}
			if(a==0){LCD_ShowNum(109,65,s,1,18);b+=s;}
		}
			if(s!=0&&s==36&&a<=5){
			a++;
			if(a==6){LCD_ShowChar(7,67,BLACK,BLACK,0,25,0);b-=S*100000;}
			if(a==5){LCD_ShowChar(27,67,BLACK,BLACK,0,25,0);b-=S*10000;}
			if(a==4){LCD_ShowChar(47,67,BLACK,BLACK,0,25,0);b-=S*1000;}
			if(a==3){LCD_ShowChar(67,67,BLACK,BLACK,0,25,0);b-=S*100;}
			if(a==2){LCD_ShowChar(87,67,BLACK,BLACK,0,25,0);b-=S*10;}
			if(a==1){LCD_ShowChar(107,67,BLACK,BLACK,0,25,0);b-=S;}
		}
	}	
}
}
