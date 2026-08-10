#ifndef __SPI_H
#define __SPI_H

#include "stm32f1xx.h"
#include <stdio.h>

#define      QST_SPI_CS_PORT                GPIOA
#define      QST_SPI_CS_PIN                 GPIO_PIN_6

#define      QST_SPI_CS_HIGH				HAL_GPIO_WritePin(QST_SPI_CS_PORT, QST_SPI_CS_PIN, GPIO_PIN_SET)				// CS----PB6
#define      QST_SPI_CS_LOW					HAL_GPIO_WritePin(QST_SPI_CS_PORT, QST_SPI_CS_PIN, GPIO_PIN_RESET)

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

extern uint8_t qst_hw_spi_write(uint8_t Addr, uint8_t Data);
extern uint8_t qst_hw_spi_read(uint8_t addr, uint8_t* buff, uint16_t len);

#endif /* __SPI_H */

