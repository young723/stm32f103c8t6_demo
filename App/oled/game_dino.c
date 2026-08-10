
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "bsp_hardware.h"
#include "game_dino_data.h"
#include "game_dino.h"

#define GAME_DINO_AUTO_PLAY
//#define GAME_DINO_DEBUG

#if defined(GAME_DINO_DEBUG)
#define DINO_LOG	qst_logi
#else
#define DINO_LOG(...)
#endif

#define GAME_DINO_SRC_WIDTH		128
#define GAME_DINO_SRC_HEIGHT	64

#define DINO_Y_MAX				60
#define DINO_Y_MIN				8
#define DINO_MAX_SPEED_UP		5
#define DINO_MAX_SPEED_DOWN		4

enum
{
	PIC_GND = 0,
	PIC_CLOUD,
	PIC_CACTUS,
	PIC_DINO,
	PIC_RESTART,
	PIC_COVER,

	GAME_DINO_MAX	
};

enum
{
	GAME_DINO_STATUS_INIT = 0,
	GAME_DINO_STATUS_OVER,
	GAME_DINO_STATUS_PLAY,
	GAME_DINO_STATUS_PAUSE,

	GAME_DINO_STATUS_MAX	
};

typedef struct
{
	short x;
	short y;
	unsigned short width;
	unsigned short height;
	short			speed;
	const unsigned char* pic_data;
	unsigned int pic_num;
} qst_pic_t;

typedef struct
{
	qst_pic_t 		pic[GAME_DINO_MAX];
	int				game_status;
	unsigned int 	pic_flag;
	unsigned int 	score;
	unsigned int	count;
} qst_dino_t;


static qst_dino_t gDino;
const unsigned char * gnd_array[] = {GROUND_128x8_1, GROUND_128x8_2, GROUND_128x8_3,GROUND_128x8_4};
const unsigned char * dino_array[] = {DINO_16x16_1, DINO_16x16_2};
const unsigned char * cactus_array[] = {CACTUS_1_8x16, CACTUS_2_16x16, CACTUS_3_16x24, CACTUS_4_16x24};

void game_dino_para_init(void);

static void game_dino_clr(void)
{
	oled_clear_data();
}

static void game_dino_refresh(void)
{
	oled_refresh();
}

static void game_dino_show_pic(short x, short y, unsigned short width, unsigned short height, const unsigned char pic[])
{
	oled_show_pic(x, y, width, height, pic, 1);
}

static void game_dino_show_str(short x, short y, unsigned char* pbuf, unsigned char size)
{
	oled_show_str(x, y, pbuf, size, 1);
}

// static void game_dino_show_num(short x, short y, unsigned int num, unsigned char len)
// {
// 	oled_show_num(x, y, num, len, FONT_SIZE8, 1);
// }

