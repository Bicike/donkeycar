/*
 * CMOS.h 图像处理
 */

#ifndef __IMAGE_PROCESS_4_H__
#define __IMAGE_PROCESS_4_H__ 

#include "imageprocess_3.h"

void serial_send_image()
{
  unsigned char i,j;
  for(i=0;i<89;i++)
  {
    Pix_Data[89-i][79] =0x00;
    j =Black_zuo[i];
    Pix_Data[89-i][j] =0x00;/**/
    j =Black_you[i];
    Pix_Data[89-i][j] =0x00;
    j =Black_zhongxin[i];
    Pix_Data[89-i][j] =0x00;    
  }
  for(i=0;i<90;i++)
    for(j=0;j<160;j++)
    {
      if(Pix_Data[i][j] ==0x01){Pix_Data[i][j] =0x02;} 
      //if(Pix_Data[i][j] ==0xff){Pix_Data[i][j] =0xfe;}                          //加一个判断语句，如果当前要发送的图像数据跟帧头0x01一样，改发另一个数比如0x02
      LPLD_UART_PutChar(UART0,Pix_Data[i][j]);
    }
}
/*************************************中线求取*********************************/
/* 高速 */
void Black_Center1()
{
  uint8_t i;
  for(i =89;i> (89-qianzhan);i --)
  {
    Black_zhongxin[i] =(Black_zuo[i] +Black_you[i])/2; 
    Direction->ActPoint =(Black_zuo[i] +Black_you[i])/2; 
  }
}
/* 中速 */
void Black_Center2()
{
  uint8_t i;
  for(i =79;i> (89-qianzhan);i --)
  {
    Black_zhongxin[i] =(Black_zuo[i] +Black_you[i])/2; 
    Direction->ActPoint =(Black_zuo[i] +Black_you[i])/2; 
  }
}
/* 低速 */
void Black_Center3()
{
  uint8_t i;
  for(i =59;i> (89-qianzhan);i --)
  {
    Black_zhongxin[i] =(Black_zuo[i] +Black_you[i])/2; 
    Direction->ActPoint =(Black_zuo[i] +Black_you[i])/2; 
  }
}
void Black_Center4()
{
  uint8_t i;
  for(i =9;i> (89-qianzhan);i --)
  {
    Black_zhongxin[i] =(Black_zuo[i] +Black_you[i])/2; 
    //Direction->ActPoint =(Black_zuo[i] +Black_you[i])/2; 
  }
}
void Black_Center5()
{
  uint8_t i;
  for(i =87;i>82;i --)
  {
    Black_zhongxin[i] =(Black_zuo[i] +Black_you[i])/2; 
    //Direction->ActPoint =(Black_zuo[i] +Black_you[i])/2; 
  }
}

void Send_Phone_3()
{
  Send_Phone_2();
  if(fengbi_flag ==1)
  {
    fengbi_flag =0;
    Find_zuo_you_jiezhi();
  }
  else
  {
    Find_Duandian();
    if(duandian_flag ==1)
    {
      duandian_flag =0;
      //LPLD_GPIO_Toggle_b(PTE,0);
      Renzi_Start();
      if((zuo_biaozhi==1)|(you_biaozhi==1))
      {
        if((zuo_biaozhi==1)&&(you_biaozhi==1));
        else
        {
          if(zuo_biaozhi==1)
          {
            zuo_biaozhi = 0;
            you_biaozhi = 0;
            Renzi_you_black();
            you_duan_black_find();             
          }
          if(you_biaozhi==1)
          {
            you_biaozhi = 0;
            zuo_biaozhi = 0;
            Renzi_zuo_black();
            zuo_duan_black_find();               
          }             
        }
      }
      else
      {
        Renzi_zhongcha_30();
        Renzi_zuo_black_30();
        Renzi_you_black_30();
        zuo_duan_black_find_30();
        you_duan_black_find_30();
              
        Renzi_zhongcha(); 
        Renzi_zuo_black();
        Renzi_you_black();
        zuo_duan_black_find();
        you_duan_black_find();
        zuo_duan_black_biao();
        you_duan_black_biao();
        zuo_you_buxian();
      }
    }
    else 
    {
      Find_zhidao();
      if(zhidao_flag==1)
      {
        zhidao_flag =0; 
        zhidao_column ++;
        if(zhidao_column >=2)
        {
          
          zhidao_flag1 =1;
        }
      }
    }
  }
}

#endif