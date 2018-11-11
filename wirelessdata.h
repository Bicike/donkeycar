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
uint8_t  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	                //���ص�ַ
uint8_t  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x02};	                //���յ�ַ
/*****************************NRF24L01�Ĵ���ָ��*******************************/
#define READ_REG        0x00  	                                                // ���Ĵ���ָ��
#define WRITE_REG       0x20 	                                                // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	                                                // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	                                                // д��������ָ��
#define FLUSH_TX        0xE1 	                                                // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	                                                // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	                                                // �����ظ�װ������ָ��
#define NOP             0xFF  	                                                // ����
/************************SPI(nRF24L01)�Ĵ�����ַ*******************************/
#define CONFIG          0x00                                                    // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01                                                    // �Զ�Ӧ��������
#define EN_RXADDR       0x02                                                    // �����ŵ�����
#define SETUP_AW        0x03                                                    // �շ���ַ�������
#define SETUP_RETR      0x04                                                    // �Զ��ط���������
#define RF_CH           0x05                                                    // ����Ƶ������
#define RF_SETUP        0x06                                                    // �������ʡ����Ĺ�������
#define STATUS          0x07                                                    // ״̬�Ĵ���
#define OBSERVE_TX      0x08                                                    // ���ͼ�⹦��
#define CD              0x09                                                    // ��ַ���           
#define RX_ADDR_P0      0x0A                                                    // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B                                                    // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C                                                    // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D                                                    // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E                                                    // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F                                                    // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10                                                    // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11                                                    // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12                                                    // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13                                                    // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14                                                    // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15                                                    // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16                                                    // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17                                                    // FIFOջ��ջ��״̬�Ĵ�������
/****************************��������******************************************/
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
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);                 // д���ص�ַ	
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);              // д���ն˵�ַ
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);                                          //Ƶ��0�Զ�  ACKӦ������	
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);                                      //������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0xf0);                                     //�Զ��ط���
  SPI_RW_Reg(WRITE_REG + RF_CH, 0);                                             //�����ŵ�����Ϊ2.4GHZ���շ�����һ��
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f);                                       //���÷�������Ϊ2MHZ�����书��Ϊ���ֵ0dB
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);                                         //IRQ�շ�����ж���Ӧ��16λCRC��������
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
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_L);                                             //CSN = 0;//SPIʹ��       
  status = SPI_RW(reg);
  for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) 
  SPI_RW(*pBuf++);
  LPLD_GPIO_Set_b(PTA,14,OUTPUT_H);                                             //CSN = 1;//�ر�SPI
  return(status);    
}
unsigned char nRF24L01_RxPacket(unsigned char *rx_buf)
{
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_L);                                             //CSN = 0;  //SPIʹ��
  SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); udelay(0);                   // read receive payload from RX_FIFO buffer
  LPLD_GPIO_Set_b(PTA,27,OUTPUT_L);                                             //CSN = 0;
  return(0);
}
void nRF24L01_TxPacket(uint8_t * tx_buf)                                        //�������ݺ���
{
  LPLD_GPIO_Set_b(PTA,16,OUTPUT_L);                                             //CE=0;//StandBy Iģʽ
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);              //װ�ؽ��ն˵�ַ
  SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 		                //װ������	
  LPLD_GPIO_Set_b(PTA,16,OUTPUT_H);                                             //CE=1 �ø�CE���������ݷ���
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
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zuo[Send_shuju];    //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==1));                     //�ȴ����ݷ������
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zuo[Send_shuju+32];  //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==2));                     //�ȴ����ݷ������
         
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_you[Send_shuju];     //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==3));                     //�ȴ����ݷ������ 
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_you[Send_shuju+32];  //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź�
  while(!(wuxian_ci==4));                     //�ȴ����ݷ������ 
      
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zhongxin[Send_shuju];    //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==5));                     //�ȴ����ݷ������
  for(Send_shuju=0;Send_shuju<32;Send_shuju++)TxBuf[Send_shuju]=Black_zhongxin[Send_shuju+32];  //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf); enable_irq (87);  //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==6));                     //�ȴ����ݷ������     
  
  /*TxBuf[Send_shuju]=((uint8_t)(1400-Speed->SetPoint));                                                    //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf);enable_irq (87);                                   //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==1));  
  
  TxBuf[Send_shuju]=((uint8_t)(Speed->ActPoint-1425));                                                    //�����ݷ������߷��ͻ�����
  nRF24L01_TxPacket(TxBuf);enable_irq (87);                                   //�����ݷ��뻺�����������жϻ�Ӧ�ź� 
  while(!(wuxian_ci==1));*/                                                     //�ȴ����ݷ������        

}

#endif
