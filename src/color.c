#include "color.h"

struct pixel get_object_color(struct scene *s, struct hit hit)
{
    struct material mat = s->mtls[hit.obj.mtl_id];
    return init_pixel(mat.diffuse.x * 255, mat.diffuse.y * 255, mat.diffuse.z * 255);
}