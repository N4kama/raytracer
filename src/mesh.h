#ifndef MESH_H
#define MESH_H

#include "color.h"
#include "light.h"
#include "struct.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

vec3_t nearest_vect(vec3_t a, vec3_t b, vec3_t o);
struct ray get_ray(vec3_t o, vec3_t d);
struct triangle get_triangle(vec3_t a, vec3_t b, vec3_t c);
void foreach_pixel(struct img *img, struct scene *s);
struct hit nearest_intersection(vec3_t *vert, size_t n, struct ray r,
                                struct object o);
vec3_t *hit_triangle(struct triangle t, struct ray r);
vec3_t nearest_vertex(struct triangle triangle, vec3_t impact_ray);

#endif /* MESH_H */