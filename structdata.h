/*
 * structdata.h 
 */

#ifndef __STRUCTDATA_H__
#define __STRUCTDATA_H__ 

/* KEY初始化 */
typedef struct key
{
  uint8_t check1;
  uint8_t check2;
  uint8_t check3;
  uint8_t check4;
  uint8_t check5;
  uint8_t check6;
  uint8_t flag1;
  uint8_t flag2;
  uint8_t flag3;
  uint8_t flag4;
  uint8_t flag5;
  uint8_t flag6;
  uint8_t flag7;
  uint8_t flag8;
  uint8_t enc;
  uint8_t count1;
  uint8_t count2;
  uint8_t count3;
}key;
key Key;

void key_init()
{
  Key.check1 =0;
  Key.check2 =0;
  Key.check3 =0;
  Key.check4 =0;
  Key.check5 =0;
  Key.check6 =0;
  Key.flag1 =2;
  Key.flag2 =0;
  Key.flag3 =0;
  Key.flag4 =0;
  Key.flag5 =0;
  Key.flag6 =0;
  Key.flag7 =0;
  Key.flag8 =0;
  Key.enc =0;
  Key.count1 =0;
  Key.count2 =0;
  Key.count3 =0;
}   
/* AD采集初始化 */
typedef struct AD
{
  int mma;
  int enc1;
  int enc2;
}AD;
AD Set,Act;

void adget_init()
{
  Set.mma =0;
  Set.enc1 =0;
  Set.enc2 =0;
  Act.mma =0;
  Act.enc1 =0;
  Act.enc2 =0;
}
/* 互补滤波初始化 */
typedef struct Filter
{
  float acc;                                                                    //加速度
  float angular;                                                                //角速度
  float angles;

}Filter;
Filter grav,gyro,balance;

void filter_init()
{
  grav.acc =0.0;
  gyro.angular =0.0;
  balance.angles =0.0;
}
/* 脉冲采集初始化 */
typedef struct Pulse
{
  int pulse;
  int pulseold;
  int pulsenew;
}Pulse;
Pulse left,right;

void speedget_init()
{
  left.pulse =0;
  left.pulseold =0;
  left.pulsenew =0;
  right.pulse =0;
  right.pulseold =0;
  right.pulsenew =0;
}
/* 电机控制初始化 */
typedef struct Motor
{
  int outold;
  int outnew;
  int deadvalue;
}Motor;
Motor Left,Right;

void motorcontrol_init()
{
  Left.outold =0;
  Left.outnew =0;
  Left.deadvalue =270;
  Right.outold =0;
  Right.outnew =0;
  Right.deadvalue =270;
}
/* PID控制初始化 */
typedef struct PID
{
  float Kp;                                                                     //比例常数
  float Ki;                                                                     //积分常数
  float Kd;                                                                     //微分常数
  float SetfDelta;
  float SetData;
  float SetmData;
  float SetPoint;                                                               //设定目标
  float ActPoint;                                                               //实际目标
  float ActNewPoint;
  float SumError;                                                               //e[k] / 误差累计
  float LastError;                                                              //e[k-1]
  float PrevError;                                                              //e[k-2]
  float max;
  float min;
  float out;
  float outold;
  float outnew;
  float getpid;
  int start;
}PID;
PID angle,speed,direction;
PID *Angle =&angle;
PID *Speed =&speed;
PID *Direction =&direction;                                                     //取址符运算符“ & ”

void anglecontrolpid_init()
{
  Angle->Kp =425;
  Angle->Ki =0.0;
  Angle->Kd =24;
  Angle->out =0.0;
}
void speedcontrolpid_init()
{
  Speed->Kp =2.7;//1.3/3.3//8.7大了
  Speed->Ki =0.2;
  Speed->Kd =0.05;
  Speed->SetPoint =0.0;
  Speed->SetData=2700;
  Speed->SetmData =1000;
  Speed->SetfDelta =0;
  Speed->ActPoint =0.0;
  Speed->ActNewPoint =0.0;
  Speed->SumError =0.0;
  Speed->LastError =0.0;
  Speed->PrevError =0.0;
  Speed->max =400;// 200|100|50
  Speed->min =-400;// 200|100|50
  Speed->out =0.0;
  Speed->outold =0.0;
  Speed->outnew =0.0;
  Speed->start =0;
}
void directioncontrolpid_init()
{
  Direction->Kp =400.0;
  Direction->Ki =0.0;
  Direction->Kd =14.0;
  Direction->SetPoint =79;
  Direction->ActPoint =79;
  Direction->SumError =0.0;
  Direction->LastError =0.0;
  Direction->PrevError =0.0;
  Direction->max =50;
  Direction->min =-50;  
  Direction->out =0.0;
  Direction->outold =0.0;
  Direction->outnew =0.0;
  Direction->getpid =0.0;
  Direction->start =0;
}
/* 初始化 */
void Struct_init()
{
  key_init();
  adget_init();
  filter_init();
  speedget_init();
  motorcontrol_init();
  anglecontrolpid_init(); 
  speedcontrolpid_init();
  directioncontrolpid_init();
}

#endif