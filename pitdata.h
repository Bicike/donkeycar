/*
 * pitdata.h
 */
#ifndef _PITDATA_H
#define _PITDATA_H

#include "main.h"
#include "senddata.h"
#include "structdata.h"
/* 函数声明 */
void pit_isr();
void ad_start();
void speed_read();
void Struct_init();
void angle_control();
void motor_output();
void speed_control_loc();
void speedcontrol_output();
void direction_control_loc();
void directincontrol_output();
void Black_Center();
void canshukongzhi(void);
void directionerror_get();
void directionerror_get1();

void display8x16Str(char page, char column, char *strrr);
void LCD_P6x8Str(char x,char y,char ch[]);
void LCD_P6x8Str_int(char x,char y,int data) ;
void LCD_P8x16Str_int(char x,char y,int data) ;
void FillArea(unsigned char spage, unsigned char epage,unsigned char scolumn,unsigned char ecolumn,unsigned char pt);
/* 变量定义 */
#define control_period 5	
#define speed_controlcount 20                                                  
#define direction_controlcount 1
#define speedcontrol_period (speed_controlcount *control_period)               
#define directioncontrol_period (direction_controlcount *control_period)    

int SpeedControl_Count,DirectionControl_Count,DirectionControl_Period,SpeedControl_Period;
int shizi_flag1 =0,zhidao_flag1=0,direction_flag=0,lost_flag1=0;
int num =0,SmartCar_Go=0,left_flag=0,right_flag=0;
int Speed_Add =100,maichong_flag =0;
int Stop_Timer =0;                                                              //车开始5s内检测起跑线
int SmartCar_Stop =0;
int Direct_Loc =0,black_center =0;
/* 人字 */
long int Maichong_Add1 =0,Maichong_Add2 =0,Maichong_Add3 =0,Maichong_Add4 =0,Maichong_Count =220000,Maichong_Renzi =80000;
int speed_di =1400;
int Maichong_Open =0;
int delay_count =0;
int speed_reduce1 =0,speed_reduce2 =0,speed_reduce3 =0,speed_reduce4 =0,speed_reduce5 =0,speed_reduce6 =0;
int speed_add1 =0,speed_add2 =0,speed_add3 =0;
int direction_reduce1 =0,direction_reduce2 =0,direction_reduce3 =0;
int direction_add1 =0,direction_add2 =0,direction_add3 =0;
/* 定时 */
int Timer_Open =0,System_Timer =0,System_TimerCount =8300,Timer_Count =10;      //初始化10s  
int renzi_p =300,renzi_d =5;
/***********************************角度控制***********************************/
void angle_control()
{
  grav.acc =(float)((Act.mma - Set.mma)*0.0694);
  gyro.angular =(float)((Act.enc1 - Set.enc1)*0.21);  
  balance.angles =0.98*(balance.angles +(gyro.angular*0.01)) +0.02*grav.acc;
  Angle->out = Angle->Kp*(0 -balance.angles) +Angle->Kd*(0 -gyro.angular); 
  if(Key.flag4 ==0)
  {
    if(balance.angles >=40 || balance.angles <= -50) {LPLD_GPIO_Set_b(PTD,10,OUTPUT_L);}
  }   
  /*OutData[0] =grav.acc*1;
  OutData[1] =gyro.angular*1;
  OutData[2] =balance.angles*1;
  OutPut_Data();*/
}
/***********************************速度控制***********************************/
void speed_read()
{
  left.pulse =FTM1_CNT;
  right.pulse =FTM2_CNT;
  if(left.pulse >32767) left.pulseold =(65535 -left.pulse); else left.pulseold =-left.pulse; 
  if(right.pulse >32767) right.pulseold =-(65535 -right.pulse); else right.pulseold =right.pulse; 
  FTM1_CNT =32767; FTM2_CNT =32767;
  left.pulsenew +=left.pulseold; 
  right.pulsenew +=right.pulseold;
}
/* 位置式PID */
void speed_control_loc()
{
  float iError=0,fP,fI;
  
  Speed->ActNewPoint =(float)(left.pulsenew + right.pulsenew)/2;
  left.pulsenew =0; right.pulsenew =0; 
  Speed->ActPoint =Speed->ActNewPoint*0.5 +Speed->ActPoint*0.5;//0.7 0.3弯道减速明显
  Maichong_Add1 +=Speed->ActNewPoint;

  if((Maichong_Add1 >=Maichong_Count)&&(Maichong_Add1 <Maichong_Count +10000))  //11500个脉冲
  {
    speed_reduce1 =1;
  }  
  else if((Maichong_Add1 >=Maichong_Count +10000)&&(Maichong_Add1 <Maichong_Count +20000))   
  {
    speed_reduce2 =1;
  }   
  else if((Maichong_Add1 >=Maichong_Count +30000)&&(Maichong_Add1 <Maichong_Count +Maichong_Renzi)) 
  {
    speed_reduce3 =1;
  }
  else if(Maichong_Add1 >=Maichong_Count +Maichong_Renzi)
  {
    speed_add1 =1;
  }
  
  if((Maichong_Add1 >=Maichong_Count)&&(Maichong_Add1 <Maichong_Count +40000))  //11500个脉冲
  {
    speed_reduce4 =1;
  }  
  else if((Maichong_Add1 >=Maichong_Count +40000)&&(Maichong_Add1 <Maichong_Count +Maichong_Renzi))   
  {
    speed_reduce5 =1;
  }   
  else if(Maichong_Add1 >=Maichong_Count +Maichong_Renzi)
  {
    speed_add2 =1;
  }  
  
  if((Maichong_Add1 >=Maichong_Count)&&(Maichong_Add1 <Maichong_Count +11500))  //11500个脉冲
  {
    speed_reduce6 =1;
  }  
  else if(Maichong_Add1 >=Maichong_Count +Maichong_Renzi)
  {
    speed_add3 =1;
  } 
  
  if(SmartCar_Go==1)  
  {
    if(Speed->ActPoint <=Speed->SetfDelta)
    {
      Speed->SetPoint +=Speed_Add;
      if(Speed->SetPoint >=Speed->SetfDelta)
      {
        Speed->SetPoint =Speed->SetfDelta;
        SmartCar_Go =2;
      }
    }
  }
  else if(SmartCar_Go ==2)
  {
    if(Speed->ActPoint <=Speed->SetfDelta)
    {
      Speed->SetPoint =Speed->SetfDelta;
    }
  }
  
  iError =Speed->SetPoint;
  iError -=Speed->ActPoint;
  fP =iError *Speed->Kp;
  fI =iError *Speed->Ki;
  Speed->SumError +=fI;
  if(Speed->SumError >=Speed->max) {Speed->SumError =Speed->max;}
  if(Speed->SumError <=Speed->min) {Speed->SumError =Speed->min;}  
  Speed->outold =Speed->outnew;
  Speed->outnew =(fP                                                            //比例项
                 +Speed->SumError                                               //积分项
                 );               
  if(Speed->outnew >=9999) Speed->outnew =9999;
  if(Speed->outnew <=-9999) Speed->outnew =-9999;  
}
/* 速度平滑输出 */
void speedcontrol_output()
{
  float fValue;
  fValue =Speed->outnew -Speed->outold;
  Speed->out =fValue *(SpeedControl_Period +1)/speedcontrol_period +Speed->outold;
}
/***********************************方向控制***********************************/
/* 高速 2700 7.8v 蛮好的 */
void direction_control_loc1()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint);
  if(Direction->SumError>20)
  {
    Direction->SumError =20;
  }
  if(Direction->SumError<-20)
  {
    Direction->SumError =-20;
  }
  Direction->LastError =(float)((Act.enc2 -Set.enc2)/2);  
  Direction->outold =Direction->outnew;
  Direction->outnew =(400*Direction->SumError          
                     +14*Direction->LastError                   
                     );   
}
void direction_control_loc2()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint)/2;
  if(Direction->SumError>12)
  {
    Direction->SumError =12;
  }
  if(Direction->SumError<-12)
  {
    Direction->SumError =-12;
  }
  Direction->LastError =(float)((Act.enc2 -Set.enc2));  
  Direction->outold =Direction->outnew;
  Direction->outnew =(710*Direction->SumError          
                     +7*Direction->LastError                   
                     );   
}
void direction_control_loc3()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint)/2;
  if(Direction->SumError>12)
  {
    Direction->SumError =12;
  }
  if(Direction->SumError<-12)
  {
    Direction->SumError =-12;
  }
  Direction->LastError =(float)((Act.enc2 -Set.enc2)/2);  
  Direction->outold =Direction->outnew;
  Direction->outnew =(700*Direction->SumError          
                     +15*Direction->LastError                   
                     );   
}
/* 中速 2750 7.7  跑完29秒 能过障碍 P给大一点 */
void direction_control_loc4()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint)/2;
  if(Direction->SumError>12)
  {
    Direction->SumError =12;
  }
  if(Direction->SumError<-12)
  {
    Direction->SumError =-12;
  }
  Direction->LastError =(float)((Act.enc2 -Set.enc2));  
  Direction->outold =Direction->outnew;
  Direction->outnew =(740*Direction->SumError          
                     +7*Direction->LastError                   
                     );   
}
/* 低速2400~2500 */
void direction_control_loc5()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint)/2;
  if(Direction->SumError>13)
  {
    Direction->SumError =13;
  }
  if(Direction->SumError<-13)
  {
    Direction->SumError =-13;
  }
  Direction->LastError =(float)((Act.enc2 -Set.enc2)/2);  
  Direction->outold =Direction->outnew;
  Direction->outnew =(700*Direction->SumError          
                     +15*Direction->LastError                   
                     );   
}
void direction_control_renzi()
{
  Direction->SumError =(Direction->ActPoint -Direction->SetPoint);
  if(Direction->SumError>30)
  {
    Direction->SumError =30;
  }
  if(Direction->SumError<-30)
  {
    Direction->SumError =-30;
  }/**/
  Direction->LastError =(float)((Act.enc2 -Set.enc2));  
  Direction->outold =Direction->outnew;
  Direction->outnew =(renzi_p*Direction->SumError       
                     +renzi_d*Direction->LastError                   
                     );   
}
/* 方向平滑输出 */
void directincontrol_output()
{
  float fValue;
  fValue =Direction->outnew -Direction->outold;
  Direction->out =fValue *(DirectionControl_Period +1)/directioncontrol_period +Direction->outold;
}
/***********************************电机控制***********************************/
void motor_output()
{
  if(Direction->start ==0)
  {
    Left.outold =(int)(Angle->out -Speed->out);//
    Right.outold =(int)(Angle->out -Speed->out);//                   
  }
  else
  {
    Left.outold =(int)(Angle->out -Speed->out -Direction->out);//
    Right.outold =(int)(Angle->out -Speed->out +Direction->out);//             
  }
  /* 左轮正输出 */
  if(Left.outold <=0)
  {
    LPLD_FTM0_PWM_ChangeDuty(1,0);   
    Left.outnew =-Left.outold +Left.deadvalue; 
    if(Left.outnew >=9999) Left.outnew =9999;
    LPLD_FTM0_PWM_ChangeDuty(0,Left.outnew);
  } 
  else 
  {
    LPLD_FTM0_PWM_ChangeDuty(0,0);
    Left.outnew =Left.outold +Left.deadvalue;
    if(Left.outnew >=9999) Left.outnew =9999;
    LPLD_FTM0_PWM_ChangeDuty(1,Left.outnew);   
  }
  /* 右轮正输出 */
  if(Right.outold <=0)
  {
    LPLD_FTM0_PWM_ChangeDuty(7,0);
    Right.outnew =-Right.outold +Right.deadvalue;
    if(Right.outnew >=9999) Right.outnew =9999;
    LPLD_FTM0_PWM_ChangeDuty(6,Right.outnew);
  }
  else 
  {
    LPLD_FTM0_PWM_ChangeDuty(6,0);
    Right.outnew =Right.outold +Right.deadvalue;
    if(Right.outnew >=9999) Right.outnew =9999;
    LPLD_FTM0_PWM_ChangeDuty(7,Right.outnew);
  }
}
void ad_start()
{
  int i,mma,enc1,enc2;
  for(i =0;i <200;i ++)
  {
    enc1 +=LPLD_ADC_SE_Get(ADC0,16);
    enc2 +=LPLD_ADC_SE_Get(ADC0,17); 
    mma  +=LPLD_ADC_SE_Get(ADC0,18);                                        
  }
  Set.mma =mma /200;
  Set.enc1 =enc1 /200;
  Set.enc2 =enc2 /200;
}
/*******************************1ms中断函数************************************/   
void pit_isr()
{
  num ++;
  if(Timer_Open ==1)  
  {
    System_Timer ++;                                                            //830 -> 1S
    if(System_Timer >=System_TimerCount) 
    {
      System_Timer =0;
      LPLD_GPIO_Set_b(PTD,10,OUTPUT_L);
    }
  }
  /* 高速 */
  if(Maichong_Open ==1)
  {
    if(speed_reduce6 ==1)
    {  
      Speed->SetPoint =speed_di;
      direction_reduce1 =1; 
    }
    if(speed_add3 ==1)
    {
      direction_reduce1 =0;
      SmartCar_Go =1;
    } 
  }
    
  /* 中速 */
  if(Maichong_Open ==2)
  {
    if(speed_reduce4 ==1)
    {
      SmartCar_Go =3;
      Speed->SetPoint =2400;
      speed_reduce4 =0;
      Direction->Kd =14 +7;
      Direction->Kp =400 -10;
    } 
    if(speed_reduce5 ==1)
    {
      Speed->SetPoint =speed_di;
      speed_reduce5 =0;
      Direction->Kd =14 +5; 
      Direction->Kp =400 -60;
      direction_reduce1 =1;
    }       
    if(speed_add2 ==1)
    {
      direction_reduce1 =0;
      SmartCar_Go =1;
    } 
  }  
  /* 低速 */
  if(Maichong_Open ==3)
  {
    if(speed_reduce1 ==1)
    {
      SmartCar_Go =3;
      Speed->SetPoint =2200;
      speed_reduce1 =0;
      Direction->Kd =14 +7;
      Direction->Kp =400 -10;
    } 
    if(speed_reduce2 ==1)
    {
      Speed->SetPoint =1600;
      speed_reduce2 =0;
      Direction->Kd =14 +5; 
      Direction->Kp =400 -60;
    }   
    if(speed_reduce3 ==1)
    {
      Speed->SetPoint =speed_di;
      speed_reduce3 =0;
      Direction->Kd =11;
      Direction->Kp =400 -60;
      direction_reduce1 =1;
    }      
    if(speed_add1 ==1)
    {
      direction_reduce1 =0;
      SmartCar_Go =1;
    } 
  }
  
  SpeedControl_Period ++; 
  speedcontrol_output();
  DirectionControl_Period ++; 
  directincontrol_output();
  if(num >=5)
  {
    num =0;
    speed_read();                 
  }
  else if(num ==1)
  {
    Act.mma =LPLD_ADC_SE_Get(ADC0,18);    
    Act.enc1 =LPLD_ADC_SE_Get(ADC0,16);   
    Act.enc2 =LPLD_ADC_SE_Get(ADC0,17);   
  }
  else if(num ==2)
  {
    angle_control();
    motor_output();   
  }
  else if(num ==3)
  {
    SpeedControl_Count ++;
    if(SpeedControl_Count >=speed_controlcount)
    {
      speed_control_loc();
      SpeedControl_Count =0; SpeedControl_Period =0;
    }
  }
  else if(num ==4)
  {
    direction_flag =1;
    DirectionControl_Count ++;
    if(DirectionControl_Count >=direction_controlcount)
    {
      if(direction_reduce1 ==0)
      {
        if(Direct_Loc ==1)
        {
          direction_control_loc1();
        }
        else if(Direct_Loc ==2)
        {
          direction_control_loc2();
        }
        else if(Direct_Loc ==3)
        {
          direction_control_loc3();
        }
        else if(Direct_Loc ==4)
        {
          direction_control_loc4();
        }
        else if(Direct_Loc ==5)
        {
          direction_control_loc5();
        }      
      }
      else
      {
        direction_control_renzi(); 
      }
      DirectionControl_Count =0; DirectionControl_Period =0;
    }          
  }
}

#endif 
