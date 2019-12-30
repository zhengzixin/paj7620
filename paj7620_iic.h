/**
 * 
 * @file 此文件是主要是配置iic相关函数，
 * 
 * @author ait_zhengchunyu@163.com
 * 
 * @brief  仅供paj7620_dev.c 调用，当移植时应修改 #include "stm32f7xx_hal.h" ,将其改成相应芯片的头文件
 * 
*/
#ifndef __PAJ7620_IIC_H
#define __PAJ7620_IIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"

struct paj7620_iic_t
{
	uint8_t dev_id;
		/**
	 * 
	 *  @向PAJ7620 指定地址写入一个数据
	 * 
	 * @param addr  PAJ7620 寄存器地址
	 * 
	 * @param val      要写入的值
	*/
	 uint8_t  (*write_reg_byte)(uint8_t  addr,uint8_t  val);
	/**
	 *  @brief 从PAJ7620 寄存器中的一个字节
	 * 
	 * @param addr 寄存器地址
	 * 
	 * @return 读取到的值  
	 * 
	*/
	 uint8_t (*read_reg_byte)(uint8_t addr);
	/**
	 *  @brief 读取 PAJ7620 寄存器中多个字节
	 * 
	 * @param addr 寄存器地址
	 * @param  buf 读取数组首地址
	 * @param  len  数组长度     
	 * 
	 * @return  0 读取成功  1读取失败
	*/
	 uint8_t (*read_reg_buf)(uint8_t addr,uint8_t *buf,uint8_t len);
	 void   (*wakeup)(void);
};

extern struct paj7620_iic_t  paj7620_iic ;

void paj7620_iic_init(void);

#ifdef __cplusplus
  }
#endif

#endif


