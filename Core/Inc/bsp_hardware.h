

#ifndef __BSP_HARDWARE_H
#define	__BSP_HARDWARE_H

#include "stm32f1xx.h"
#include "qst_log.h"
#include "bsp_sw_i2c.h"
#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_sw_spi.h"
#include "oled.h"
// #include "bsp_flash.h"

#if defined(STM32F429xx)
#define DEBUG_USART		USART3
#else
#define DEBUG_USART		USART1
#endif

typedef void (*int_callback)(void);
typedef void (*usart_callback)(unsigned char *buf, unsigned short len);

typedef enum
{
	INTERFACE_USER_SEL = -1,
	INTERFACE_I2C_SW = 0,
	INTERFACE_I2C_HW,
	INTERFACE_I2C_HW_1M,
	INTERFACE_I3C_4M,
	INTERFACE_I3C_6_25M,
	INTERFACE_I3C_10M,
	INTERFACE_I3C_12_5M,
	INTERFACE_SPI_HW4,
	INTERFACE_SPI_HW3,
	INTERFACE_SPI_SW4,
	INTERFACE_SPI_SW3,


	INTERFACE_TOTAL,	
	INTERFACE_NONE = 0xff,
} evb_interface_e;

typedef enum
{
	EVB_SPI_MODE0,
	EVB_SPI_MODE1,
	EVB_SPI_MODE2,
	EVB_SPI_MODE3

} evb_spi_mode_e;

typedef struct
{
	int					i2c_type;
	int					i3c_type;
	int					spi_type;
	evb_spi_mode_e		spi_mode;
} evb_port_t;

enum 
{
	QST_REPORT_OFF = 0x0000,
	QST_REPORT_POLLING = 0x0001,
	QST_REPORT_DRI  = 0x0002,
	QST_REPORT_FIFO = 0x0004,
	QST_REPORT_SELFTEST = 0x0008,
	QST_REPORT_EXT_INT = 0x0010,

	QST_REPORT_END
};

typedef enum
{
	QST_NUCLEO_INT_NONE = 0x00,
	QST_NUCLEOPC7_IMU_INT1 = 0x01,
	QST_NUCLEOPA9_IMU_INT2 = 0x02,
	QST_NUCLEOPB4_ACC_INT1 = 0x04,
	QST_NUCLEOPB10_ACC_INT2 = 0x08,
	QST_NUCLEOPB3_INT = 0x10,
	QST_NUCLEO_INT_ALL = 0xff
}qst_nucleo_int;

enum
{
	QST_KEY_NONE = 0x00,
	QST_KEY1 = 0x01,
	QST_KEY2 = 0x02
};

typedef struct bsp_tim_func_t
{
	unsigned char		tim1_flag;
	unsigned char		tim2_flag;
	unsigned char		tim3_flag;
	unsigned char		tim4_flag;
	unsigned char		tim5_flag;
	unsigned char		tim6_flag;
	unsigned char		tim7_flag;
	int_callback		tim1_func;
	int_callback		tim2_func;
	int_callback		tim3_func;
	int_callback		tim4_func;
	int_callback		tim5_func;
	int_callback		tim6_func;
	int_callback		tim7_func;
}bsp_tim_func_t;

typedef struct bsp_irq_func_t
{
	unsigned char		irq1_flag;
	unsigned char		irq2_flag;
	int_callback		irq1_func;
	int_callback		irq2_func;	
}bsp_irq_func_t;

typedef struct bsp_usart_rx_t
{
	unsigned char		rx_it[2];
	unsigned char		rx_buf[256];
	unsigned short		rx_len;
	unsigned int		rx_cplt_count;
	usart_callback		rx_cbk;
}bsp_usart_rx_t;

typedef struct bsp_key_func_t
{
	unsigned char		key1_flag;
	int_callback		key1_func;
	unsigned char		key2_flag;
	int_callback		key2_func;
	int					debounce_flag;
	unsigned int		key1_delay_count;
	unsigned int		key2_delay_count;
}bsp_key_func_t;

extern void bsp_event_clear(void);
extern void bsp_hardware_init(void);
extern void evb_setup_irq(int int_type, int_callback func, FunctionalState enable);
extern void evb_setup_timer(TIM_TypeDef* timid, int_callback func, uint16_t ms, FunctionalState enable);
extern void evb_set_pwm_duty(TIM_TypeDef * tim_id, uint16_t duty);
extern void evb_setup_uart_rx(USART_TypeDef* USARTx, usart_callback func);
extern void evb_setup_user_key(int id, int_callback func, int debounce_en);
extern void evb_setup_adc(int channel, FunctionalState enable);
extern unsigned int evb_get_adc(int channel);


extern void evb_irq_handle(void);
extern void evb_tim_handle(void);
extern void evb_usart_rx_handle(void);
extern void evb_key_handle(void);

extern void qst_delay_ms(unsigned int delay);
extern void qst_delay_us(unsigned int delay);
extern void SysTick_Enable(unsigned char enable);
extern void usart_send_ch(uint8_t ch);

extern void Error_Handler(void);
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#endif

extern void MX_USART2_UART_DeInit(void);
extern void MX_USART3_UART_DeInit(void);
extern void bsp_power_pin_set(int on);

extern void MX_I2C1_Init(uint32_t clk);
extern void MX_I2C1_DeInit(void);

extern void bsp_port_i2c_init(evb_interface_e type);
extern void bsp_port_i2c_deinit(void);
extern int bsp_i2c_write_reg(unsigned char slave, unsigned char reg, unsigned char value);
extern int bsp_i2c_write_regs(unsigned char slave, unsigned char reg, unsigned char *values, unsigned short len);
extern int bsp_i2c_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, unsigned short len);
extern int bsp_i2c2_write_reg(unsigned char slave, unsigned char reg, unsigned char value);
extern int bsp_i2c2_write_regs(unsigned char slave, unsigned char reg, unsigned char *values, unsigned short len);
extern int bsp_i2c2_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, unsigned short len);
extern int bsp_i3c_write_reg(unsigned char reg, unsigned char value);
extern int bsp_i3c_read_reg(unsigned char reg, uint8_t* buff, unsigned short len);
extern int bsp_spi_write_reg(unsigned char reg, unsigned char value);
extern int bsp_spi_read_reg(unsigned char reg, uint8_t* buff, uint16_t len);
extern void bsp_port_i2c_init(evb_interface_e type);
extern void bsp_port_i2c_deinit(void);
extern void bsp_port_spi_init(evb_interface_e type, int mode);
extern int bsp_write_reg(unsigned char slave, unsigned char reg, unsigned char value);
extern int bsp_read_reg(unsigned char slave, unsigned char reg, uint8_t* buff, uint16_t len);

extern void bsp_port_init(int *intf, int spi_sel);
extern void bsp_port_deinit(int sel);
extern char * bsp_get_interface_info(int sel);

#define QST_EVB_NONE		QST_NUCLEO_INT_NONE
#if 1	// imu
#define QST_EVB_INT1		QST_NUCLEOPC7_IMU_INT1
#define QST_EVB_INT2		QST_NUCLEOPA9_IMU_INT2
#else
#define QST_EVB_INT1		QST_NUCLEOPA9_IMU_INT2
#define QST_EVB_INT2		QST_NUCLEOPC7_IMU_INT1
#endif

//#define HC_BLE_SUPPORT

#endif
