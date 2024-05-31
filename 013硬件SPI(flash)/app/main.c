/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * 

 Change Logs:
 * Date           Author       Notes
 * 2024-03-11     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>
#include "spi_flash.h"

int main(void)
{
	
	board_init();
	
	uart1_init(115200U);
	
	unsigned char buff[20] = {0};

	/* SPI初始化 */
	bsp_spi_init();
			
	//获取GD25Q32的设备ID
	printf("ID = %X\r\n",GD25Q32_readID());
			
	//读取0地址长度为7个字节的数据到buff
	GD25Q32_read(buff, 0, 10);
	//输出读取到的数据
	printf("buff: %s\r\n",buff);
	delay_ms(200);
	//往0地址写入7个字节的数据 “嘉立创”
	GD25Q32_write((uint8_t *)"嘉立创", 0, 10);

	// 等待写入完成
	delay_ms(200);

	//读取0地址长度为7个字节的数据到buff
	GD25Q32_read(buff, 0, 10);
	//输出读取到的数据
	printf("buff: %s\r\n",buff);
			
	delay_ms(1000);
			
	unsigned int t = 0;
	while(1) 
	{
		t %= 15;
		GD25Q32_erase_sector(t++);
					
	}
	

}
