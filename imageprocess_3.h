
/*
 * CMOS.h ͼ����
 */

#ifndef __IMAGE_PROCESS_3_H__
#define __IMAGE_PROCESS_3_H__ 

#include "imageprocess_2.h"

#define         Xu_qianzhan     85
uint16_t        zhongzhi = 0 , zuo_biaozhi = 0 , you_biaozhi = 0;

/*****************************************************************************/
/**** ��Ҫ�ж������Ƿ��Ǽ���Ƕ� ********
      ����������ұ�־λ��λ             *************************************/
/*****************************************************************************/
void Renzi_Start()
{
  unsigned char  i , zuo_biao = 0 , you_biao = 0;
  for(i = 0; i < Xu_qianzhan; i ++)
  {
    if(Black_zuo[i] == 0 )
    {
      zuo_biao ++;
    }
    if(Black_you[i] == 159)
    {
      you_biao ++;
    }
  }
  if((zuo_biao>75)&&(you_biao<35))
  {
    zuo_biaozhi = 1;
    zhongzhi = 20;
  }
  if((zuo_biao<35)&&(you_biao>75))
  {
    you_biaozhi = 1;
    zhongzhi  = 140;
  }
}

/*****************************************************************************/
/****����������������ǰ30�еı߽���ʼ��*************************************/
/*****************************************************************************/

void Renzi_zhongcha_30()
{
  uint8_t  i;
  for(i = 0 ; i < 30 ; i++) 
  {
    Black_zhong_B[i] = (Black_you[i] + Black_zuo[i] + Black_you_B[i] + Black_zuo_B[i]) / 4;
  }
}

/*****************************************************************************/
/****  ������������������ʼ��  **************
        ��ֵ�ļ���          *******************************/
/*****************************************************************************/

void Renzi_zhongcha()
{
  unsigned char i;
  uint16_t  sum = 0 , count = 0 ;
  
  for(i = 0; i < 30; i ++)
  {
    if(((Black_you[i]!=159)||(Black_zuo[i]!=0)))
    {
      sum += ((Black_you[i]+Black_zuo[i])/2);
      count ++;
    }
  }
  zhongzhi = sum/count ;
}
/*****************************************************************************/
/****�����в�������ߺ���*********************************************/
/*****************************************************************************/

void Renzi_zuo_black()
{
  int16_t i,j;
  for( i = 89 ; i > (89-Xu_qianzhan) ; i --)                                    //ÿһ������ѭ��
  {
    for(j = zhongzhi ; j > 0 ; j --)                                            //ÿһ������ѭ��
    {
      if((Pix_Data[i][j]<Yuzhi)&&(Pix_Data[i][j-1]<Yuzhi))
      {
        Black_zuo[89-i] = j ; break; 
      }
      if(j==1)                                                                  //�����Ϊ1����ִ�������ж�
      {
        Black_zuo[89-i] = 0 ; break;                                            //���������һ��Ϊ�����Ϊ "0"
      }
    }
  }  
}

void Renzi_zuo_black_30()
{
  uint8_t i,j;
  for( i = 89 ; i > 59 ; i --)                                                  //ÿһ������ѭ��
  {
    for(j = Black_zhong_B[89-i] ; j > 0 ; j --)                                 //ÿһ������ѭ��
    {
      if((Pix_Data[i][j]<Yuzhi)&&(Pix_Data[i][j-1]<Yuzhi))
      {
        Black_zuo[89-i] = j ; break; 
      }
      if(j==1)                                                                  //�����Ϊ1����ִ�������ж�
      {
        Black_zuo[89-i] = 0 ; break;                                            //���������һ��Ϊ�����Ϊ "0"
      }
    }
  }  
}
/*****************************************************************************/
/****�����в����Һ��ߺ���*********************************************/
/*****************************************************************************/

