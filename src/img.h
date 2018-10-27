#ifndef IMG_H
#define IMG_H
#include "struct.h"

struct img *init_img(size_t w, size_t h);
unsigned int create_img(char *path, struct img *img);
struct pixel init_pixel(unsigned char r, unsigned char g, unsigned char b);
struct img *set_pixel(size_t x, size_t y, struct img *img, struct pixel p);

#endif /* IMG_H */