static int game_dino_check_in_cactus(int id)
{
	short x, y;
	short x1,y1,x2,y2;
	short x_gap = gDino.pic[PIC_CACTUS].width/5;
	short y_gap = gDino.pic[PIC_CACTUS].height/5;

	x1 = gDino.pic[PIC_CACTUS].x + x_gap;
	y1 = gDino.pic[PIC_CACTUS].y + y_gap;
	x2 = gDino.pic[PIC_CACTUS].x + gDino.pic[PIC_CACTUS].width - x_gap;
	y2 = gDino.pic[PIC_CACTUS].y + gDino.pic[PIC_CACTUS].height - y_gap;

	x = gDino.pic[id].x + gDino.pic[id].width/2;
	y = gDino.pic[id].y + gDino.pic[id].height/2;
	if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
	{
		return 1;
	}

	x = gDino.pic[id].x;
	y = gDino.pic[id].y;
	if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
	{
		return 1;
	}

	x = gDino.pic[id].x+gDino.pic[id].width;
	y = gDino.pic[id].y;
	if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
	{
		return 1;
	}

	x = gDino.pic[id].x;
	y = gDino.pic[id].y+gDino.pic[id].height;
	if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
	{
		return 1;
	}

	x = gDino.pic[id].x+gDino.pic[id].width;
	y = gDino.pic[id].y+gDino.pic[id].height;
	if((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
	{
		return 1;
	}

	return 0;
}

static void game_dino_create_cactus(int id)
{
	unsigned int sel_pic = 0;

#if defined(GAME_DINO_AUTO_PLAY)
	static unsigned int last_sel_pic = 0;

	last_sel_pic++;
	last_sel_pic = last_sel_pic % gDino.pic[PIC_CACTUS].pic_num;
	sel_pic = last_sel_pic;
#else
	sel_pic = gDino.count % gDino.pic[PIC_CACTUS].pic_num;
#endif

	if(sel_pic < gDino.pic[id].pic_num)
	{
		gDino.pic[id].pic_data = cactus_array[sel_pic];
		if(sel_pic == 0)
		{
			gDino.pic[id].width = 8;
			gDino.pic[id].height = 16;
		}
		else if(sel_pic == 1)
		{
			gDino.pic[id].width = 16;
			gDino.pic[id].height = 16;
		}
		else if(sel_pic == 2)
		{
			gDino.pic[id].width = 16;
			gDino.pic[id].height = 24;
		}
		else if(sel_pic == 3)
		{
			gDino.pic[id].width = 16;
			gDino.pic[id].height = 24;
		}
		gDino.pic[id].x = GAME_DINO_SRC_WIDTH /*- gDino.pic[id].width*/;
		gDino.pic[id].y = DINO_Y_MAX - gDino.pic[id].height;
		gDino.pic_flag |= (1 << id);
	}
}

static void game_dino_draw_obj(void)
{
	uint8_t buf[20];

	oled_set_lcm(OLED_LCM_0);
	game_dino_clr();
	sprintf((char*)buf, "Hi: %d", gDino.score);
	game_dino_show_str(0, 0, buf, FONT_SIZE8);
	for(int i=0; i<GAME_DINO_MAX; i++)
	{
		if((gDino.pic[i].pic_data) && (gDino.pic_flag &(1<<i)))
		{
			game_dino_show_pic(gDino.pic[i].x, gDino.pic[i].y, gDino.pic[i].width, gDino.pic[i].height, gDino.pic[i].pic_data);
		}
	}
	if(gDino.game_status == GAME_DINO_STATUS_OVER)
	{
		game_dino_show_str(10, 24, (unsigned char*)"GAME", FONT_SIZE16);
		game_dino_show_str(86, 24, (unsigned char*)"OVER", FONT_SIZE16);
	}
	game_dino_refresh();
}

static void game_dino_timer_hdr(void)
{
	if(gDino.game_status == GAME_DINO_STATUS_PLAY)
	{
		gDino.count++;
		if(gDino.pic_flag & (1<<PIC_GND))
		{
			gDino.pic[PIC_GND].pic_data = gnd_array[gDino.count % gDino.pic[PIC_GND].pic_num];
		}

		if(gDino.pic_flag & (1<<PIC_CLOUD))
		{
			gDino.pic[PIC_CLOUD].x += gDino.pic[PIC_CLOUD].speed;
			if(gDino.pic[PIC_CLOUD].x <= 0-gDino.pic[PIC_CLOUD].width)
			{
				gDino.pic[PIC_CLOUD].x = GAME_DINO_SRC_WIDTH + gDino.count%50;
				gDino.pic[PIC_CLOUD].y = (gDino.count%3 + 1)*8;
				// gDino.pic_flag &= ~(1<<PIC_CLOUD);
			}
		}
		// else
		// {
		// 	gDino.pic[PIC_CLOUD].x += gDino.pic[PIC_CLOUD].speed;
		// 	if(gDino.pic[PIC_CLOUD].x <= GAME_DINO_SRC_WIDTH /*- gDino.pic[PIC_CLOUD].width*/)
		// 	{
		// 		gDino.pic_flag |= (1<<PIC_CLOUD);
		// 	}
		// }

		if(gDino.pic_flag & (1<<PIC_CACTUS))
		{
			gDino.pic[PIC_CACTUS].x += gDino.pic[PIC_CACTUS].speed;
			if(gDino.pic[PIC_CACTUS].x <= 0-gDino.pic[PIC_CACTUS].width)
			{
				// gDino.pic[PIC_CACTUS].x = 0;
				gDino.pic_flag &= ~(1<<PIC_CACTUS);
				// game_dino_create_cactus(PIC_CACTUS);
			}
		}

		if(gDino.pic_flag & (1<<PIC_DINO))
		{
			gDino.pic[PIC_DINO].pic_data = dino_array[gDino.count % gDino.pic[PIC_DINO].pic_num];
			gDino.pic[PIC_DINO].y += gDino.pic[PIC_DINO].speed;
			if(gDino.pic[PIC_DINO].speed < -(DINO_MAX_SPEED_UP-1))
			{
				gDino.pic[PIC_DINO].speed++;
			}
			else if((gDino.pic[PIC_DINO].speed > 0) && (gDino.pic[PIC_DINO].speed < DINO_MAX_SPEED_DOWN))
			{
				gDino.pic[PIC_DINO].speed++;
			}

			if(gDino.pic[PIC_DINO].y <= DINO_Y_MIN)
			{
				gDino.pic[PIC_DINO].speed = 1;
			}
			else if(gDino.pic[PIC_DINO].y >= (DINO_Y_MAX-gDino.pic[PIC_DINO].height))
			{
				gDino.pic[PIC_DINO].speed = 0;
				gDino.pic[PIC_DINO].y = DINO_Y_MAX-gDino.pic[PIC_DINO].height;
			}
		}

		DINO_LOG("DINO[%02d,%02d,%02d] CACTUS[%03d,%03d,%03d]\n",gDino.pic[PIC_DINO].x,gDino.pic[PIC_DINO].y,gDino.pic[PIC_DINO].speed,
														gDino.pic[PIC_CACTUS].x,gDino.pic[PIC_CACTUS].y,gDino.pic[PIC_CACTUS].speed);
#if defined(GAME_DINO_AUTO_PLAY)
		if( (gDino.pic[PIC_DINO].speed == 0) && (gDino.pic_flag & (1<<PIC_CACTUS)) && ((gDino.pic[PIC_CACTUS].x - gDino.pic[PIC_DINO].x) <= 38) )
		{
			DINO_LOG("auto jump\r\n");
			gDino.pic[PIC_DINO].speed = -DINO_MAX_SPEED_UP;
		}
#endif
		// check game over
		if(game_dino_check_in_cactus(PIC_DINO))
		{
			evb_setup_timer(TIM2, NULL, 50, DISABLE);
			game_dino_para_init();
			gDino.game_status = GAME_DINO_STATUS_OVER;
			gDino.pic_flag = (1<<PIC_RESTART);
		}
		else
		{
			if(gDino.pic[PIC_DINO].speed == 0)
			{
				if(gDino.pic[PIC_DINO].x > (gDino.pic[PIC_CACTUS].x+gDino.pic[PIC_CACTUS].width))
				{
					gDino.score += gDino.pic[PIC_CACTUS].width/8;
				}
				if((gDino.pic[PIC_CACTUS].x <= 0) && ((gDino.pic_flag&(1<<PIC_CACTUS))==0))
				{
					//game_dino_create_cactus(PIC_CACTUS, gDino.count % gDino.pic[PIC_CACTUS].pic_num);
					game_dino_create_cactus(PIC_CACTUS);
				}
			}
		}
		// check game over
	}

	game_dino_draw_obj();
}

static void game_dino_key_hdr(void)
{
	switch(gDino.game_status)
	{
		case GAME_DINO_STATUS_INIT:
		case GAME_DINO_STATUS_OVER:
			gDino.count = 0;
			gDino.score = 0;
			gDino.pic_flag = 0;
			gDino.pic_flag |= (1<<PIC_GND);
			gDino.pic_flag |= (1<<PIC_DINO);
			gDino.pic_flag |= (1<<PIC_CLOUD);
			game_dino_create_cactus(PIC_CACTUS);
			gDino.game_status = GAME_DINO_STATUS_PLAY;
			qst_delay_ms(500);
			evb_setup_timer(TIM2, game_dino_timer_hdr, 40, ENABLE);
			break;
		case GAME_DINO_STATUS_PLAY:
			if((gDino.pic[PIC_DINO].speed == 0) && (gDino.count > 5))
			{
				gDino.pic[PIC_DINO].speed = -DINO_MAX_SPEED_UP;
			}
			break;
		default:
			break;
	}
}

void game_dino_para_init(void)
{
	//memset(&gDino, 0, sizeof(gDino));

	gDino.pic[PIC_GND].width = 128;
	gDino.pic[PIC_GND].height = 8;
	gDino.pic[PIC_GND].x = 0;
	gDino.pic[PIC_GND].y = 64-gDino.pic[PIC_GND].height;
	gDino.pic[PIC_GND].speed = 0;
	gDino.pic[PIC_GND].pic_data = gnd_array[0];
	gDino.pic[PIC_GND].pic_num = sizeof(gnd_array) / sizeof(gnd_array[0]);

	gDino.pic[PIC_DINO].width = 16;
	gDino.pic[PIC_DINO].height = 16;
	gDino.pic[PIC_DINO].x = 24;
	gDino.pic[PIC_DINO].y = DINO_Y_MAX - gDino.pic[PIC_DINO].height;
	gDino.pic[PIC_DINO].speed = 0;
	gDino.pic[PIC_DINO].pic_data = dino_array[0];
	gDino.pic[PIC_DINO].pic_num = sizeof(dino_array) / sizeof(dino_array[0]);

	gDino.pic[PIC_CLOUD].width = 24;
	gDino.pic[PIC_CLOUD].height = 8;
	gDino.pic[PIC_CLOUD].x = 60;
	gDino.pic[PIC_CLOUD].y = 8;
	gDino.pic[PIC_CLOUD].speed = -1;
	gDino.pic[PIC_CLOUD].pic_data = CLOUD_24x8;
	gDino.pic[PIC_CLOUD].pic_num = 1;

	gDino.pic[PIC_CACTUS].width = 8;
	gDino.pic[PIC_CACTUS].height = 16;
	gDino.pic[PIC_CACTUS].x = 128-gDino.pic[PIC_CACTUS].width;
	gDino.pic[PIC_CACTUS].y = DINO_Y_MAX - gDino.pic[PIC_CACTUS].height;
	gDino.pic[PIC_CACTUS].speed = -4;
	gDino.pic[PIC_CACTUS].pic_data = cactus_array[0];
	gDino.pic[PIC_CACTUS].pic_num = sizeof(cactus_array) / sizeof(cactus_array[0]);

	gDino.pic[PIC_RESTART].width = 24;
	gDino.pic[PIC_RESTART].height = 24;
	gDino.pic[PIC_RESTART].x = (GAME_DINO_SRC_WIDTH-gDino.pic[PIC_RESTART].width)/2;
	gDino.pic[PIC_RESTART].y = (GAME_DINO_SRC_HEIGHT-gDino.pic[PIC_RESTART].height)/2;
	gDino.pic[PIC_GND].speed = 0;
	gDino.pic[PIC_RESTART].pic_data = RESTART_24x24;
	gDino.pic[PIC_RESTART].pic_num = 1;

	gDino.pic[PIC_COVER].width = 128;
	gDino.pic[PIC_COVER].height = 64;
	gDino.pic[PIC_COVER].x = 0;
	gDino.pic[PIC_COVER].y = 0;
	gDino.pic[PIC_GND].speed = 0;
	gDino.pic[PIC_COVER].pic_data = COVER_128x64;
	gDino.pic[PIC_COVER].pic_num = 1;

	gDino.pic_flag = (1 << PIC_COVER);
	gDino.game_status = GAME_DINO_STATUS_INIT;
}

void game_dino_init(void)
{
	qst_logi("game_dino_init\r\n");
	game_dino_para_init();
	game_dino_draw_obj();

#if defined(GAME_DINO_AUTO_PLAY)
	game_dino_key_hdr();
#else
	evb_setup_user_key(QST_KEY1, game_dino_key_hdr, 0);
#endif
}
