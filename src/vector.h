#ifndef VECTOR_H
#define VECTOR_H

#include "libparser.h"
#include "struct.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

double get_vector_length(vec3_t v);
vec3_t normalize_vector(vec3_t vect);
int equal_vectors(vec3_t a, vec3_t b);
vec3_t sum_vectors(vec3_t a, vec3_t b);
double dot_vectors(vec3_t a, vec3_t b);
vec3_t cross_vectors(vec3_t a, vec3_t b);
vec3_t mul_vectors(vec3_t a, double b);
vec3_t sub_vectors(vec3_t a, vec3_t b);
vec3_t div_vectors(vec3_t a, double b);
double dot_product(vec3_t a, vec3_t b);
vec3_t get_normal_vector(vec3_t a, vec3_t b);
vec3_t scale_vectors(vec3_t a, vec3_t b);
vec3_t rotate_vectors(vec3_t a, vec3_t b);

#endif /* VECTOR_H */