/*
 * ����Ӳ��ƽ̨:  LPLD_K60 Card
 * ��Ȩ����:      �����������µ��Ӽ������޹�˾
 * ��������:      http://laplenden.taobao.com
 * ��˾�Ż�:      http://www.lpld.cn
 *
 * ˵��:    �����̻���Kintis K60��Դ�ײ㿪����������
 *          ���п�Դ�����������"LPLD"�ļ����£�����˵�����ĵ�[#LPLD-003-N]
 *
 * �ļ���:  isr.h
 * ��;:    �����жϷ����ӳ��򣬸�ͷ�ļ������������жϺ�����Ϊ�ײ��жϺ�����
 *	    �����ż���������ο��ĵ�[#LPLD-003-N]������ײ�ģ��".h"ͷ�ļ���
 * ע��:   ��ͷ�ļ�ֻ�ܱ�"vectors.c"��������
 *
 */


#ifndef __ISR_H
#define __ISR_H 1


/* �����ײ��жϷ����ӳ��� */

//DMAģ���жϷ�����
#undef  VECTOR_017
#define VECTOR_017 LPLD_DMA_Isr
extern void LPLD_DMA_Isr(void);

//GPIOģ���жϷ�����
#undef  VECTOR_103
#define VECTOR_103 LPLD_GPIO_Isr
#undef  VECTOR_104
#define VECTOR_104 LPLD_GPIO_Isr
#undef  VECTOR_105
#define VECTOR_105 LPLD_GPIO_Isr
#undef  VECTOR_106
#define VECTOR_106 LPLD_GPIO_Isr
#undef  VECTOR_107
#define VECTOR_107 LPLD_GPIO_Isr
extern void LPLD_GPIO_Isr(void);

//PITģ���жϷ�����
#undef  VECTOR_084
#define VECTOR_084 LPLD_PIT_Isr
extern void LPLD_PIT_Isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
