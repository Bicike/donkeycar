/*
 * ov7620.h �ɼ�����
 */

#ifndef __IMAGEDATA_H__
#define __IMAGEDATA_H__ 
/***************************��������*******************************************/
LPLD_eDMA_Cfg_t User_Def_DMA_Init;                                              //����DMA��ʼ���ṹ��
/************************OV7620��ʼ��******************************************/
void cmos_init(void);
void portb_isr(void);
void dma1_isr(void);
/***************************��������*******************************************/
#define Max_row 90
#define Max_column 320
uint8_t Pix_Data[Max_row][Max_column] ={0};
uint8_t chang_flag=0,hang_flag=0,hang_cai=0,send_phone=0;
/************************OV7620���ų�ʼ��**************************************/
void gpio2_init()
{
  LPLD_GPIO_Init(PTC, 8, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);                     //ѡ��IO E0~E7��Ϊ���ݿ�
  LPLD_GPIO_Init(PTC, 9, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);                     //���롢�ڲ������������ж�
  LPLD_GPIO_Init(PTC, 10, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 11, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 12, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 13, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 14, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
  LPLD_GPIO_Init(PTC, 15, DIR_INPUT, INPUT_PDOWN, IRQC_DIS);
                                                                                // ѡ��PTC0��Ϊ PCLK������
  LPLD_GPIO_Init(PTC, 0, DIR_INPUT, INPUT_PDOWN, IRQC_DMARI);                   // ���ó��������������ģʽ
                                                                                // DMA�����ش���  
                                                                                // ѡ��PTB0��Ϊ���ж�
  LPLD_GPIO_Init(PTB, 0, DIR_INPUT, INPUT_PDOWN, IRQC_RI);                      // ���ó��������������ģʽ 
                                                                                // IO�������ж�
                                                                                // ѡ��PTA5��Ϊ���ж�
  LPLD_GPIO_Init(PTB, 3, DIR_INPUT, INPUT_PDOWN, IRQC_RI);                      // ���ó��������������ģʽ
                                                                                // IO�������ж�
  LPLD_GPIO_SetIsr(PTB, portb_isr);                                             //�û��жϳ�����ڵ�ַ 
}
/************************DMA�жϳ�ʼ������*************************************/
void dma1_init()
{
  User_Def_DMA_Init.Channelx           =DMA_CH1;                                //����Ϊͨ��1
  User_Def_DMA_Init.Peri_DmaReq        =PORTC_DMAREQ;                           //IO C ����DMA����
  User_Def_DMA_Init.Minor_loop_Length  =Max_column;                             //DMA ��Ҫ�����ֽڵĸ���
  User_Def_DMA_Init.Source_Addr        =(uint32_t)&GPIOC_PDIR +1;                  //�������ݼĴ�����Դ��ַ ����Ϊ32λ
  User_Def_DMA_Init.Dest_Addr          =(uint32_t)Pix_Data;                     //����Ŀ�ĵ�ַΪPix_Data���׵�ַ
  User_Def_DMA_Init.Dma_irqc           =1;                                      //�ɼ����������ж�
  User_Def_DMA_Init.isr_func           =dma1_isr;                               //�û��жϳ�����ڵ�ַ
  LPLD_DMA_Init(&User_Def_DMA_Init);                                            //��ʼ��DMAģ��
} 
/************************�����ж���Ӧ����**************************************/
void portb_isr(void)
{
  unsigned char  C_delay=0;
  if(PORTB_ISFR & (1 << 3))
  { 
    PORTB_ISFR |= (1 << 3);
    LPLD_DMA_Init(&User_Def_DMA_Init);                                          //��ʼ��DMAģ��
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
/************************OV7620��ʼ��******************************************/
void cmos_init(void)
{
  gpio2_init();
  dma1_init();
}
#endif  

