#ifndef STRUCT_H
#define STRUCT_H
#include "libparser.h"

struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct img
{
    size_t width;
    size_t height;
    struct pixel *pixels;
};

struct vec2_t
{
    double x;
    double y;
};

struct triangle
{
    vec3_t a;
    vec3_t b;
    vec3_t c;
};

struct ray
{
    vec3_t origin;
    vec3_t dir;
};

struct hit
{
    int touched;
    struct object obj;
    struct triangle triangle;
    vec3_t impact_point;
};

#endif /* STRUCT_H */
