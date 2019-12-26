/**
 * 
 * @file 与 paj7620功能有关的操作函数
 * 
 * @author zhengchuynu 
 * 
*/

#ifndef      _PAJ7620_DEV_H
#define    _PAJ7620_DEV_H

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
   extern  }
    #endif


#endif /    / ! _PAJ7620_DEV_H