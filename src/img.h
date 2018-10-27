#ifndef IMG_H
#define IMG_H
#include "struct.h"
#include "vector.h"

struct img *init_img(size_t w, size_t h);
unsigned int create_img(char *path, struct img *img);
struct pixel init_pixel(unsigned char r, unsigned char g, unsigned char b);
struct img *set_pixel(size_t x, size_t y, struct img *img, struct pixel p);
vec3_t get_pixel_pos(struct camera cam, struct vec2_t coords,
                     struct vec2_t w_h);
#endif /* IMG_H */