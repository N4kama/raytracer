#ifndef COLOR_H
#define COLOR_H

#include "img.h"
#include "struct.h"

struct pixel get_object_color(struct scene *s, struct hit hit, vec3_t light);

#endif /* COLOR_H */