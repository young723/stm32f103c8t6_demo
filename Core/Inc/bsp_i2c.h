

#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f1xx.h"

extern int bsp_i2c1_write(unsigned char slave, unsigned char addr, unsigned char value);
extern int bsp_i2c1_writes(unsigned char slave, unsigned char addr, unsigned char *value, unsigned short len);
extern int bsp_i2c1_read(unsigned char slave, unsigned char addr, unsigned char *buf, unsigned short len);

extern int bsp_i2c2_write(unsigned char slave, unsigned char addr, unsigned char value);
extern int bsp_i2c2_writes(unsigned char slave, unsigned char addr, unsigned char *value, unsigned short len);
extern int bsp_i2c2_read(unsigned char slave, unsigned char addr, unsigned char *buf, unsigned short len);
// extern int bsp_i2c2_write_buf(unsigned char slave, unsigned char *buf, unsigned short len);
// extern int bsp_i2c2_read_buf(unsigned char slave, unsigned char *buf, unsigned short len);

#endif
