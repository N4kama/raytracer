#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libparser.h"

void sum_vectors(vec3_t *res, vec3_t *a, vec3_t *b)
{
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void mul_vectors(vec3_t *res, vec3_t *a, double b)
{
    res->x = a->x * b;
    res->y = a->y * b;
    res->z = a->z * b;
}

void sub_vectors(vec3_t *res, vec3_t *a, vec3_t *b)
{
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

void div_vectors(vec3_t *res, vec3_t *a, double b)
{
    res->x = a->x / b;
    res->y = a->y / b;
    res->z = a->z / b;
}