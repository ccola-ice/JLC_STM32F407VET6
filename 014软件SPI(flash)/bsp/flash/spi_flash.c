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
#include "spi_flash.h"
#include "board.h"

/**********************************************************
 * �� �� �� �ƣ�bsp_spi_init
 * �� �� �� �ܣ���ʼ��SPI
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void bsp_spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitTypeDef GPIO_InitStructure2;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure1.GPIO_Pin = BSP_SPI_NSS|BSP_SPI_SCK|BSP_SPI_MOSI;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT ;     // �������
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure1);
	
	
	GPIO_InitStructure2.GPIO_Pin = BSP_SPI_MISO;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN ;     // ����ģʽ
	GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure2);
	
	W25QXX_CS_ON(1); 	// Ƭѡ����
	W25QXX_SCK_ON(1); 	// ʱ������
}


uint8_t spi_read_write_byte(uint8_t dat)
{
	uint8_t i;
	uint8_t rxData = 0;

	for(i = 0; i < 8; i++)
	{
		W25QXX_SCK_ON(0);
		
		delay_us(1);
		
		//���ݷ���
		if(dat & 0x80)
		{        
			W25QXX_MOSI_ON(1);
		}
		else
		{
			W25QXX_MOSI_ON(0);
		}
		dat <<= 1;
		delay_us(1);

		W25QXX_SCK_ON(1);
		delay_us(1);
		//���ݽ���
		rxData <<= 1;
		if( W25QXX_MISO_ON() )
		{
			rxData |= 0x01;
		}
		delay_us(1);
	}
	W25QXX_SCK_ON(0);

	return rxData;
}

//��ȡоƬID          
//��ȡ�豸ID
uint16_t GD25Q32_readID(void)
{
    uint16_t  temp = 0;     
    //��CS������Ϊ�͵�ƽ     
    W25QXX_CS_ON(0);        
    //����ָ��90h    
    spi_read_write_byte(0x90);//���Ͷ�ȡID����      
    //���͵�ַ  000000H    
    spi_read_write_byte(0x00);             
    spi_read_write_byte(0x00);             
    spi_read_write_byte(0x00); 
        
    //��������
    //����������ID
    temp |= spi_read_write_byte(0xFF)<<8;  
    //�����豸ID
    temp |= spi_read_write_byte(0xFF);        
    //�ָ�CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);      
    //����ID                  
    return temp;
}

//����дʹ��
void GD25Q32_write_enable(void)   
{
    //����CS��Ϊ�͵�ƽ
    W25QXX_CS_ON(0);                          
    //����ָ��06h
    spi_read_write_byte(0x06);                  
    //����CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);
}

/**********************************************************
 * �� �� �� �ƣ�GD25Q32_wait_busy
 * �� �� �� �ܣ������·�Ƿ�æ
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void GD25Q32_wait_busy(void)   
{   
    unsigned char byte = 0;
    do
     { 
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0); 
        //����ָ��05h                           
        spi_read_write_byte(0x05);                
        //����״̬�Ĵ���ֵ
        byte = spi_read_write_byte(0Xff);       
        //�ָ�CS��Ϊ�ߵ�ƽ
        W25QXX_CS_ON(1);      
     //�ж�BUSYλ�Ƿ�Ϊ1 ���Ϊ1˵����æ�����¶�дBUSYλֱ��Ϊ0   
     }while( ( byte & 0x01 ) == 1 );  
}

/**********************************************************
 * �� �� �� �ƣ�GD25Q32_erase_sector
 * �� �� �� �ܣ�����һ������
 * �� �� �� ����addr=������������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��addr=�����������ţ���Χ=0~15
**********************************************************/
void GD25Q32_erase_sector(uint32_t addr)   
{
        //���������ţ�һ������4KB=4096
        addr *= 4096;
        GD25Q32_write_enable();  //дʹ��   
        GD25Q32_wait_busy();     //�ж�æ�����æ��һֱ�ȴ�
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0);  
        //����ָ��20h                                     
        spi_read_write_byte(0x20);
        //����24λ������ַ�ĸ�8λ                
        spi_read_write_byte((uint8_t)((addr)>>16));      
        //����24λ������ַ����8λ    
        spi_read_write_byte((uint8_t)((addr)>>8));   
        //����24λ������ַ�ĵ�8λ    
        spi_read_write_byte((uint8_t)addr);
        //�ָ�CS��Ϊ�ߵ�ƽ  
        W25QXX_CS_ON(1);                  
        //�ȴ��������                                                  
        GD25Q32_wait_busy();   
}

/**********************************************************
 * �� �� �� �ƣ�GD25Q32_write
 * �� �� �� �ܣ�д���ݵ�GD25Q32���б���
 * �� �� �� ����buffer=д�����������        addr=д���ַ        numbyte=д�����ݵĳ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void GD25Q32_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte)
{    
    unsigned int i = 0;
    //������������
    GD25Q32_erase_sector(addr/4096);
    //дʹ�� 
    GD25Q32_write_enable();  
    //æ���  
    GD25Q32_wait_busy();    
    //д������
    //����CS��Ϊ�͵�ƽ
    W25QXX_CS_ON(0);         
    //����ָ��02h                              
    spi_read_write_byte(0x02);                 
    //����д���24λ��ַ�еĸ�8λ   
    spi_read_write_byte((uint8_t)((addr)>>16));  
    //����д���24λ��ַ�е���8λ
    spi_read_write_byte((uint8_t)((addr)>>8));   
    //����д���24λ��ַ�еĵ�8λ
    spi_read_write_byte((uint8_t)addr);   
    //����д����ֽڳ�������д������buffer
    for(i=0;i<numbyte;i++)
    {
        spi_read_write_byte(buffer[i]);  
    }
    //�ָ�CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);
    //æ��� 
    GD25Q32_wait_busy();      
}

/**********************************************************
 * �� �� �� �ƣ�GD25Q32_read
 * �� �� �� �ܣ���ȡGD25Q32������
 * �� �� �� ����buffer=�������ݵı����ַ  read_addr=��ȡ��ַ   read_length=��ȥ����
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void GD25Q32_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length)   
{ 
        uint16_t i;                   
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0);    
        //����ָ��03h        
        spi_read_write_byte(0x03);  
        //����24λ��ȡ���ݵ�ַ�ĸ�8λ                         
        spi_read_write_byte((uint8_t)((read_addr)>>16));     
        //����24λ��ȡ���ݵ�ַ����8λ      
        spi_read_write_byte((uint8_t)((read_addr)>>8));   
        //����24λ��ȡ���ݵ�ַ�ĵ�8λ
        spi_read_write_byte((uint8_t)read_addr);   
        //���ݶ�ȡ���ȶ�ȡ����ַ���浽buffer��
        for(i=0;i<read_length;i++)
        { 
            buffer[i]= spi_read_write_byte(0XFF);  
        }
        //�ָ�CS��Ϊ�ߵ�ƽ
        W25QXX_CS_ON(1);                                    
}
