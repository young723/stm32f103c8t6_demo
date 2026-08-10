 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi bsp 
  ******************************************************************************
  */
  
#include "bsp_spi.h"
#include "bsp_sw_spi.h"


extern SPI_HandleTypeDef hspi1;

static void bsp_spi_delay(uint32_t nCount)
{
	while(nCount--)
	{
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	}
}

uint8_t qst_hw_spi_read(uint8_t addr, uint8_t* buff, uint16_t len)
{
	HAL_StatusTypeDef status;

	QST_SPI_CS_LOW;
	bsp_spi_delay(2);
	addr |= 0x80;
	status = HAL_SPI_Transmit(&hspi1, &addr, 1, 100);
	status = HAL_SPI_Receive(&hspi1, buff, len, 100);
	bsp_spi_delay(2);
	QST_SPI_CS_HIGH;
	if(status == HAL_OK)
		return 1;
	else
		return 0;
}

uint8_t qst_hw_spi_write(uint8_t Addr, uint8_t Data)
{
	HAL_StatusTypeDef status;
	unsigned char buf[2];
	
	buf[0] = Addr&0x7f;
	buf[1] = Data;
	
	QST_SPI_CS_LOW;
	bsp_spi_delay(2);
	status = HAL_SPI_Transmit(&hspi1, buf, 2, 100);
	bsp_spi_delay(2);
	QST_SPI_CS_HIGH;

	if(status == HAL_OK)
		return 1;
	else
		return 0;
}


uint8_t spi_send_byte(uint8_t byte)
{
	HAL_SPI_Transmit(&hspi1, &byte, 1, 100);

	return 1;
}

uint8_t spi_recv_byte(uint8_t byte)
{
	//HAL_StatusTypeDef status;
	uint8_t out;

	HAL_SPI_Receive(&hspi1, &out, 1, 100);

	return out;
}

unsigned char qmp6989_spi_write(unsigned char Addr, unsigned char* Data, unsigned short len)
{
	unsigned char addr = 0;
	unsigned char sdobyte = 0;
	

	QST_SPI_CS_LOW;
	bsp_spi_delay(5);

	if(len == 1)
	{
		sdobyte = 0x00|0x00;
	}
	else if(len == 2)
	{
		sdobyte = 0x00|0x20;
	}
	else if(len == 3)
	{
		sdobyte = 0x00|0x40;
	}
	else
	{
		sdobyte = 0x00|0x60;
	}
	spi_send_byte(sdobyte);         
	spi_send_byte(Addr);
	while(len)
	{
		spi_send_byte(Data[addr]);				  
		addr++;
		len--;
	}

	bsp_spi_delay(5);
	QST_SPI_CS_HIGH;

	return 1;
}

unsigned char qmp6989_spi_read(unsigned char Addr, unsigned char *pData, unsigned char Length)
{
	unsigned char i = 0;
	unsigned char sdoByte=0;

	QST_SPI_CS_LOW;
	bsp_spi_delay(5);

	if(Length == 1)
	{
		sdoByte = 0x80|0x00;
	}
	else if(Length == 2)
	{
		sdoByte = 0x80|0x20;
	}
	else if(Length == 3)
	{
		sdoByte = 0x80|0x40;
	}
	else
	{
		sdoByte = 0x80|0x60;
	}
	spi_send_byte(sdoByte); 		
	spi_send_byte(Addr);
	// SpiShift(0); 						 
	while(Length)
	{
		pData[i] = spi_recv_byte(0x00);
		i++;
		Length--;
	}

	bsp_spi_delay(5);
	QST_SPI_CS_HIGH;
	
	return 1;
}


