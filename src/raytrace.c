#include "raytrace.h"

void foreach_pixel(struct img *img, struct scene *s)
{

    struct vec2_t w_h = {s->width, s->height};
    for (unsigned i = 0; i < img->height; i++)
    {
        for (unsigned j = 0; j < img->width; j++)
        {
            struct vec2_t coords = {j, i};
            struct ray r = get_ray(s->camera.position,
                                   get_pixel_pos(s->camera, coords, w_h,
                                                 s->camera.fov));

            for (unsigned m = 0; m < s->object_count; m++)
            {
                struct object o = s->objects[m];
                size_t mesh_id = o.mesh_id;
                vec3_t *vertices = s->meshs[mesh_id].vtx;
                size_t count = s->meshs[mesh_id].tri_count;

                struct hit imp = nearest_intersection(vertices, count, r, o);
                if (imp.touched == 1)
                {
                    vec3_t light = get_lightning(s, imp, o.mtl_id);
                    struct pixel pixel = get_object_color(s, imp, light);
                    set_pixel(j, i, img, pixel);
                }
            }
        }
    }
}