void Renzi_you_black()
{
  int16_t i,j;
  for( i = 89 ; i > (89-Xu_qianzhan) ; i --)                                    //ÿһ������ѭ��
  {
    for(j = zhongzhi ; j < 159 ; j ++)                               //ÿһ������ѭ��
    {
      if((Pix_Data[i][j]<Yuzhi)&&(Pix_Data[i][j+1]<Yuzhi))
      {
        Black_you[89-i] = j; break;
      }
      if(j==158)
      {
        Black_you[89-i] = 159 ; break; 
      }
    }
  }
}

void Renzi_you_black_30()
{
  uint8_t i,j;
  for( i = 89 ; i > 59 ; i --)                                    //ÿһ������ѭ��
  {
    for(j = Black_zhong_B[89-i] ; j < 159 ; j ++)                               //ÿһ������ѭ��
    {
      if((Pix_Data[i][j]<Yuzhi)&&(Pix_Data[i][j+1]<Yuzhi))
      {
        Black_you[89-i] = j; break;
      }
      if(j==158)
      {
        Black_you[89-i] = 159 ; break; 
      }
    }
  }
}
/*****************************************************************************/
/****������ߺ��ߵĶϵ㲢�� 0 *********************************************/
/*****************************************************************************/

void zuo_duan_black_find()
{
  uint16_t   i , flag_zuo=0;
  
  for(i = 0;i < Xu_qianzhan; i ++)
  {
    if(flag_zuo==0)
    {
      if(Black_zuo[i-1] - Black_zuo[i] > 3)
      {
        Black_zuo[i] = 0;
        flag_zuo = 1;
      }
    }
    else
    {
      if(Black_zuo[i+1] - Black_zuo[i] > 5)
      {
        Black_zuo[i] = 0;
        flag_zuo = 0;
      }
      else
      {
        Black_zuo[i] = 0;
      }
    }
  }
}

void zuo_duan_black_find_30()
{
  unsigned char i,flag_zuo=0;
  for(i = 1;i < 30; i ++)
  {
    if(flag_zuo==0)
    {
      if(Black_zuo[i-1] - Black_zuo[i] > 3)
      {
        Black_zuo[i] = 0;
        flag_zuo = 1;
      }
    }
    else
    {
      if(Black_zuo[i+1] - Black_zuo[i] > 5)
      {
        Black_zuo[i] = 0;
        flag_zuo = 0;
      }
      else
      {
        Black_zuo[i] = 0;
      }
    }
  }
}
/*****************************************************************************/
/****�����ұߺ��ߵĶϵ㲢�� 159 *********************************************/
/*****************************************************************************/

void you_duan_black_find()
{
  unsigned char i,flag_you=0;
  for(i = 1;i < Xu_qianzhan;i ++)
  {
    if(flag_you==0)
    {
      if(Black_you[i] - Black_you[i-1] > 3)
      {
        Black_you[i] = 159;
        flag_you = 1;
      }
    }
    else
    {
      if(Black_you[i] - Black_you[i+1] > 5)
      {
        Black_you[i] = 159;
        flag_you = 0;
      }
      else
      {
        Black_you[i] = 159;
      }
    }
  }
}
void you_duan_black_find_30()
{
  unsigned char i,flag_you=0;
  for(i = 1;i < 30;i ++)
  {
    if(flag_you==0)
    {
      if(Black_you[i] - Black_you[i-1] > 3)
      {
        Black_you[i] = 159;
        flag_you = 1;
      }
    }
    else
    {
      if(Black_you[i] - Black_you[i+1] > 3)
      {
        Black_you[i] = 159;
        flag_you = 0;
      }
      else
      {
        Black_you[i] = 159;
      }
    }
  }
}

/*****************************************************************************/
/****   ��-�Ҷϵ㴦�����ܺ���    *********************************************/
/*****************************************************************************/

