
#ifndef __GIF_CAT_H__
#define __GIF_CAT_H__

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    GIF_CAT_A = 0,
    GIF_CAT_B,
    GIF_CAT_MAX
};

extern void git_cat_init(int cat_i);

#ifdef __cplusplus
}
#endif

#endif
