/*
 * CMOS.h ͼ����
 */

#ifndef __IMAGE_PROCESS_2_H__
#define __IMAGE_PROCESS_2_H__ 

#include "imageprocess_1.h"

/********************�߽��պ󲹳��������߶ȵ����ұ߽�**********************/
/* �߽��պ���߽�������߶� */
void Find_zuo()
{
  unsigned char i;
  for(i = 1;i < qianzhan;i ++)
  {
    if((Black_zuo[i] -Black_zuo[i -1] >0)&(Black_zuo[i] >140))
    {
      long_qianzhan_zuo =i;
      break;
    }
  }
}
/* �߽��պ��ұ߽�������߶� */
void Find_you()
{
  unsigned char i;
  for(i = 1;i < qianzhan;i ++)
  {
    if((Black_you[i] -Black_you[i +1] >0)&(Black_you[i +1] <20))
    {
      long_qianzhan_you =i;
      break;
    }
  }
}

void Find_zuo_you_jiezhi()
{
  unsigned char i,zuo_Bai,you_Bai;
  long_qianzhan_zuo =long_qianzhan_you =qianzhan;
  Find_zuo();
  Find_you();
  if(long_qianzhan_you >=long_qianzhan_zuo)
  {
    long_qianzhan =long_qianzhan_zuo;
  }
  else
  {
    long_qianzhan =long_qianzhan_you;
  }
  for(i =0;i <long_qianzhan;i ++)
  {
    if(Black_zuo[i] == 0)
    {
      zuo_Bai ++;
    }
    if(Black_you[i] == 159)
    {
      you_Bai ++;
    }
  }
  if(zuo_Bai >you_Bai)
  {
    for(i =long_qianzhan;i <qianzhan;i ++)
    {
      Black_zuo[i] =0;
      Black_you[i] =10;
    }
  }
  else
  {
    for(i =long_qianzhan;i <qianzhan;i ++)
    {
      Black_zuo[i] = 150;
      Black_you[i] = 159;          
    }
  }
}
/*************************************�ϵ����*********************************/
/* ����߷�����ȡ���Һ��������� */
void Find_Black_zuoturn()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)
  {
    for(j=Black_zuo[89-i];j<159;j++)
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j +1] <Yuzhi))
      {
        Black_zuo_B[89 -i] =j;break;
      }
      if(j ==158)
      {
        Black_zuo_B[89 -i] =159;break;          
      }
    }
  }
}
/* �Һ��߷�����ȡ������������� */
void Find_Black_youturn()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)
  {
    for(j=Black_you[89-i];j>0;j--)
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j -1] <Yuzhi))
      {
        Black_you_B[89 -i] =j;break;
      }
      if(j ==1)
      {
        Black_you_B[89 -i] =0;break; 
      }
    }
  }
}

void Find_Duandian()
{
  uint8_t i,zuo_flag1 =0,you_flag1 =0;
  
  Find_Black_zuoturn();
  Find_Black_youturn();
  for(i =0;i <70;i ++)
  {
    if(Black_zuo[i]!=Black_you_B[i])
    {
      zuo_flag1 ++;
    }
    if(Black_you[i]!=Black_zuo_B[i])
    {
      you_flag1 ++;
    }
  }
  if((you_flag1>2)&&(zuo_flag1>2))
  {
    duandian_flag =1;
  }
}

