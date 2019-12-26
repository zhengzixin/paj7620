#include "paj7620_iic.h"



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

struct paj7620_iic_t paj7620_iic;

static void iic_board_iniit(void)
{
}
static  void my_delay_us(int nus)
{
}
/**
 * @brief 配置sda 引脚方向
 *
 * @param val ：1 输出 OUT 0 输入 IN
 *
 * */
static void _config_sda_dir(uint8_t val)
{
}
/**
 * 
 * @brief sda 输出电平信号
 *
 * @param val :1 输出高电平  0 低电平
 *
 * */
static void _sda_out(uint8_t val)
{
}

/**
 *
 * @brief scl 输出电平信号
 *
 * @param val 1：输出高电平 0 低电平
 *
 * */
static void _scl_out(uint8_t val)
{
	
}
/**
 *  @brief 返回当前sda 电平状态
 * 
*/
static uint8_t _sda_state(void)
{
	
}
/**
 * @brief iic 起始信号
 *
 * */
static  void my_iic_start(void)
{
	_config_sda_dir(1);//sda线输出
	_sda_out(1);	  	  
	_scl_out(1);
	my_delay_us(4);
 	_sda_out(0);//START:when CLK is high,DATA change form high to low 
	my_delay_us(4);
	_scl_out(0);//钳住I2C总线，准备发送或接收数据 
}
/**
 *
 * @brief iic 停止信号
 *
 * */
static void my_iic_stop(void)
{	
	_config_sda_dir(1);
	_scl_out(0);
	_sda_out(0);//STOP:when CLK is high DATA change form low to high
 	my_delay_us(4);
	_scl_out(1); 
	_sda_out(1);//发送I2C总线结束信号
	my_delay_us(4);		
}
/**
 * 
 * @brief  iic 等待从机回复ack信号
 * 
 * @return 0 应答成功 1 应答失败
 *
 * */
static uint8_t  my_iic_wait_ack(void)
{
	uint8_t err_count=0;
	_config_sda_dir(0);

	_sda_out(1);
	delay_us(3);	   
	_scl_out(1);
	delay_us(3);	 
	while(_sda_state())
	{
		err_count++;
		if(err_count>250)
		{
			my_iic_stop();
			return 1;
		}
	}
	_scl_out(0);//时钟输出0 	   
	return 0;  
}

/**
 *
 * @biref iic 应答信号
 *
 * */
static void my_iic_ack(void)
{
	_scl_out(0);
	_config_sda_dir(1);
	_sda_out(0);
	delay_us(3);
	_scl_out(1);
	delay_us(3);
	_scl_out(0);
}
/**
 *
 * @brief  iic 非应答信号
 * */
static void my_iic_no_ack(void)
{
		_scl_out(0);
		_config_sda_dir(1);
		_sda_out(1);
		delay_us(2);
		_scl_out(1);
		delay_us(2);
		_scl_out(0);
}

/**
 *  @brief iic 向从机发送一个字节
 * 
 * @ param  val 字符内容
 *   
 * @return  
*/
static void my_iic_write_byte(uint8_t  val)
{                        
     uint8_t t;
	_config_sda_dir(1); 	    
    _scl_out(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((val&0x80)>>7)
			_sda_out(1);
		else
			_sda_out(0);
		val<<=1; 	  
		my_delay_us(5);  
		_scl_out(1);
		my_delay_us(5); 
		_scl_out(0);	
		my_delay_us(5);
    }	 
} 
/**
 * 
 *  @brief  读取一个字节 
 * 
 * @param ack  1 读取完成后发送一个ack  0 读取完成后发送一个noack
 * 
 * @return 返回读取到的字节
 *
 * */
uint8_t my_iic_read_byte(uint8_t ack)
{
		uint8_t  i,receive=0;
		_config_sda_dir(0);//SDA设置为输入
		for(i=0;i<8;i++ )
		{
			_scl_out(0); 
			my_delay_us(4);
	  		_scl_out(1);
			receive<<=1;
			if(_sda_state)receive++;   
	  		my_delay_us(4); 
	}					 
	if (!ack)
		my_iic_no_ack();//发送nACK
	else
		my_iic_ack(); //发送ACK   
	return receive;
}

/**
 * 
 *  @向PAJ7620 指定地址写入一个数据
 * 
 * @param addr  PAJ7620 寄存器地址
 * 
 * @param val      要写入的值
*/
static uint8_t  iic_write_reg_byte(uint8_t  addr,uint8_t  val)
{
	my_iic_start();
	my_iic_write_byte(PAJ7620_ID);
	if(my_iic_wait_ack())
	{
		my_iic_stop();//释放总线
		return 1;//没应答则退出
	}
	my_iic_write_byte(addr);
	my_iic_wait_ack();	
	my_iic_write_byte(val);
	my_iic_wait_ack();	
	my_iic_stop();

	return 0;
}

/**
 *  @brief 从PAJ7620 寄存器中的一个字节
 * 
 * @param addr 寄存器地址
 * 
 * @return 读取到的值  
 * 
*/
static uint8_t iic_read_reg_byte(uint8_t addr)
{
	uint8_t reg_val;
	
	my_iic_start();
	my_iic_write_byte(PAJ7620_ID);//发写命令
	if(my_iic_wait_ack())
	{
		 my_iic_stop();//释放总线
		 return 0;//没应答则退出
	}		
	my_iic_write_byte(addr);
	my_iic_wait_ack();
	my_iic_start(); 
	my_iic_write_byte(PAJ7620_ID|0x01);//发读命令
	my_iic_wait_ack();
	reg_val = my_iic_read_byte(0);
	my_iic_stop();
	return reg_val;
}


/**
 *  @brief 读取 PAJ7620 寄存器中多个字节
 * 
 * @param addr 寄存器地址
 * @param  buf 读取数组首地址
 * @param  len  数组长度     
 * 
 * @return  0 读取成功  1读取失败
*/
static uint8_t iic_read_reg_buf(uint8_t addr,uint8_t *buf,uint8_t len)
{
	my_iic_start();
	my_iic_write_byte(PAJ7620_ID);//发写命令
	if(my_iic_wait_ack()) 
	{
		my_iic_stop();//释放总线
		return 1;//没应答则退出
	}
	my_iic_write_byte(addr);
	my_iic_wait_ack();

	my_iic_start();
	my_iic_write_byte(PAJ7620_ID|0x01);//发读命令
	my_iic_wait_ack();
	while(len)
	{
		if(len==1)
		{
			*buf = my_iic_read_byte(0);
		}
		else
		{
			*buf = my_iic_read_byte(1);
		}
		buf++;
		len--;
	}
	my_iic_stop();//释放总线

	return 0;
	
}
static void my_wakeup_dev(void)
{
	my_iic_start();
	my_iic_write_byte(PAJ7620_ID);
	my_iic_stop();	
}
void paj7620_iic_init(void)
{
	iic_board_iniit();
	paj7620_iic.read_reg_buf   = iic_read_reg_buf ;
	paj7620_iic.read_reg_byte =	iic_read_reg_byte;
	paj7620_iic.write_reg_byte= iic_write_reg_byte;
	paj7620_iic.wakeup =my_wakeup_dev;
}