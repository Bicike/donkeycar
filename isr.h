/*
 * 测试硬件平台:  LPLD_K60 Card
 * 版权所有:      北京拉普兰德电子技术有限公司
 * 网络销售:      http://laplenden.taobao.com
 * 公司门户:      http://www.lpld.cn
 *
 * 说明:    本工程基于Kintis K60开源底层开发包开发，
 *          所有开源驱动代码均在"LPLD"文件夹下，调用说明见文档[#LPLD-003-N]
 *
 * 文件名:  isr.h
 * 用途:    声明中断服务子程序，该头文件中所声明的中断函数均为底层中断函数，
 *	    向量号及函数名请参考文档[#LPLD-003-N]，或各底层模块".h"头文件。
 * 注意:   该头文件只能被"vectors.c"所包含。
 *
 */


#ifndef __ISR_H
#define __ISR_H 1


/* 声明底层中断服务子程序 */

//DMA模块中断服务定义
#undef  VECTOR_017
#define VECTOR_017 LPLD_DMA_Isr
extern void LPLD_DMA_Isr(void);

//GPIO模块中断服务定义
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

//PIT模块中断服务定义
#undef  VECTOR_084
#define VECTOR_084 LPLD_PIT_Isr
extern void LPLD_PIT_Isr(void);


#endif  //__ISR_H

/* End of "isr.h" */