void Find_Renzi()
{
  uint8_t i;
  //uint8_t chang_duan =0,chang_fenbi =0;
  zuo_flag1 =you_flag1 =zuo_flag2 =you_flag2 =renzi_high =0;
  Send_Phone_3();
    
  for(i=0;i<qianzhan;i++)                                                    
  {
    if(Black_zuo[i] -Black_zuo[i +1] >20)
    { 
      zuo_flag1 ++;
      zuo_flag2 =i;
      duandian_zuo_flag =Black_zuo[i]; 
      break;
    }
  }
  for(i=0;i<qianzhan;i++)                                                    
  {
    if(Black_you[i +1] -Black_you[i] >20)
    {
      you_flag1 ++;
      you_flag2 =i;
      duandian_you_flag =Black_you[i];
      break;
    }
  }  
  if((zuo_flag1 >3) && (you_flag1 >3))
  {
    if(zuo_flag1 >you_flag1)
    {
      for(i =you_flag2;i <qianzhan;i ++)
      {
        Black_you[i] =159;
      }
    }
    else
    {
      for(i =zuo_flag2;i <qianzhan;i ++)
      {
        Black_zuo[i] =0;
      }
    }
  }
  if(zuo_flag1 >3)
  {
    renzi_zuo_flag =1;
    zuo_chang_count ++;
    for(i =zuo_flag2;i <qianzhan;i ++)
    {
      Black_zuo[i] =0;
    }
  }  
  if(you_flag1 >3)
  {
    renzi_you_flag =1;
    you_chang_count ++;
    for(i =you_flag2;i <qianzhan;i ++)
    {
      Black_you[i] =159;
    }
  }  
}
/*************************************ֱ������*********************************/
void Black_center_lvbo()
{
    unsigned char i = 0;
    int sum = 0;
    for(i = 2;i < qianzhan-2;i++)
    {
      sum = Black_zhongxin[i-2]+Black_zhongxin[i-1]+Black_zhongxin[i]+Black_zhongxin[i+1]+Black_zhongxin[i+2];
      Direction->ActPoint = sum/5;
    }
}

void Find_zhidao()
{
  unsigned char i,left_zhidao =0,right_zhidao =0;
  for(i =0;i <qianzhan -1;i ++)
  {
    if((Black_zuo[i +1] -Black_zuo[i] <2)&&(Black_zuo[i +1] -Black_zuo[i] >-2))
    {
      left_zhidao ++;
    }
    if((Black_you[i] -Black_you[i +1] <2)&&(Black_you[i] -Black_you[i +1] >-2))
    {
      right_zhidao ++;
    }
  }
  if((left_zhidao ==88)&&(right_zhidao ==88))
  {
    zhidao_flag =1;
    Black_center_lvbo();
  }
}
/********************************�����ߵ��ж�**********************************/
unsigned char Black_zuo_return_qi(unsigned char Z)                             
{
  if(Z==89){ return(80);}                                                       //�ж���һ����߽�Ŀ�ʼɨ���
  else 
  {
    if((Black_zuo_ji_2[88 -Z] +5) >158){return(159);}                           //����߽���Խ�磬�򷵻����ֵ
    else{return(Black_zuo_ji_2[88-Z] +5);}                                    //����߽���û��Խ�磬�򷵻�ԭʼֵ
  }
}

void Find_black_zuo_qi()                                                       //����߽��ߺ���
{
  uint8_t  i,j;
  for(i = 89 ; i > 64 ; i --)                                                   //ÿһ������ѭ��
  {
    for(j = (Black_zuo_return_qi(i)) ; j > 0 ; j --)                            //ÿһ������ѭ��
    {
      if((Pix_Data[i][j] < Yuzhi)&&(Pix_Data[i][j-1]<Yuzhi))                    //�жϴ˵�ĺںͰ�
      {
          Black_zuo_ji_2[89-i] = j ; break;                                          //����Ϊ����������ѭ��
      }
      
      if(j==1)                                                                  //�����Ϊ1����ִ�������ж�
      {
          Black_zuo_ji_2[89-i] = 0 ; break;                                          //�����е�һ��Ϊ�����Ϊ "1"
      }
    }
  }
}

unsigned char Black_you_return_qi(unsigned char Z)                             //�ж��ұ߽����Ƿ�Խ��
{
  if(Z ==89){return(80);}                                            //�ж���һ���ұ߽�Ŀ�ʼɨ���
  else
  {
    if((Black_you_ji_2[88 -Z] -5) >0){return( Black_you_ji_2[88 -Z] -5);}       //���ұ߽���Խ�磬�򷵻���Сֵ
    else{return(0);}                                                          //���ұ߽���û��Խ�磬�򷵻�ԭʼֵ
  }
}

