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
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>
#include "spi_flash.h"

int main(void)
{
	
	board_init();
	
	uart1_init(115200U);
	
	unsigned char buff[20] = {0};

	/* SPI��ʼ�� */
	bsp_spi_init();
			
	//��ȡGD25Q32���豸ID
	printf("ID = %X\r\n",GD25Q32_readID());
			
	//��ȡ0��ַ����Ϊ7���ֽڵ����ݵ�buff
	GD25Q32_read(buff, 0, 10);
	//�����ȡ��������
	printf("buff: %s\r\n",buff);
	delay_ms(200);
	//��0��ַд��7���ֽڵ����� ����������
	GD25Q32_write((uint8_t *)"������", 0, 10);

	// �ȴ�д�����
	delay_ms(200);

	//��ȡ0��ַ����Ϊ7���ֽڵ����ݵ�buff
	GD25Q32_read(buff, 0, 10);
	//�����ȡ��������
	printf("buff: %s\r\n",buff);
			
	delay_ms(1000);
			
	unsigned int t = 0;
	while(1) 
	{
		t %= 15;
		GD25Q32_erase_sector(t++);
					
	}
	

}
