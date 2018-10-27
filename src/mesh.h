#ifndef MESH_H
#define MESH_H
#include "libparser.h"

vec3_t nearest_vect(vec3_t a, vec3_t b, vec3_t o);
void foreach_pixel(struct img *img, struct scene *s);
struct hit nearest_intersection(vec3_t *vert, size_t n, struct ray r);
vec3_t hit_triangle(struct triangle t, struct ray r);
vec3_t get_nearest_vertex(struct triangle triangle, vec3_t impact_ray);

#endif /* MESH_H */