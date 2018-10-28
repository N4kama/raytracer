#include "img.h"

struct img *init_img(size_t w, size_t h)
{
    struct img *img = malloc(sizeof(struct img));
    if (!img)
    {
        return NULL;
    }
    img->height = h;
    img->width = w;
    img->pixels = calloc(w * h, sizeof(struct pixel));
    if (!img->pixels)
    {
        free(img);
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
    FILE *f = fopen(path, "w");
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
    fclose(f);
    return 1;
}

struct pixel init_pixel(unsigned char r, unsigned char g, unsigned char b)
{
    struct pixel p =
    {
        r, g, b
    };
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

vec3_t get_pixel_pos(struct camera cam, struct vec2_t coords,
                     struct vec2_t w_h, double fov)
{
    double ratio_x = w_h.x / w_h.y;
    double ratio_y = w_h.y / w_h.x;
    if (ratio_x > ratio_y)
    {
        ratio_y = -1;
    }
    else
    {
        ratio_x = 1;
        ratio_y *= -1;
    }
    double pi = 3.14159265359;
    double px = ((coords.x / w_h.x) - 0.5) * ratio_x * tan(fov * pi / 180 / 2);
    double py = ((coords.y / w_h.y) - 0.5) * ratio_y * tan(fov * pi / 180 / 2);
    vec3_t m = sum_vectors(cam.position, cam.forward);
    vec3_t u = mul_vectors(cam.up, py);
    vec3_t r = get_normal_vector(cam.up, cam.forward);
    r = mul_vectors(r, px);
    vec3_t res = sum_vectors(m, u);
    res = sum_vectors(res, r);
    return sub_vectors(res, cam.position);
}