void zuo_duan_black_biao()
{
  unsigned char i,flag_zuo=0,flag_high=0;
  for(i = 0;i < 65;i ++)
  {
    if(flag_zuo==0)
    {
      if(Black_zuo[i]==0)
      {
        if(i==0)
        {
          Black_zuo_hight_1[flag_high][flag_zuo] = 0;
          flag_zuo = 1;
        }
        else
        {
          Black_zuo_hight_1[flag_high][flag_zuo] = i-1;
          flag_zuo = 1;
        }
      }
    }
    else
    {
      if(Black_zuo[i]!=0)
      {
        Black_zuo_hight_1[flag_high][flag_zuo] = i;
        flag_high ++;
        flag_zuo = 0;
      }
    }
  }
}
void zuo_buxian(unsigned char X,unsigned char Y)
{
  uint8_t  i;
  
  for(i = X+1;i < Y;i ++)
  {
    Black_zuo[i] = (Black_zuo[X]+Black_zuo[Y])/2;
  }
}
void zuo_buxian1(unsigned char X,unsigned char Y)
{
  uint8_t i;
  float bu_a;
  if(Black_zuo[X] - Black_zuo[Y] >0)
  {
    bu_a =(float)(Black_zuo[X] -Black_zuo[Y])/(float)(Y -X);
    for(i =X +1;i <Y;i ++)
    {
      Black_zuo[i] =Black_zuo[X] -(uint8_t)(bu_a*(float)(i -X));
    }
  }
  else
  {
    bu_a =(float)(Black_zuo[Y] - Black_zuo[X])/(float)(Y -X);
    for(i =X +1;i <Y;i ++)
    {
      Black_zuo[i] =Black_zuo[X] +(uint8_t)(bu_a*(float)(i -X));
    }
  }
}
void you_duan_black_biao()
{
  unsigned char i,flag_you=0,flag_high=0;
  for(i = 0;i < 65;i ++)
  {
    if(flag_you==0)
    {
      if(Black_you[i]==159)
      {
        if(i == 0)
        {
          Black_you_hight_1[flag_high][flag_you] = 0;
          flag_you = 1;
        }
        else
        {
          Black_you_hight_1[flag_high][flag_you] = i-1;
          flag_you = 1;
        }
      }
    }
    else
    {
      if(Black_you[i]!=159)
      {
        Black_you_hight_1[flag_high][flag_you] = i;
        flag_high ++;
        flag_you = 0;
      }
    }
  }
}

void  you_buxian(unsigned char X,unsigned char Y)
{
  uint8_t  i;
  
  for(i = X+1;i < Y;i ++)
  {
    Black_you[i] = (Black_you[X] + Black_you[Y])/2 ;
  }
}
void you_buxian1(unsigned char X,unsigned char Y)
{
  uint8_t i;
  float bu_b;
  if(Black_you[X] - Black_you[Y] >0)
  {
    bu_b =(float)(Black_you[X] - Black_you[Y])/(float)(Y -X);
    for(i =X +1;i <Y;i ++)
    {
      Black_you[i] =Black_you[X] -(uint8_t)(bu_b*(float)(i -X));
    }
  }
  else
  {
    bu_b =(float)(Black_you[Y] - Black_you[X])/(float)(Y -X);
    for(i =X +1;i <Y;i ++)
    {
      Black_you[i] =Black_you[X] +(uint8_t)(bu_b*(float)(i -X));
    }
  }
}

void zuo_you_buxian()
{
   unsigned char  i;
  
    for(i = 0;i < 6;i ++)
    {
      if((Black_zuo_hight_1[i][0]!=100)&&(Black_zuo_hight_1[i][1]!=100))
      {
        zuo_buxian(Black_zuo_hight_1[i][0],Black_zuo_hight_1[i][1]);
      }
      else { break; }
    }   
  
    for(i = 0;i < 6;i ++)
    {
      if((Black_you_hight_1[i][0]!=100)&&(Black_you_hight_1[i][1]!=100))
      {
        you_buxian(Black_you_hight_1[i][0],Black_you_hight_1[i][1]);
      }
      else { break; }
    }  
}
#endif
