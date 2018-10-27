#include "libparser.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct vec2_t
{
    double x;
    double y;
};

double get_vector_length(vec3_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t normalize_vector(vec3_t vect)
{
    double len = get_vector_length(vect);
    vect.x /= len;
    vect.y /= len;
    vect.z /= len;
    return vect;
}

int equal_vectors(vec3_t a, vec3_t b)
{
    int res = 1;
    res &= a.x == b.x;
    res &= a.y == b.y;
    res &= a.z == b.z;
    return res;
}

vec3_t sum_vectors(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;
    return res;
}

double dot_vectors(vec3_t a, vec3_t b)
{
    double res = 0;
    res += a.x * b.x;
    res += a.y * b.y;
    res += a.z * b.z;
    return res;
}

vec3_t cross_vectors(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
    return res;
}

vec3_t mul_vectors(vec3_t a, double b)
{
    vec3_t res;
    res.x = a.x * b;
    res.y = a.y * b;
    res.z = a.z * b;
    return res;
}

vec3_t sub_vectors(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;
    return res;
}

vec3_t div_vectors(vec3_t a, double b)
{
    vec3_t res;
    res.x = a.x / b;
    res.y = a.y / b;
    res.z = a.z / b;
    return res;
}

vec3_t get_pixel_pos(struct camera cam, struct vec2_t coords, struct vec2_t w_h)
{
    vec3_t m = sum_vectors(cam.position, cam.forward); //middle of the screen
    double factor_1 = (w_h.x / 2 - coords.y) / (w_h.x / 2);
    double factor_2 = (coords.x - w_h.y / 2) / (w_h.y / 2);
    vec3_t u = mul_vectors(cam.up, factor_1);
    vec3_t r = get_normal_vector(cam.forward, cam.up);
    r = mul_vectors(r, factor_2);
    vec3_t res = sum_vectors(m, u);
    return sub_vectors(res, r);
}

vec3_t get_normal_vector(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = (a.y * b.z) - (b.y * a.z);
    res.y = (a.x * b.z) - (b.x * a.z);
    res.z = (a.x * b.y) - (b.x * a.y);
    return res;//normalize_vector(res);
}