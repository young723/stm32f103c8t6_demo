#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#define FONT_SIZE8		8
#define FONT_SIZE12		12
#define FONT_SIZE16		16
#define FONT_SIZE24		24
#define XLevelL			0x02
#define XLevelH			0x10
#define OLED_MAX_COL	128
#define OLED_MAX_ROW	8
#define OLED_WIDTH 	128
#define OLED_HEIHT 	64

#define OLED_SLAVE	(0x78>>1) // 0x78

// #define USE_SH1106
#define USE_SSD1306
// #define USE_SSD1315

enum
{    
    SCROLL_OFF = 0,
    SCROLL_LEFT = 1,
    SCROLL_RIGHT,
};

enum
{
	OLED_LCM_0 = 0,
	OLED_LCM_1,
	OLED_LCM_MAX
};

void oled_init(void);
void oled_clear_data(void);
void oled_refresh(void);
// void oled_refresh_burst(void);
void oled_set_lcm(int lcm_i);
void oled_display_on(void);
void oled_display_off(void);
void oled_fill_data(unsigned char fill_Data);
void oled_set_pos(unsigned char x, unsigned char y);

void oled_draw_line(short x1, short y1, short x2, short y2, uint8_t mode);
void oled_draw_circle(short x, short y, uint8_t r);
void oled_show_char(short x,short y,uint8_t chr,uint8_t size1,uint8_t mode);
void oled_show_num(short x,short y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void oled_show_str(short x,short y,uint8_t *chr,uint8_t size1,uint8_t mode);
void oled_show_chinese(short x,short y,uint8_t num,uint8_t size1,uint8_t mode);
void oled_show_pic(short x,short y,uint8_t sizex,uint8_t sizey,const uint8_t BMP[],uint8_t mode);
void oled_scroll(int direct);

#endif
