

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "bsp_hardware.h"
#include "gif_cat_a.h"
#include "gif_cat_b.h"
// #include "gif_cat_c.h"

#define GIF_CAT_SRC_WIDTH		128
#define GIF_CAT_SRC_HEIGHT		64

#define GIF_CATA_X_OFT			40
#define GIF_CATB_X_OFT			0
#define GIF_CATB_W				128

#if defined(GIF_CAT_DEBUG)
#define CAT_LOG	qst_logi
#else
#define CAT_LOG(...)
#endif

enum
{
	CAT_GIF_A,
	CAT_GIF_B,
	CAT_GIF_C,

	CAT_GIF_MAX
};

typedef struct
{
	short x;
	short y;
	unsigned short width;
	unsigned short height;

	const unsigned char** pic_p;
//	unsigned short	pic_i;
	unsigned short	pic_max;

	short			pic_x_oft;
	unsigned short	pic_w;
	//short pic_y_oft;
	//unsigned short pic_h;

	short			speed;
	int				repeat_cnt;

	int				use_lcm;
	unsigned int	count;
} qst_cat_t;

const unsigned char* gif_cat_a_array[] = 
{
	cat_a_1, cat_a_2, cat_a_3, cat_a_4, cat_a_5
};
const unsigned char* gif_cat_b_array[] =
{
    p1Data, p2Data, p3Data, p4Data, p5Data, p6Data, p7Data, p8Data, p9Data,p10Data,p11Data, p12Data, p13Data, p14Data,
	p15Data, p16Data, p17Data, p18Data, p19Data, p20Data, p21Data, p22Data, p23Data, p24Data, p25Data, p26Data, p27Data, p28Data
};
// const unsigned char* gif_cat_c_array[] =
// {
// 	cat_c_1, cat_c_2, cat_c_3, cat_c_4, cat_c_5, cat_c_6, cat_c_7, cat_c_8, cat_c_9
// }; 

static qst_cat_t cat_a = {0};
static qst_cat_t cat_b = {0};

static void gif_cat_clr(void)
{
	oled_clear_data();
}

static void gif_cat_refresh(void)
{
	oled_refresh();
}

static void gif_cat_show_pic(short x, short y, unsigned short width, unsigned short height, const unsigned char pic[])
{
	oled_show_pic(x, y, width, height, pic, 1);
}

static void gif_cat_show_num(short x, short y, unsigned int num, unsigned char len)
{
	oled_show_num(x, y, num, len, FONT_SIZE8, 1);
}

static void git_cat_refresh(qst_cat_t *cat_p)
{
	int pic_i = 0;

	if((cat_p->pic_p == NULL) || (cat_p->pic_max <= 0))
	{
		CAT_LOG("git_cat_refresh error\r\n");
		return;
	}
	cat_p->count += 1;
	// cat_p->pic_i++;
	// cat_p->pic_i = cat_p->pic_i % cat_p->pic_max;
	pic_i = cat_p->count % cat_p->pic_max;
	gif_cat_clr();
#if 1
	#define CAT_CYCLE_MAX	1
	if((cat_p->x+cat_p->pic_x_oft) <= -cat_p->pic_w)
	{
		cat_p->repeat_cnt--;
		if((cat_p->repeat_cnt <= 0))
		{
			cat_p->repeat_cnt = CAT_CYCLE_MAX;
			cat_p->speed = -cat_p->speed;
		}
		else
		{
			cat_p->x = GIF_CAT_SRC_WIDTH-cat_p->pic_x_oft;
		}
	}
	else if((cat_p->x+cat_p->pic_x_oft) >= GIF_CAT_SRC_WIDTH)
	{
		cat_p->repeat_cnt--;
		if((cat_p->repeat_cnt <= 0))
		{
			cat_p->repeat_cnt = CAT_CYCLE_MAX;
			cat_p->speed = -cat_p->speed;
		}
		else
		{
			cat_p->x = -(cat_p->pic_w+cat_p->pic_x_oft);
		}
	}

	cat_p->x += cat_p->speed;
#else
	cat_p->x += cat_p->speed;
#endif

	CAT_LOG("[x = %d, y = %d]\r\n", cat_p->x, cat_p->y);
	oled_set_lcm(cat_p->use_lcm);
	gif_cat_show_pic(cat_p->x, cat_p->y, cat_p->width, cat_p->height, cat_p->pic_p[pic_i]);
	gif_cat_show_num(0, 0, cat_p->count, 8);
	gif_cat_refresh();
}

void git_cata_refresh(void)
{
	git_cat_refresh(&cat_a);
}

static void git_catb_refresh(void)
{
	git_cat_refresh(&cat_b);
}

void git_cat_init(int cat_i)
{
	int odr = 0;

	if(cat_i == CAT_GIF_A)
	{
		memset(&cat_a, 0, sizeof(cat_a));
		cat_a.x = 0;
		cat_a.y = 0;
		cat_a.width = 128;
		cat_a.height = 64;
		cat_a.pic_p = gif_cat_a_array;
		cat_a.pic_max = (unsigned short)(sizeof(gif_cat_a_array) / sizeof(gif_cat_a_array[0]));
		cat_a.speed = 2;

		cat_a.count = 0;
		// cat_a.pic_i = 0;
		cat_a.repeat_cnt = 0;
	
		cat_a.pic_x_oft = GIF_CATA_X_OFT;
		cat_a.pic_w = cat_a.width-2*cat_a.pic_x_oft;

		cat_a.use_lcm = OLED_LCM_0;
		oled_set_lcm(cat_a.use_lcm);
		gif_cat_clr();
		gif_cat_refresh();

		odr = 25;
		evb_setup_timer(TIM2, git_cata_refresh, (1000/odr), ENABLE);
	}
	else if(cat_i == CAT_GIF_B)
	{
		memset(&cat_b, 0, sizeof(cat_b));
		cat_b.x = 0;
		cat_b.y = 32;
		cat_b.width = 128;
		cat_b.height = 32;
		cat_b.pic_p = gif_cat_b_array;
		cat_b.pic_max = (unsigned short)(sizeof(gif_cat_b_array) / sizeof(gif_cat_b_array[0]));
		cat_b.speed = -2;
		cat_b.pic_x_oft = GIF_CATB_X_OFT;
		cat_b.pic_w = GIF_CATB_W;

		cat_b.count = 0;
		// cat_b.pic_i = 0;
		cat_b.repeat_cnt = 0;
	
#if defined(USE_DUAL_OLED)
		cat_b.use_lcm = OLED_LCM_1;
#else
		cat_b.use_lcm = OLED_LCM_0;
#endif
		oled_set_lcm(cat_b.use_lcm);
		gif_cat_clr();
		gif_cat_refresh();

		odr = 20;
		evb_setup_timer(TIM3, git_catb_refresh, (1000/odr), ENABLE);
	}
	// else if(cat_i == CAT_GIF_C)
	// {
	// 	odr = 10;
	// }
	else
	{
		odr = 0;
	}
}
