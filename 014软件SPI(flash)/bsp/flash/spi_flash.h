/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * 

 Change Logs:
 * Date           Author       Notes
 * 2024-03-11     LCKFB-LP    first version
 */
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include "stm32f4xx.h"

#define BSP_GPIO_RCU			RCC_AHB1Periph_GPIOA // GPIOʱ��
#define BSP_GPIO_PORT			GPIOA	// �˿�

#define BSP_SPI_NSS				GPIO_Pin_4 // CS
#define BSP_SPI_SCK				GPIO_Pin_5 // SCK
#define BSP_SPI_MISO			GPIO_Pin_6 // MISO
#define BSP_SPI_MOSI			GPIO_Pin_7 // MOSI


#define W25QXX_SCK_ON(x)		GPIO_WriteBit(BSP_GPIO_PORT, BSP_SPI_SCK,  x ? Bit_SET : Bit_RESET)
#define W25QXX_MOSI_ON(x)		GPIO_WriteBit(BSP_GPIO_PORT, BSP_SPI_MOSI, x ? Bit_SET : Bit_RESET)
#define W25QXX_CS_ON(x)			GPIO_WriteBit(BSP_GPIO_PORT, BSP_SPI_NSS,  x ? Bit_SET : Bit_RESET)

#define W25QXX_MISO_ON()		GPIO_ReadInputDataBit(BSP_GPIO_PORT, BSP_SPI_MISO)

void bsp_spi_init(void);
uint8_t spi_read_write_byte(uint8_t dat);
uint16_t GD25Q32_readID(void);
void GD25Q32_write_enable(void);
void GD25Q32_wait_busy(void);
void GD25Q32_erase_sector(uint32_t addr);
void GD25Q32_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte);
void GD25Q32_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length);



#endif
