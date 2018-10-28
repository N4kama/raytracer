#include "color.h"

struct pixel get_object_color(struct scene *s, struct hit hit, vec3_t light)
{
    struct material mat = s->mtls[hit.obj.mtl_id];
    return init_pixel(light.x * mat.diffuse.x * 255,
                      light.y * mat.diffuse.y * 255,
                      light.z * mat.diffuse.z * 255);
}