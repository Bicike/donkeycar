/*
 * cmos.h 图像处理
 */

#ifndef _IMAGEPROCESS_1_H_
#define _IMAGEPROCESS_1_H_

#define qianzhan     89                                                         //设置前瞻的距离
#define Yuzhi        150                                                        //白天与夜晚阈值设置不一样 

uint8_t chang_huang =0;
uint8_t Black_zuo[qianzhan],Black_you[qianzhan],Black_zhongxin[qianzhan];
uint8_t Black_zuo_B[qianzhan],Black_you_B[qianzhan],Black_zhong_B[qianzhan];
uint8_t leftbreak_num =0,rightbreak_num =0;                                     //判断左右侧是否是全白的情况
uint8_t lostzuo_flag =0,lostyou_flag =0,stop_flag =0;
uint8_t no_duandian_flag =1,fengbi_flag =0;
uint8_t zuo_zhidao =0,you_zhidao =0,zhidao_column =0,zhidao_flag=0;
uint8_t duandian_flag =0,duandian_zuo_flag =0,duandian_you_flag =0,duandian_row =0;
/* 起跑线 */
int16_t  Black_zuo_ji_1[qianzhan],Black_you_ji_1[qianzhan];
int16_t  Black_zuo_ji_2[qianzhan],Black_you_ji_2[qianzhan];
long int Qi_jishu = 0 , Qi_zhouqi = 0;
/* 人字 */
uint8_t renzi_zuo =0,renzi_you =0;
uint8_t renzi_zuo_flag =0,renzi_you_flag =0,renzi_flag =0;
int16_t  Black_zuo_renzi[qianzhan],Black_you_renzi[qianzhan];
int16_t  Black_zuo_renzi_1[qianzhan],Black_you_renzi_1[qianzhan];
uint8_t zuo_chang_count =0,you_chang_count =0;
uint8_t zuo_flag1 =0,you_flag1 =0,zuo_flag2 =0,you_flag2 =0,renzi_high =0;

uint8_t zuorow_flag =0,yourow_flag =0,Qipao_biao =0,biaozhicha_flag =0;
uint8_t long_qianzhan =qianzhan , long_qianzhan_zuo =qianzhan , long_qianzhan_you =qianzhan;
uint8_t Black_zuo_hight_1[6][2];
uint8_t Black_you_hight_1[6][2];
/* 函数声明 */
void Max_To_Min();
void Send_Phone_1();
void Send_Phone_2();
void Send_Phone_3();
void duandian_panduan();
void Find_zuo_you_jiezhi();
void Find_Duandian();
void Find_zhidao();

void Renzi_Start();
void Renzi_zhongcha_30();
void Renzi_zhongcha();
void Renzi_zuo_black();
void Renzi_zuo_black_30();
void Renzi_you_black();
void Renzi_you_black_30();
void zuo_duan_black_find();
void zuo_duan_black_find_30();
void you_duan_black_find();
void you_duan_black_find_30();
void zuo_duan_black_biao();
void zuo_you_buxian();
void serial_send_image();
void blackcenter_get();
/**********************************黑线的中值滤波******************************/
int get_mid(unsigned int a,unsigned int b,unsigned int c)
{   
  unsigned int x=0;
  if(a>b){x=b;b=a;a=x;}
  if(b>c){x=c;c=b;b=x;}
  if(a>b){x=b;b=a;a=x;}
  return b;
}
/* 求差值的绝对值 */
int abss(int x,int y)
{
  int dif;
  if(x -y >0) dif=x-y;
  else dif=y-x;
  return dif;
}
/************************取所采图像的一半,每隔一个点读取一个数据***************/
void Max_To_Min()
{
  uint16_t  row =0,column =0;
  for(row=0;row<90;row++)
    for(column=0;column<160;column++)
      Pix_Data[row][column] =Pix_Data[row][column*2 +1];
  
  for(row=0;row<6;row++)
    for(column=0;column<2;column++)
    {
      Black_zuo_hight_1[row][column] =100;
      Black_you_hight_1[row][column] =100;
    }
}
/***************************第一场黑线提取*************************************/
void Find_Black_zuo_1()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)                                                 
  {
    for(j=79;j>0;j--)                                                           //每一行中列循环
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j -1] <Yuzhi))
      {
        Black_zuo[89 -i] =j;break;                                              //若点为黑则跳出列循环
      }
      if(j ==1)                                                                 //如果列为1，则执行以下判断
      if(Pix_Data[i][0] <Yuzhi)                                                 //判断每一行第一点的黑和白
      {
        Black_zuo[89 -i] =1;break;                                              //若行中第一点为黑则记为 "1"   
      }
      else{Black_zuo[89 -i] =0;break;}                                          //若行中第一点为白则记为 "0"
    } 
  }
}

