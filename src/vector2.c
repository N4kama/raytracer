#include "vector.h"

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