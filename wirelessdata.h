#ifndef __WIRELESSDATA_H
#define __WIRELESSDATA_H 

uint8_t    MAX_RT=0,TX_RT=0,wuxian_ci=0;
uint8_t    TxBuf[32];
uint8_t Send_shuju =0;
/****************************************NRF24L01******************************/
#define  TX_ADR_WIDTH    5   	                                                // 5 uints TX address width
#define  RX_ADR_WIDTH    5   	                                                // 5 uints RX address width
#define  TX_PLOAD_WIDTH  32  	                                                // 20 uints TX payload
#define  RX_PLOAD_WIDTH  32  	                                                // 20 uints TX payload
uint8_t  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	                //本地地址
uint8_t  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x02};	                //接收地址
/*****************************NRF24L01寄存器指令*******************************/
#define READ_REG        0x00  	                                                // 读寄存器指令
#define WRITE_REG       0x20 	                                                // 写寄存器指令
#define RD_RX_PLOAD     0x61  	                                                // 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	                                                // 写待发数据指令
#define FLUSH_TX        0xE1 	                                                // 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	                                                // 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	                                                // 定义重复装载数据指令
#define NOP             0xFF  	                                                // 保留
/************************SPI(nRF24L01)寄存器地址*******************************/
#define CONFIG          0x00                                                    // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01                                                    // 自动应答功能设置
#define EN_RXADDR       0x02                                                    // 可用信道设置
#define SETUP_AW        0x03                                                    // 收发地址宽度设置
#define SETUP_RETR      0x04                                                    // 自动重发功能设置
#define RF_CH           0x05                                                    // 工作频率设置
#define RF_SETUP        0x06                                                    // 发射速率、功耗功能设置
#define STATUS          0x07                                                    // 状态寄存器
#define OBSERVE_TX      0x08                                                    // 发送监测功能
#define CD              0x09                                                    // 地址检测           
#define RX_ADDR_P0      0x0A                                                    // 频道0接收数据地址
#define RX_ADDR_P1      0x0B                                                    // 频道1接收数据地址
#define RX_ADDR_P2      0x0C                                                    // 频道2接收数据地址
#define RX_ADDR_P3      0x0D                                                    // 频道3接收数据地址
#define RX_ADDR_P4      0x0E                                                    // 频道4接收数据地址
#define RX_ADDR_P5      0x0F                                                    // 频道5接收数据地址
#define TX_ADDR         0x10                                                    // 发送地址寄存器
#define RX_PW_P0        0x11                                                    // 接收频道0接收数据长度
#define RX_PW_P1        0x12                                                    // 接收频道0接收数据长度
#define RX_PW_P2        0x13                                                    // 接收频道0接收数据长度
#define RX_PW_P3        0x14                                                    // 接收频道0接收数据长度
#define RX_PW_P4        0x15                                                    // 接收频道0接收数据长度
#define RX_PW_P5        0x16                                                    // 接收频道0接收数据长度
#define FIFO_STATUS     0x17                                                    // FIFO栈入栈出状态寄存器设置
/****************************函数定义******************************************/
void wuxian_gpio_init();
void porta_isr(void);
void NRF24L01_init(void);
uint16_t SPI_RW(uint16_t uuchar);
uint16_t SPI_RW_Reg(uint8_t reg, uint8_t value);
uint16_t SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char uchars);
uint16_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
void nRF24L01_TxPacket(uint8_t * tx_buf);
unsigned char nRF24L01_RxPacket(unsigned char *rx_buf);
void wireless_send_data();

