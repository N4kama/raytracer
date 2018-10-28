#ifndef LIGHT_H
#define LIGHT_H

#include "mesh.h"

vec3_t get_lightning(struct scene *s, struct hit hit, unsigned m);

#endif /* LIGHT_H */
