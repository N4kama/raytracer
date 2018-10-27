#include "struct.h"
#include "img.c"

struct pixel get_object_color(struct scene *s, struct hit hit)
{
    struct material mat = s->mtls[hit.obj->mtl_id];
    return init_pixel(mat.diffuse)
}