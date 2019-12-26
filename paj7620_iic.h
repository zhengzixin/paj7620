/**
 *  @file 次文件夹为paj7620 iic 的对外文件，包含了所有关于iic 硬件接口的 操作
 * 
*/

#ifndef  _PAJ7620_IIC_H
#define _PAJ7620_IIC_H

#ifdef __cplusplus
    extern "C"{
#endif

#include "paj7620_cfg.h"

struct paj7620_iic_t
{
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
    extern }
#endif

#endif

