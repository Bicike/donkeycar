/*
 * ov7620.h 采集函数
 */

#ifndef __IMAGEDATA_H__
#define __IMAGEDATA_H__ 
/***************************函数声明*******************************************/
LPLD_eDMA_Cfg_t User_Def_DMA_Init;                                              //声明DMA初始化结构体
/************************OV7620初始化******************************************/
void cmos_init(void);
void portb_isr(void);
void dma1_isr(void);
/***************************变量声明*******************************************/
#define Max_row 90
#define Max_column 320
uint8_t Pix_Data[Max_row][Max_column] ={0};
uint8_t chang_flag=0,hang_flag=0,hang_cai=0,send_phone=0;
/************************OV7620引脚初始化**************************************/
void gpio2_init()
{
  LPLD_GPIO_Init(PTC, 8, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);                     //选择IO E0~E7作为数据口
  LPLD_GPIO_Init(PTC, 9, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);                     //输入、内部下拉、禁用中断
  LPLD_GPIO_Init(PTC, 10, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 11, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 12, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 13, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 14, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 15, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
                                                                                // 选择PTC0作为 PCLK触发端
  LPLD_GPIO_Init(PTC, 0, DIR_INPUT, INPUT_PDOWN, IRQC_DMARI);                   // 配置成输入下拉电阻的模式
                                                                                // DMA上升沿触发  
                                                                                // 选择PTB0作为行中断
  LPLD_GPIO_Init(PTB, 0, DIR_INPUT, INPUT_PDOWN, IRQC_RI);                      // 配置成输入下拉电阻的模式 
                                                                                // IO上升沿中断
                                                                                // 选择PTA5作为场中断
  LPLD_GPIO_Init(PTB, 3, DIR_INPUT, INPUT_PDOWN, IRQC_RI);                      // 配置成输入下拉电阻的模式
                                                                                // IO上升沿中断
  LPLD_GPIO_SetIsr(PTB, portb_isr);                                             //用户中断程序入口地址 
}
/************************DMA中断初始化函数*************************************/
void dma1_init()
{
  User_Def_DMA_Init.Channelx           =DMA_CH1;                                //设置为通道1
  User_Def_DMA_Init.Peri_DmaReq        =PORTC_DMAREQ;                           //IO C 产生DMA请求
  User_Def_DMA_Init.Minor_loop_Length  =Max_column;                             //DMA 需要传输字节的个数
  User_Def_DMA_Init.Source_Addr        =(uint32_t)&GPIOC_PDIR +1;                  //设置数据寄存器的源地址 长度为32位
  User_Def_DMA_Init.Dest_Addr          =(uint32_t)Pix_Data;                     //设置目的地址为Pix_Data的首地址
  User_Def_DMA_Init.Dma_irqc           =1;                                      //采集结束触发中断
  User_Def_DMA_Init.isr_func           =dma1_isr;                               //用户中断程序入口地址
  LPLD_DMA_Init(&User_Def_DMA_Init);                                            //初始化DMA模块
} 
/************************场行中断响应函数**************************************/
void portb_isr(void)
{
  unsigned char  C_delay=0;
  if(PORTB_ISFR & (1 << 3))
  { 
    PORTB_ISFR |= (1 << 3);
    LPLD_DMA_Init(&User_Def_DMA_Init);                                          //初始化DMA模块
    chang_flag =1;
    hang_cai =0;
    hang_flag =0;
  }
  
  if(PORTB_ISFR & chang_flag & (1 << 0))
  {
    PORTB_ISFR |= (1 << 0);
    if(chang_flag ==0)
    {
      return;
    }
    for(C_delay=0;C_delay<195;C_delay++) asm("nop");
    hang_cai++;
    LPLD_DMA_EnableReq(1,1);                                                     
  }
}
void dma1_isr(void)
{
  if(hang_cai ==Max_row)
  {
    PORTB_ISFR =0x0000009;     
    chang_flag =0;
    send_phone =1;                                                          
  }
}
/************************OV7620初始化******************************************/
void cmos_init(void)
{
  gpio2_init();
  dma1_init();
}
#endif  