void Find_Black_you_1()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)
  {
    for(j=80;j<159;j++)
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j +1] <Yuzhi))
      {
        Black_you[89 -i] =j;break;
      }
      if(j ==158)                                                               //如果列为148，则执行以下判断
      if(Pix_Data[i][159] <Yuzhi)                                               //判断每一行最后一点的黑和白
      {
        Black_you[89 -i] =158;break;                                            //若行中最后一点为黑则记为 "158"
      }  
      else{Black_you[89 -i] =159;break;}                                        //若行中最后一点为白则记为 "159"
    }  
  }
}
/*************************第一场图像处理***************************************/
void Send_Phone_1()                                                            
{
  Max_To_Min();                                                                 
  Find_Black_zuo_1();                                                           
  Find_Black_you_1(); 
  chang_huang =1;                                                              //进入跟踪场数据计算标志
}
/*************************第二场之后黑线提取***********************************/
/* 判断左边界线是否越界 */
unsigned char Black_zuo_return(unsigned char W)
{
  if(W ==89){return(Black_zuo[89 -W] +15);}                                     //判定第一行左边界的开始扫描点
  else 
  { 
    if((Black_zuo[88 -W] +10) >159){return (159);}                              //若左边界线越界，则返回最大值
    else{return(Black_zuo[88 -W] +10);}                                         //若左边界线没有越界，则返回原始值
  }  
}
/* 判断右边界线是否越界 */
unsigned char  Black_you_return(unsigned char W)                             
{
  if(W ==89){return(Black_you[89 -W] -10);}                                     //判定第一行右边界的开始扫描点
  else 
  {  
    if((Black_you[88 -W] -10) >0){return(Black_you[88 -W] -10);}                //若右边界线越界，则返回最小值
    else{return(0);}                                                            //若右边界线没有越界，则返回原始值                                                                   
  }   
}

void Find_Black_zuo_2()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)
  {
    for(j=(Black_zuo_return(i));j>0;j--)
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j -1] <Yuzhi))
      {
        Black_zuo[89 -i] =j;break;
      }    
      if(j ==1)
      {
        Black_zuo[89 -i] =0;break;          
      }
    }
  }
}

void Find_Black_you_2()
{
  uint8_t i,j;
  for(i=89;i>(89-qianzhan);i--)
  {
    for(j=(Black_you_return(i));j<159;j++)
    {
      if((Pix_Data[i][j] <Yuzhi)&&(Pix_Data[i][j +1] <Yuzhi))
      {
        Black_you[89 -i] =j;break;
      }
      if(j ==158)
      {
        Black_you[89 -i] =159;break; 
      } 
    }
  }
}
void Start_Black_flag()
{
  uint8_t i;
  for(i =0;i <qianzhan;i ++)
  {
    Black_zuo_ji_1[i] =Black_zuo[i];
    Black_you_ji_1[i] =Black_you[i];
  }
}
void Renzi_flag()
{
  uint8_t i;
  for(i =0;i <qianzhan;i ++)
  {
    Black_zuo_renzi[i] =Black_zuo[i];
    Black_you_renzi[i] =Black_you[i];
  }  
}
/*******************************防止得到错误断点*******************************/
/* 如果黑线封闭则置位无断点标志位和封闭线标志位 */
void duandian_panduan()
{
  unsigned char i;
  for(i=0;i<qianzhan;i++)
  {
    if(Black_zuo[i] >120)                                                       //如果左边黑线超出了预设定的范围则认为左黑线封闭
    {
      no_duandian_flag =1;                                                      //无断点标志位置位
      fengbi_flag =1;                                                           //封闭线标志位置位
      break;                                                                    //跳出查找断点循环
    }
    if(Black_you[i] <40)                                                        //如果右边黑线超出了预设定的范围则认为右黑线封闭
    {
      no_duandian_flag =1;                                                      //无断点标志位置位
      fengbi_flag =1;                                                           //封闭线标志位置位
      break;                                                                    //跳出查找断点循环
    }
  }
}
/*************************第二场以后的图像处理*********************************/
void Send_Phone_2()
{
   Max_To_Min();                                                              
   Find_Black_zuo_2();                                                       
   Find_Black_you_2(); 
   Start_Black_flag(); 
   Renzi_flag();
   duandian_panduan();
}

#endif
