/**
 * 
 * @file 包含了paj7620 操作的相关函数
 * 
 * @author ait_zhengchunyu@163.com
 * 
 * @brief 在使用中应调用paj7620_init()进行设备初始化
 * 
*/
#ifndef __PAJ7620_DEV_H
#define __PAJ7620_DEV_H


#ifdef __cplusplus
extern "C" {
#endif


#include "paj7620_iic.h"

struct paj7620_dev_t
{
    struct paj7620_iic_t *iicdev;

    /**
 * 
 *  @brief paj7260 读取手势 
 * 
 * @return 0 无手势操作  1 上 2 下 3左 4 右 5前 6 后 7 顺时针 8 逆时针 9 挥动
 * 
*/
uint8_t (*read_gesture)(void);
};

extern  struct paj7620_dev_t paj7602_dev ;

/**
 * 
 * @brief paj7620 初始化 
 * 
 * @return  0 唤醒成功 1唤醒失败
*/
uint8_t paj7620_init(void);



#ifdef __cplusplus
  }
#endif


#endif