void wuxian_gpio_init()
{
  LPLD_GPIO_Init(PTA,16,DIR_OUTPUT,OUTPUT_L,IRQC_DIS);                          //CE
  LPLD_GPIO_Init(PTA,14,DIR_OUTPUT,OUTPUT_L,IRQC_DIS);                          //CSN
  LPLD_GPIO_Init(PTA,17,DIR_OUTPUT,OUTPUT_L,IRQC_DIS);                          //SCK
  LPLD_GPIO_Init(PTA,19,DIR_OUTPUT,OUTPUT_L,IRQC_DIS);                          //MOSI
  LPLD_GPIO_Init(PTA,15,DIR_INPUT,OUTPUT_L,IRQC_DIS);                           //MISO
  LPLD_GPIO_Init(PTA, 9,DIR_INPUT,OUTPUT_H,IRQC_FA);                            //IRQ
  LPLD_GPIO_SetIsr (PTA, porta_isr);
}
void NRF24L01_init(void)
{
  wuxian_gpio_init();
  LPLD_GPIO_Set_b(PTA,16,OUTPUT_L);                                             //CE=0;    // chip enable
  LPLD_GPIO_Set_b(PTA,17,OUTPUT_L);                                             //SCK=0;   // Spi clock line init high
  udelay(5);
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);                 // 写本地地址	
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);              // 写接收端地址
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);                                          //频道0自动  ACK应答允许	
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);                                      //允许接收地址只有频道0，如果需要多频道可以参考Page21
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0xf0);                                     //自动重发，
  SPI_RW_Reg(WRITE_REG + RF_CH, 0);                                             //设置信道工作为2.4GHZ，收发必须一致
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f);                                       //设置发射速率为2MHZ，发射功率为最大值0dB
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);                                         //IRQ收发完成中断响应，16位CRC，主发送
}
uint16_t SPI_RW(uint16_t uuchar)
{
  uint16_t bit_ctr;
  LPLD_GPIO_Set_b(PTA,17,OUTPUT_L);                                             //SCK = 0;
  for(bit_ctr=0;bit_ctr<8;bit_ctr++)                                            //output 8-bit
  {
    LPLD_GPIO_Set_b(PTA,19, uuchar&0x80);                                       //MOSI = (uuchar & 0x80);// output 'uchar', MSB to MOSI
    uuchar = (uuchar << 1);                                                     // shift next bit into MSB..
    LPLD_GPIO_Set_b(PTA,17,OUTPUT_H);                                           //SCK = 1; // Set SCK high..
    uuchar |= LPLD_GPIO_Get_b(PTA,15);                                          //uuchar |= MISO;// capture current MISO bit
    LPLD_GPIO_Set_b(PTA,17,OUTPUT_L);                                           //SCK = 0;// ..then set SCK low again
  }
  return(uuchar);           		                                        // return read uchar
}
uint16_t SPI_RW_Reg(uint8_t reg, uint8_t value)
{
  uint16_t status;
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_L);                                             //CSN = 0;// CSN low, init SPI transaction
  status = SPI_RW(reg);                                                         // select register
  SPI_RW(value);                                                                // ..and write value to it..
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_H);                                             //CSN = 1;// CSN high again
  MAX_RT = (status & 0x10);TX_RT = (status & 0x20); 
  return(status);                                                               //return nRF24L01 status uchar
}
uint16_t SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char uchars)
{
  uint16_t status,uchar_ctr;
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_L);                                             //CSN = 0;  // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);       		                                        // Select register to write to and read status uchar
  for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
  pBuf[uchar_ctr] = SPI_RW(0);      
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_H);                                             //CSN = 1;                           
  return(status);                                                               // return nRF24L01 status uchar
}
uint16_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
  uint16_t status,uchar_ctr;
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_L);                                             //CSN = 0;//SPI使能       
  status = SPI_RW(reg);
  for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) 
  SPI_RW(*pBuf++);
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_H);                                             //CSN = 1;//关闭SPI
  return(status);    
}
unsigned char nRF24L01_RxPacket(unsigned char *rx_buf)
{
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_L);                                             //CSN = 0;  //SPI使能
  SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); udelay(0);                   // read receive payload from RX_FIFO buffer
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_L);                                             //CSN = 0;
  return(0);
}
void nRF24L01_TxPacket(uint8_t * tx_buf)                                        //发送数据函数
{
  LPLD_GPIO_Set_b(PTA,16,OUTPUT_L);                                             //CE=0;//StandBy I模式
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);              //装载接收端地址
  SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 		                //装载数据	
  LPLD_GPIO_Set_b(PTA,16,OUTPUT_H);                                             //CE=1 置高CE，激发数据发送
}
void porta_isr()
{
  disable_irq(87);
  SPI_RW_Reg(WRITE_REG+STATUS,0X7c);
  PORTA_ISFR = 0x0000200;
  wuxian_ci ++ ;
}

void wireless_send_data()
{    
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zuo[Send_shuju];    //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==1));                     //等待数据发送完成
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zuo[Send_shuju+32];  //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==2));                     //等待数据发送完成
         
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_you[Send_shuju];     //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==3));                     //等待数据发送完成 
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_you[Send_shuju+32];  //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号
  while(!(wuxian_ci==4));                     //等待数据发送完成 
      
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zhongxin[Send_shuju];    //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==5));                     //等待数据发送完成
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zhongxin[Send_shuju+32];  //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==6));                     //等待数据发送完成     
  
  /*TxBuf[Send_shuju]=((uint8_t)(1400-Speed->SetPoint));                                                    //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf);enable_irq (87);                                   //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==1));  
  
  TxBuf[Send_shuju]=((uint8_t)(Speed->ActPoint-1425));                                                    //将数据放入无线发送缓冲区
  nRF24L01_TxPacket(TxBuf);enable_irq (87);                                   //将数据放入缓冲区并开启中断回应信号 
  while(!(wuxian_ci==1));*/                                                     //等待数据发送完成        

}

#endif
