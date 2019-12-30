/**
 *  @file  此文件包含paj7620 的主要驱动程序 
 * 
 * @author ait_zhengchunyu@163.com
 * 
 * @brief  移植时需要修改my_delay_ms(uint32_t nms)，与当前芯片匹配
 *  
*/
#include "paj7620_dev.h"
#include "paj7620_cfg.h"

struct paj7620_dev_t paj7602_dev ;

static void my_delay_ms(uint32_t nms)
{
  HAL_Delay(nms);
}
/**
 *  @brief  选择PAJ7620U2 BANK区域
 * 
*/
static void paj7620_select_bank(bank_e bank)
{
	switch(bank)
	{

        case BANK0:paj7602_dev.iicdev->write_reg_byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
        case BANK1:paj7602_dev.iicdev->write_reg_byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域

	}
			
}

/**
 *  @brief  PAJ7620 唤醒
 * 
 * @return  0 唤醒成功  1唤醒失败
 * 
*/
static uint8_t  paj7620_device_wakeup(void)
{
    uint8_t data=0x0a;
	paj7602_dev.iicdev->wakeup();//唤醒PAJ7620U2
	my_delay_ms(5);//唤醒时间>400us
	paj7602_dev.iicdev->wakeup();//唤醒PAJ7620U2
	my_delay_ms(5);//唤醒时间>400us
	paj7620_select_bank(BANK0);//进入BANK0寄存器区域
	data = paj7602_dev.iicdev->read_reg_byte(0x00);//读取状态
	if(data!=0x20) return 1; //唤醒失败
	return 0;
}
/**
 *  
 * @brief 手势检测初始化
 * 
*/

static void paj7620_gesture_init(void)
{
    int i;
    paj7620_select_bank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<GESTURE_SIZE;i++)
	{
		paj7602_dev.iicdev->write_reg_byte(gesture_cfg_buf[i][0],gesture_cfg_buf[i][1]);//手势识别模式初始化
	}
	paj7620_select_bank(BANK0);//切换回BANK0寄存器区域

}


/**
 * 
 *  @brief paj7260 读取手势 
 * 
 * @return 0 无手势操作  1 上 2 下 3左 4 右 5前 6 后 7 顺时针 8 逆时针 9 挥动
 * 
*/
uint8_t paj7260_read_gesture(void)
{
    uint8_t ret = 0;
    uint8_t status;
	uint8_t data[2]={0x00};
	uint16_t gesture_data;
    status = paj7602_dev.iicdev->read_reg_buf(PAJ_GET_INT_FLAG1,&data[0],2);//读取手势状态			
	if(!status)
	{   
			gesture_data =(uint16_t)data[1]<<8 | data[0];
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:   
                        ret = 1;
                    break;
                    case GES_DOWM:   
                        ret = 2;
                    break;
					case GES_LEFT:
                        ret = 3;
                    break;
					case GES_RIGHT:   
                        ret = 4;
                    break;
					case GES_FORWARD: 
                        ret = 5;
                    break;
					case GES_BACKWARD:   
                        ret = 6;
                    break;
					case GES_CLOCKWISE: 
                        ret = 7;
                    break;
					case GES_COUNT_CLOCKWISE:  
                        ret = 8;
                    break;
					case GES_WAVE:  
                        ret = 9;
                    break;
					default: 
                        ret = 0;
                    break;
					
				}						
			}       
    }
    return ret;
}
/**
 * 
 * @brief paj7620 初始化 
 * 
 * @return  0 唤醒成功 1唤醒失败
*/
uint8_t paj7620_init(void)
{
    uint8_t i,status; 
    paj7602_dev.iicdev = &paj7620_iic;
    paj7602_dev.read_gesture =paj7260_read_gesture;
    paj7620_iic_init();

     status = paj7620_device_wakeup();//唤醒PAJ7620U2
	if(status) return 1;
	paj7620_select_bank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<INIT_SIZE;i++)
	{
         paj7602_dev.iicdev->write_reg_byte(init_cfg_buf[i][0],init_cfg_buf[i][1]);//初始化PAJ7620U2
	}
    paj7620_select_bank(BANK0);//切换回BANK0寄存器区域
    paj7620_gesture_init();
	return 0;
}
 