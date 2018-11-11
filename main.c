/*
 * main.c
 */

#include "common.h"
#include "main.h"
/* 
 * -----------------------每天进步一点点(程序调试记录)-------------------------- 
 * 6.14 方向控制微分项会影响时序    
 * 6.20 电机死区调节,跑直道  
 * 6.21 err =(Direction->ActPoint -Direction->SetPoint) Seting 在后 
 * 6.22 Direction->Kp Ki 方向控制参数调节成正比例关系
 * 6.22 速度控制不能有回荡现象,Kp值小一点,下坡道顺利通过
 * 6.22 不加方向控制,给固定值让其转向,检测角度.速度控制,不能出现跑停跑停情况
 * 6.26 Oled 出现花屏是Cs端未接 
 * 6.28 转向控制偏差量除2可使转向圆滑 小s直穿、转弯侧滑是因为Kd给小了、Kp Kd成正比关系
 * 6.29 速度快时在特别急的弯道会出现减速现象  再次加速时电池太低导致停车
 * 7.04 标志位之位后首先清除标志位，收到意想不到结果
 * 7.09 小宇宙操轮子,擦赛道啊,漂移就这样解决了 
 * 7.16 高速3000 2、3效果好多
 * 7.16 脉冲计数高速不掉轮子
 */

void main(void)
{
  DisableInterrupts;
  LPLD_PLL_Setup(PLL_200);
  Struct_init();
  oled_init();
  cmos_init();
  NRF24L01_init();
  FTM_QUAD_init();
  LPLD_FTM0_PWM_Init(10000);
  /* 右电机  PTC1 */
  LPLD_FTM0_PWM_Open(0,0);
  /* 右电机  PTC2 */
  LPLD_FTM0_PWM_Open(1,0);
  /* 左电机  PTD6 */
  LPLD_FTM0_PWM_Open(6,0);
  /* 左电机  PTD7 */
  LPLD_FTM0_PWM_Open(7,0);
  /* 电机驱动开关 PTD10 */  
  LPLD_GPIO_Set_b(PTD,10,OUTPUT_L);  
  LPLD_UART_Init(UART0,115200);
  LPLD_ADC_Init(ADC0,MODE_12,CONV_SING);
  LPLD_PIT_Init(PIT0,1000,pit_isr);  
  set_irq_priority(88,0);
  set_irq_priority(68,1);  
  set_irq_priority(87,1);  
  ad_start();
  display8x16Str(0,8,"Welcome To Use");  
  display8x16Str(3,5,"Camera Balanced"); 
  display8x16Str(6,0,"Producter ZhaoYu"); 
  EnableInterrupts; 
  enable_irq(88);  
  while(1)
  {
    KeyCheck();
    if(send_phone)
    {
      if(direction_flag==1)
      {
        direction_flag=0; 
        if(chang_huang ==0) 
          Send_Phone_1();
        else
        {
          if(direction_reduce1 ==0)
          {
            Send_Phone_3();
            if(black_center ==1)
            {
              Black_Center1();              
            }
            else if(black_center ==2)
            {
              Black_Center1();
            }      
            else if(black_center ==3)
            {
              Black_Center1();
            } 
            else if(black_center ==4)
            {
              Black_Center2();
            }            
            else if(black_center ==5)
            {
              Black_Center3();
            }
          }
          else
          {
            Send_Phone_3();
            Black_Center1();
          }
        }
        disable_irq(88);  
      }  
      /*LPLD_UART_PutChar(UART0,0xFF);
      LPLD_UART_PutChar(UART0,0xFF);
      serial_send_image();  
      LPLD_UART_PutChar(UART0,0x01);
      serial_send_image(); 
      LPLD_UART_PutChar(UART0,0x01);*/
      
      send_phone =0; 
      PORTB_ISFR =0x0000009;     
      enable_irq(88);  
    }
  }
}
