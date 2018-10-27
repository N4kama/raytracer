#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "img.h"

struct img *init_img(size_t w, size_t h)
{
    struct img *img = malloc(sizeof(struct img)); //toFree
    if (!img)
    {
        return NULL;
    }
    img->height = h;
    img->width = w;
    img->pixels = calloc(w * h, sizeof(struct pixel)); //toFree
    if (!img->pixels)
    {
        free(img); //(init_img);
        return NULL;
    }
    return img;
}

unsigned int create_img(char *path, struct img *img)
{
    if (!path || !img)
    {
        return 0;
    }
    FILE *f = fopen(path, "w"); //to close
    if (!f)
    {
        return 0;
    }
    fprintf(f, "P3\n");
    fprintf(f, "%ld", img->width);
    fprintf(f, " %ld\n", img->height);
    fprintf(f, "255\n");
    for (size_t y = 0; y < img->height; y++)
    {
        for (size_t x = 0; x < img->width; x++)
        {
            fprintf(f, "%d ", img->pixels[y * img->width + x].r);
            fprintf(f, "%d ", img->pixels[y * img->width + x].g);
            if (x == img->width - 1)
            {
                fprintf(f, "%d\n", img->pixels[y * img->width + x].b);
            }
            else
            {
                fprintf(f, "%d  ", img->pixels[y * img->width + x].b);
            }
        }
    }
    fclose(f); //create_img
    return 1;
}

struct pixel init_pixel(unsigned char r, unsigned char g, unsigned char b)
{
    struct pixel p =
        {
            r, g, b};
    return p;
}

struct img *set_pixel(size_t x, size_t y, struct img *img, struct pixel p)
{
    if (!img)
    {
        return NULL;
    }
    img->pixels[y * img->width + x].r = p.r;
    img->pixels[y * img->width + x].g = p.g;
    img->pixels[y * img->width + x].b = p.b;
    return img;
}
/*
int main(void)
{
    struct img *omg = init_img(500, 500);
    for (unsigned int i = 0; i < 500; i++)
    {
        for (unsigned int j = 0; j < 500; j++)
        {
            set_pixel(i, j, omg, init_pixel(255, 0, 0));
        }
    }
    return create_img("wowowowow.ppm", omg);
}*/