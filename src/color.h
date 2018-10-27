#ifndef COLOR_H
#define COLOR_H

#include "struct.h"
#include "img.h"

struct pixel get_object_color(struct scene *s, struct hit hit);

#endif /* COLOR_H */