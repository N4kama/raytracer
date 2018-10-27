#include "libparser.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"

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

vec3_t scale_vectors(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = a.x * b.x;
    res.y = a.y * b.y;
    res.z = a.z * b.z;
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

vec3_t get_normal_vector(vec3_t a, vec3_t b)
{
    vec3_t res;
    res.x = (a.y * b.z) - (b.y * a.z);
    res.y = (a.x * b.z) - (b.x * a.z);
    res.z = (a.x * b.y) - (b.x * a.y);
    return res;//normalize_vector(res);
}