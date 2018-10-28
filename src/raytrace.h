#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "mesh.h"

struct pixel get_object_color(struct scene *s, struct hit hit, vec3_t light);

#endif /* RAYTRACE_H */