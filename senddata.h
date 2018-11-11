/*
 * Send.h 
 */

#ifndef __SEND_DATA_H__
#define __SEND_DATA_H__ 

#include <math.h>


extern float OutData[4];
float OutData[4] = { 0 };

unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)             //CRC校检
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp =0xffff;

    for(i =0;i <CRC_CNT;i ++)
    {      
        CRC_Temp ^=Buf[i];
        for(j =0;j <8;j ++) 
        {
            if(CRC_Temp &0x01)CRC_Temp =(CRC_Temp >>1) ^0xa001;
            else CRC_Temp =CRC_Temp >>1;
        }
    }
    return(CRC_Temp);
}  

void OutPut_Data(void)
{
  int temp[4] ={0};
  unsigned int temp1[4] ={0};
  unsigned char databuf[10] ={0};
  unsigned char i;
  unsigned short CRC16 =0;
  for(i =0;i <4;i ++)
  {    
    temp[i]  =(int)OutData[i];
    temp1[i] =(unsigned int)temp[i];    
  }
   
  for(i =0;i <4;i ++) 
  {
    databuf[i*2] = (unsigned char)(temp1[i]%256);                               //每个通道的数据是两个字节,四个通道就是8个字节
    databuf[i*2 +1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 =CRC_CHECK(databuf,8);
  databuf[8] =CRC16%256;                                                        //两个字节的CRC校检
  databuf[9] =CRC16/256;
  
  for(i=0;i<10;i++)
    LPLD_UART_PutChar(UART0,databuf[i]);
} 

void Send_Outdata()
{
  float temp =0.0;
  for(temp = 0; temp <= 3.14*2; temp += 0.1)  
  {
    OutData[0] = sin(temp)*(float)1000;
    OutData[1] = sin(temp-3.14/4)*(float)1000;
    OutData[2] = sin(temp+3.14/4)*(float)1000;
    OutData[3] = sin(temp+3.14/2)*(float)1000;
    OutPut_Data();                
  }  
}



#endif
