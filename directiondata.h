/*
 * directiondata.h
 */
#ifndef _DIRECTIONDATA_H
#define _DIRECTIONDATA_H

#define ZHONGXIN 80
#define ZHONGXIN1 5
#define MAICHONG_K 583  //脉冲个数
#define CHETAI_r 0.032  //车胎半径
#define CHE_KUAN 0.18     //车宽

#include "main.h"

int set_speed,act_speed;

void directionerror_get()
{
  Direction->ActPoint =(Black_zhongxin[ZHONGXIN]*10 
                      +Black_zhongxin[ZHONGXIN +1] +Black_zhongxin[ZHONGXIN +2] +Black_zhongxin[ZHONGXIN +3] +Black_zhongxin[ZHONGXIN +4] +Black_zhongxin[ZHONGXIN +5] 
                      +Black_zhongxin[ZHONGXIN +6] +Black_zhongxin[ZHONGXIN +7] +Black_zhongxin[ZHONGXIN +8] +Black_zhongxin[ZHONGXIN +9] +Black_zhongxin[ZHONGXIN +10]
                      +Black_zhongxin[ZHONGXIN -9] +Black_zhongxin[ZHONGXIN -8] +Black_zhongxin[ZHONGXIN -7] +Black_zhongxin[ZHONGXIN -6] +Black_zhongxin[ZHONGXIN -5] 
                      +Black_zhongxin[ZHONGXIN -4] +Black_zhongxin[ZHONGXIN -3] +Black_zhongxin[ZHONGXIN -2] +Black_zhongxin[ZHONGXIN -1] +Black_zhongxin[ZHONGXIN -10])/30;
}
void directionerror_get1()
{
  Direction->ActPoint =(Black_zhongxin[ZHONGXIN1]*10 
                      +Black_zhongxin[ZHONGXIN1 +1] +Black_zhongxin[ZHONGXIN1 +2] +Black_zhongxin[ZHONGXIN1 +3] +Black_zhongxin[ZHONGXIN1 +4] +Black_zhongxin[ZHONGXIN1 +5]                      
                      +Black_zhongxin[ZHONGXIN1 -1] +Black_zhongxin[ZHONGXIN1 -2] +Black_zhongxin[ZHONGXIN1 -3] +Black_zhongxin[ZHONGXIN1 -4] +Black_zhongxin[ZHONGXIN1 -5] )/20;
                      
}







































#endif 