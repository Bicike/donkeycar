/*
 * main.c
 */

#include "common.h"
#include "main.h"
/* 
 * -----------------------ÿ�����һ���(������Լ�¼)-------------------------- 
 * 6.14 �������΢�����Ӱ��ʱ��    
 * 6.20 �����������,��ֱ��  
 * 6.21 err =(Direction->ActPoint -Direction->SetPoint) Seting �ں� 
 * 6.22 Direction->Kp Ki ������Ʋ������ڳ���������ϵ
 * 6.22 �ٶȿ��Ʋ����лص�����,KpֵСһ��,���µ�˳��ͨ��
 * 6.22 ���ӷ������,���̶�ֵ����ת��,���Ƕ�.�ٶȿ���,���ܳ�����ͣ��ͣ���
 * 6.26 Oled ���ֻ�����Cs��δ�� 
 * 6.28 ת�����ƫ������2��ʹת��Բ�� Сsֱ����ת��໬����ΪKd��С�ˡ�Kp Kd�����ȹ�ϵ
 * 6.29 �ٶȿ�ʱ���ر𼱵��������ּ�������  �ٴμ���ʱ���̫�͵���ͣ��
 * 7.04 ��־λ֮λ�����������־λ���յ����벻�����
 * 7.09 С���������,��������,Ư�ƾ���������� 
 * 7.16 ����3000 2��3Ч���ö�
 * 7.16 ����������ٲ�������
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
  /* �ҵ��  PTC1 */
  LPLD_FTM0_PWM_Open(0,0);
  /* �ҵ��  PTC2 */
  LPLD_FTM0_PWM_Open(1,0);
  /* ����  PTD6 */
  LPLD_FTM0_PWM_Open(6,0);
  /* ����  PTD7 */
  LPLD_FTM0_PWM_Open(7,0);
  /* ����������� PTD10 */  
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