void Find_black_you_qi()                                                       //���ұ߽��ߺ���  
{
  uint8_t i,j;
  for(i =89 ;i> 64;i --)                                                  //ÿһ������ѭ��
  {
    for(j = (Black_you_return_qi(i));j <159;j ++)                          //ÿһ������ѭ��
    {
      if((Pix_Data[i][j]<Yuzhi)&&(Pix_Data[i][j+1]<Yuzhi))                      //�жϴ˵�ĺںͰ�
      {
          Black_you_ji_2[89-i] = j ; break;                                     //����Ϊ����������ѭ��
      }
      if(j==158)                                                                //�����Ϊ153����ִ�������ж�
      {
          Black_you_ji_2[89-i] = 159 ; break;                                   //���������һ��Ϊ�����Ϊ "159"
      }
    }
  }
}

void Start_Black_Find()
{
  unsigned char  i , j , Qipao_xian_A = 0 , Qipao_xian_B = 0 , Qipao_xian_1 = 0 , Qipao_xian_2 = 0 ;
  int8_t QA_1 =0,QA_2 =0,QA_3 =0,QA_4 =0;
  if((Black_you_ji_1[0] -Black_zuo_ji_1[0] >100)&&(Black_you_ji_1[0] -Black_zuo_ji_1[0] <130))
  {
      Find_black_zuo_qi();
      Find_black_you_qi();
  }
  for(i =10;i <24;i ++)
  {
    if((Black_zuo_ji_2[i]-Black_zuo_ji_2[i+1]<-2)||(Black_zuo_ji_2[i]-Black_zuo_ji_2[i+1]>2)||(Black_zuo_ji_2[i]==0))
    {
      Qipao_xian_1 = 1;
      break;
    }
  }
  for(i =10;i <24;i ++)
  {
    if((Black_you_ji_2[i]-Black_you_ji_2[i+1]<-2)||(Black_you_ji_2[i]-Black_you_ji_2[i+1]>2)||(Black_you_ji_2[i]==159))
    {
      Qipao_xian_2 = 1;
      break;
    }
  }  
  if((Qipao_xian_1==0)&&(Qipao_xian_2==0))
  {
    for(i =79;i >65;i --)
    {
      for(j = Black_zuo_ji_2[89-i]+1; j < Black_you_ji_2[89-i]-1; j ++)
      {
        if(QA_1==0)
        {
          if(Pix_Data[i][j] < Yuzhi)
          {
            QA_1 = j;
          }          
        }
        else
        {
          if(QA_2==0)
          {
            if(Pix_Data[i][j] > Yuzhi)
            {
              QA_2 = j;
            }             
          }
          else
          {
            if(QA_3==0)
            {
              if(Pix_Data[i][j] < Yuzhi)
              {
                QA_3 = j;
              }
            }
            else
            {
              if(QA_4==0)
              {
                if(Pix_Data[i][j] > Yuzhi)
                {
                  QA_4 = j;
                }
              }
            }
          }
        }
      }
      if((QA_1!=0)&&(QA_2!=0)&&(QA_3!=0)&&(QA_4!=0))
      {
         Qipao_xian_A = 1;
         break;
      }
      else
      {
         QA_1 = QA_2 = QA_3 = QA_4 = 0;
      }
    }
    for(i = 79; i > 65; i --)
    {
      for(j = Black_zuo_ji_2[89-i]+1; j < Black_you_ji_2[89-i]-1; j ++)
      {
        if(Pix_Data[i][j] < Yuzhi)
        {
          Qipao_xian_B ++;
        }
      }
    }
  }
  if((Qipao_xian_B<180)&&(Qipao_xian_B>60)&&(Qipao_xian_A==1))
  {
    Qipao_biao = 1;
  }
}
/**********************************�����ж�************************************/


#endif
