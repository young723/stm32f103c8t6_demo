/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
	
	* Function List:
	*	1. void I2C_Configuration(void) -- ����CPU��Ӳ��I2C
	* 2. void oled_write_byte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
	* 3. void oled_write_cmd(unsigned char I2C_Command) -- д����
	* 4. void oled_write_data(unsigned char I2C_Data) -- д����
	* 5. void oled_init(void) -- OLED����ʼ��
	* 6. void oled_set_pos(unsigned char x, unsigned char y) -- ������ʼ������
	* 7. void oled_fill(unsigned char fill_Data) -- ȫ�����
	* 8. void oled_cls(void) -- ����
	* 9. void oled_on(void) -- ����
	* 10. void oled_off(void) -- ˯��
	* 11. void oled_show_str(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
	* 12. void oled_show_cn(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
	* 13. void oled_drae_bmp(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
	*
  *
  ******************************************************************************
  */ 

#include <stdint.h>
#include <string.h>
#include "oled.h"
#include "oledfont.h"
#include "bsp_hardware.h"

// #define oled_write_cmd(cmd)			bsp_i2c_write_reg(OLED_SLAVE, 0x00, cmd)
// #define oled_write_data(data)		bsp_i2c_write_reg(OLED_SLAVE, 0x40, data)
// #define oled_write_buf(buf, len)	bsp_i2c_write_regs(OLED_SLAVE, 0x40, buf, len)
static int oled_lcm_i = OLED_LCM_0;
static uint8_t lcm_gram[OLED_MAX_COL][OLED_MAX_ROW];	//[144][8];
static uint8_t lcm_col[OLED_MAX_COL];

 void oled_delay(unsigned int Del_1ms)
 {
	qst_delay_ms(Del_1ms);
 }
 
void oled_write_cmd(unsigned char cmd)
{
	if(oled_lcm_i == OLED_LCM_0)
	{
		bsp_i2c_write_reg(OLED_SLAVE, 0x00, cmd);
	}
	else if(oled_lcm_i == OLED_LCM_1)
	{
		bsp_i2c2_write_reg(OLED_SLAVE, 0x00, cmd);
	}
}

void oled_write_data(unsigned char data)
{
	if(oled_lcm_i == OLED_LCM_0)
	{
		bsp_i2c_write_reg(OLED_SLAVE, 0x40, data);
	}
	else if(oled_lcm_i == OLED_LCM_1)
	{
		bsp_i2c2_write_reg(OLED_SLAVE, 0x40, data);
	}
}

void oled_write_buf(unsigned char *buf, unsigned short len)
{
	if(oled_lcm_i == OLED_LCM_0)
	{
		bsp_i2c_write_regs(OLED_SLAVE, 0x40, buf, len);
	}
	else if(oled_lcm_i == OLED_LCM_1)
	{
		bsp_i2c2_write_regs(OLED_SLAVE, 0x40, buf, len);
	}
}

void oled_set_lcm(int lcm_i)
{
#if defined(USE_DUAL_OLED)
	if(lcm_i < OLED_LCM_MAX)
	{
		oled_lcm_i = lcm_i;
	}
	else
	{
		qst_loge("oled_set_lcm error %d\r\n", lcm_i);
	}
#else
	oled_lcm_i = OLED_LCM_0;
#endif
}
/********************************************
// fill data
********************************************/
void oled_clear_data(void)
{
	memset(lcm_gram, 0x00, sizeof(lcm_gram));
}

void oled_fill_data(unsigned char fill_Data)
{
	memset(lcm_gram, fill_Data, sizeof(lcm_gram));
	//oled_refresh_burst();
}

void oled_refresh(void)
{
#if defined(USE_SH1106)
	uint16_t m,n;

	for(m=0;m<OLED_MAX_ROW;m++)
	{
		oled_write_cmd(0xb0+m); //设置行起始地址
		oled_write_cmd(0x02);   //设置低列起始地址
		oled_write_cmd(0x10);   //设置高列起始地址
		for(n=0;n<OLED_MAX_COL;n++)
		{
			//oled_write_data(lcm_gram[n][m]);
			lcm_col[n] = lcm_gram[n][m];
		}
		oled_write_buf(lcm_col, OLED_MAX_COL);
	}
#elif defined(USE_SSD1315)
	uint16_t m,n;

	for(m=0;m<OLED_MAX_ROW;m++)
	{
		oled_write_cmd(0xb0+m); //设置行起始地址
		oled_write_cmd(0x00);   //设置低列起始地址
		oled_write_cmd(0x10);   //设置高列起始地址
		for(n=0;n<OLED_MAX_COL;n++)
		{
			//oled_write_data(lcm_gram[n][m]);
			lcm_col[n] = lcm_gram[n][m];
		}
		oled_write_buf(lcm_col, OLED_MAX_COL);
	}
#elif defined(USE_SSD1306)
	oled_write_cmd(0x20); oled_write_cmd(0x01);
	oled_write_cmd(0x21); oled_write_cmd(0x00); oled_write_cmd(0x7F); // 列 0..127
	oled_write_cmd(0x22); oled_write_cmd(0x00); oled_write_cmd(0x07); // 页 0..7
	oled_write_buf(&lcm_gram[0][0], OLED_MAX_COL*OLED_MAX_ROW);
	// 恢复页寻址(与 oled_init 默认一致), 保证后续 set_pos/show_pic 正常
	oled_write_cmd(0x20); oled_write_cmd(0x02);
#endif
}

/********************************************
// 高速整帧刷新: 切到垂直寻址, 一次 I2C 突发把整帧发出。
// lcm_gram[col][page] 在内存中是列优先连续排列:
//   col0_pg0, col0_pg1, ..., col0_pg7, col1_pg0, ...
// 垂直寻址模式(0x01)的自动增量顺序恰好匹配——先增页再增列。
// 400kHz I2C 下耗时约 23ms(≈40~50Hz)。
********************************************/
// void oled_refresh_burst(void)
// {
// 	// 垂直寻址模式 + 设置整屏列/页范围
// 	oled_write_cmd(0x20); oled_write_cmd(0x01);
// 	oled_write_cmd(0x21); oled_write_cmd(0x00); oled_write_cmd(0x7F); // 列 0..127
// 	oled_write_cmd(0x22); oled_write_cmd(0x00); oled_write_cmd(0x07); // 页 0..7

// 	oled_write_buf(&lcm_gram[0][0], OLED_MAX_COL*OLED_MAX_ROW);

// 	// 恢复页寻址(与 oled_init 默认一致), 保证后续 set_pos/show_pic 正常
// 	oled_write_cmd(0x20); oled_write_cmd(0x02);
// }

// set the start position (x,y) of the screen
void oled_set_pos(unsigned char x, unsigned char y) 
{ 	
	oled_write_cmd(0xb0+y);
	oled_write_cmd(((x&0xf0)>>4)|0x10);
	oled_write_cmd((x&0x0f)); 
}

// OLED display ON    
void oled_display_on(void)
{
	oled_write_cmd(0X8D);  //SET DCDC command
	oled_write_cmd(0X14);  //DCDC ON
	oled_write_cmd(0XAF);  //DISPLAY ON
}

// OLED display OFF     
void oled_display_off(void)
{
	oled_write_cmd(0X8D);  //SET DCDC command
	oled_write_cmd(0X10);  //DCDC OFF
	oled_write_cmd(0XAE);  //DISPLAY OFF
}


//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void oled_draw_point(short x, short y, uint8_t t)
{
	uint8_t i, m, n;

	if(x<0 || x>=OLED_MAX_COL || y<0 || y>=OLED_HEIHT)
	{
		return;
	}

	i = y / 8;
	m = y % 8;
	n = 1 << m;

	if(t)
	{
		lcm_gram[x][i] |= n;
	}
	else
	{
		// lcm_gram[x][i]=~lcm_gram[x][i];
		// lcm_gram[x][i]|=n;
		// lcm_gram[x][i]=~lcm_gram[x][i];
		lcm_gram[x][i] &= (~n);
	}
}

void oled_draw_line(short x1, short y1, short x2, short y2, uint8_t mode)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		oled_draw_point(uRow,uCol,mode);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:圆心坐标
//r:圆的半径
void oled_draw_circle(short x, short y, uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        oled_draw_point(x + a, y - b,1);
        oled_draw_point(x - a, y - b,1);
        oled_draw_point(x - a, y + b,1);
        oled_draw_point(x + a, y + b,1);
 
        oled_draw_point(x + b, y + a,1);
        oled_draw_point(x + b, y - a,1);
        oled_draw_point(x - b, y - a,1);
        oled_draw_point(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

void oled_show_char(short x,short y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	short x0=x,y0=y;

	if(size1==FONT_SIZE8)
		size2=6;
	else
		size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数

	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==FONT_SIZE8)
			{temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==FONT_SIZE12)
			{temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==FONT_SIZE16)
			{temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==FONT_SIZE24)
			{temp=asc2_2412[chr1][i];} //调用2412字体
		else return;

		for(m=0;m<8;m++)
		{
			if(temp&0x01)
				oled_draw_point(x,y,mode);
			else 
				oled_draw_point(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=FONT_SIZE8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void oled_show_str(short x,short y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		oled_show_char(x,y,*chr,size1,mode);
		if(size1==FONT_SIZE8)
			x+=6;
		else 
			x+=size1/2;
		chr++;
  }
}

//m^n
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void oled_show_num(short x,short y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
	uint8_t t,temp,m=0;

	if(size1==FONT_SIZE8)
		m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
			if(temp==0)
			{
				oled_show_char(x+(size1/2+m)*t,y,'0',size1, mode);
      }
			else 
			{
			  oled_show_char(x+(size1/2+m)*t,y,temp+'0',size1, mode);
			}
  }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void oled_show_chinese(short x,short y,uint8_t num,uint8_t size1,uint8_t mode)
{
	uint8_t m,temp;
	short x0=x,y0=y;
	uint16_t i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//调用16*16字体
		else if(size1==24)
				{temp=Hzk2[num][i];}//调用24*24字体
		else if(size1==32)       
				{temp=Hzk3[num][i];}//调用32*32字体
		else if(size1==64)
				{temp=Hzk4[num][i];}//调用64*64字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)
				oled_draw_point(x,y,mode);
			else 
				oled_draw_point(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{
			x=x0;
			y0=y0+8;
		}
		y=y0;
	}
}

void oled_show_pic(short x,short y,uint8_t sizex,uint8_t sizey,const uint8_t BMP[],uint8_t mode)
{
	uint16_t j=0;
	uint8_t i,n,temp,m;
	short x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
			temp=BMP[j];
			j++;
			for(m=0;m<8;m++)
			{
				if(temp&0x01)
					oled_draw_point(x,y,mode);
				else
					oled_draw_point(x,y,!mode);
				temp>>=1;
				y++;
			}
			x++;
			if((x-x0)==sizex)
			{
				x=x0;
				y0=y0+8;
			}
			y=y0;
     	}
	 }
}

void oled_scroll(int direct)
{
	if(direct == SCROLL_LEFT)
	{
		oled_write_cmd(0x27);          // 向左滚动
	}
	else if(direct == SCROLL_RIGHT)
	{
		oled_write_cmd(0x26);          // 向右滚动
	}
	oled_write_cmd(0x00);          // dummy
	oled_write_cmd(0x00);    // 起始页 0x00~0x07
	oled_write_cmd(0x05);      // 滚动速度 0x00~0x07（帧间隔）
	oled_write_cmd(0x07);      // 结束页 0x00~0x07
	oled_write_cmd(0x00);          // dummy
	oled_write_cmd(0xFF);          // dummy
	oled_write_cmd(0x2F);          // 激活
}

// init SSD1306					    
void oled_init(void)
{ 	
	oled_delay(200);
#if defined(USE_SH1106)
	oled_write_cmd(0xAE); /*display off*/ 
	oled_write_cmd(0x02); /*set lower column address*/ 
	oled_write_cmd(0x10); /*set higher column address*/ 
	oled_write_cmd(0x40); /*set display start line*/ 
	oled_write_cmd(0xB0); /*set page address*/
	oled_write_cmd(0x81); /*contract control*/ 
	oled_write_cmd(0xcf); /*128*/ 
	oled_write_cmd(0xA1); /*set segment remap*/ 
	oled_write_cmd(0xA6); /*normal / reverse*/ 
	oled_write_cmd(0xA8); /*multiplex ratio*/ 
	oled_write_cmd(0x3F); /*duty = 1/64*/ 
	oled_write_cmd(0xad); /*set charge pump enable*/ 
	oled_write_cmd(0x8b); /* 0x8B 内供 VCC */ 
	oled_write_cmd(0x33); /*0X30---0X33 set VPP 9V */ 
	oled_write_cmd(0xC8); /*Com scan direction*/ 
	oled_write_cmd(0xD3); /*set display offset*/ 
	oled_write_cmd(0x00); /* 0x20 */ 
	oled_write_cmd(0xD5); /*set osc division*/ 
	oled_write_cmd(0x80); 
	oled_write_cmd(0xD9); /*set pre-charge period*/ 
	oled_write_cmd(0x1f); /*0x22*/ 
	oled_write_cmd(0xDA); /*set COM pins*/ 
	oled_write_cmd(0x12); 
	oled_write_cmd(0xdb); /*set vcomh*/ 
	oled_write_cmd(0x40);
	oled_fill_data(0x00);
	oled_write_cmd(0xAF); /*display ON*/
#endif
#if defined(USE_SSD1306)
	oled_write_cmd(0xAE);//close display
	oled_write_cmd(0xAE);//--turn off oled panel
	oled_write_cmd(0x00);//---set low column address
	oled_write_cmd(0x10);//---set high column address
	oled_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_cmd(0x81);//--set contrast control register
	oled_write_cmd(0xCF);// Set SEG Output Current Brightness
	oled_write_cmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_cmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_cmd(0xA6);//--set normal display
	oled_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3f);//--1/64 duty
	oled_write_cmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_cmd(0x00);//-not offset
	oled_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_cmd(0xD9);//--set pre-charge period
	oled_write_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_cmd(0xDA);//--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xDB);//--set vcomh
	oled_write_cmd(0x40);//Set VCOM Deselect Level
	oled_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_cmd(0x02);//
	oled_write_cmd(0x8D);//--set Charge Pump enable/disable
	oled_write_cmd(0x14);//--set(0x10) disable
	oled_write_cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	oled_write_cmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	oled_write_cmd(0xAF);
	oled_fill_data(0x00);
#endif
#if defined(USE_SSD1315)
	oled_write_cmd(0xAE);//--turn off oled panel
	oled_write_cmd(0x00);//---set low column address
	oled_write_cmd(0x10);//---set high column address
	oled_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_cmd(0x81);//--set contrast control register
	oled_write_cmd(0xCF);// Set SEG Output Current Brightness
	oled_write_cmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_cmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_cmd(0xA6);//--set normal display
	oled_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3f);//--1/64 duty
	oled_write_cmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_cmd(0x00);//-not offset
	oled_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_cmd(0xD9);//--set pre-charge period
	oled_write_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_cmd(0xDA);//--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xDB);//--set vcomh
	oled_write_cmd(0x30);//Set VCOM Deselect Level
	oled_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_cmd(0x02);//
	oled_write_cmd(0x8D);//--set Charge Pump enable/disable
	oled_write_cmd(0x14);//--set(0x10) disable
	oled_write_cmd(0xAF);
	oled_fill_data(0x00);
#endif
}  

