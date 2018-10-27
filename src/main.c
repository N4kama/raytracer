#include <dlfcn.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "libparser.h"


/* helper function.
 *  Used to format a vector value in a string.
 *
 *  NOTE: we CAN return a local array here, because it's a static variable.
 *  It means it's located not in the stack, but in the .data segment
 *
 *  see: https://en.wikipedia.org/wiki/Data_segment
 */
static const char* vec2str(vec3_t vec)
{
#define BUF_SIZE 128
    static char buffer[BUF_SIZE];

    snprintf(buffer, BUF_SIZE, "%.3f %.3f %.3f",
             vec.x, vec.y, vec.z);

    return buffer;
#undef BUF_SIZE
}

/* the following functions are just basic helpers */
static void dump_light(const struct light *l)
{
    const char *type2str[] = {
        "ambient",
        "point",
        "directional",
    };
    printf("   type:    %s\n", type2str[l->type]);
    printf("   color:   %s\n", vec2str(l->color));
    printf("   vector:  %s\n", vec2str(l->vector));
}

static void dump_mesh(const struct mesh *m)
{
    printf("    tri_count: %zu\n", m->tri_count);
    for (size_t i = 0; i < m->tri_count; i++) {

        printf("\tVTX\t%s\t%s\t%s\n", vec2str(m->vtx[i * 3 + 0]),
                                      vec2str(m->vtx[i * 3 + 1]),
                                      vec2str(m->vtx[i * 3 + 2]));

        if (m->nrm != NULL) {
            printf("\tNRMs\t%s\t%s\t%s\n\n", vec2str(m->nrm[i * 3 + 0]),
                                             vec2str(m->nrm[i * 3 + 1]),
                                             vec2str(m->nrm[i * 3 + 2]));
        }

        if (m->uv != NULL) {
            printf("\tUVs\t%s\t%s\t%s\n\n", vec2str(m->uv[i * 3 + 0]),
                                            vec2str(m->uv[i * 3 + 1]),
                                            vec2str(m->uv[i * 3 + 2]));
        }
    }
}

static void dump_material(const struct material *m)
{
#define PRINT_KEY(Var, Key)     printf("\t" #Key ":\t%.3f\n", (Var)->Key)
    PRINT_KEY(m, specular_level);
    PRINT_KEY(m, shininess);
    PRINT_KEY(m, reflectivity);
    PRINT_KEY(m, opacity);
    PRINT_KEY(m, refr_index);
    PRINT_KEY(m, fog_density);
#undef PRINT_KEY
#define PRINT_KEY(Var, Key) printf("\t" #Key ":\t%s\n", vec2str((Var)->Key))
    PRINT_KEY(m, diffuse);
    PRINT_KEY(m, specular);
    PRINT_KEY(m, fog_color);
#undef PRINT_KEY
#define PRINT_KEY(Var, Key) printf("\t" #Key ":\t%s\n", (Var)->Key)
    PRINT_KEY(m, tex_diffuse);
    PRINT_KEY(m, tex_normal);
    PRINT_KEY(m, tex_opacity);
#undef PRINT_KEY
}

static void dump_object(const struct object *o)
{
#define PRINT_KEY(Var, Key)     printf("\t" #Key ":\t%zu\n", (Var)->Key)
    PRINT_KEY(o, mesh_id);
    PRINT_KEY(o, mtl_id);
#undef PRINT_KEY
#define PRINT_KEY(Var, Key) printf("\t" #Key ":\t%s\n", vec2str((Var)->Key))
    PRINT_KEY(o, position);
    PRINT_KEY(o, rotation);
    PRINT_KEY(o, scale);
#undef PRINT_KEY
}

static void dump_scene(const struct scene *s)
{
    puts("scene:");
    printf("    output:      %dx%d\n", s->width, s->height);
    printf("    mesh count:  %zu\n",   s->mesh_count);
    printf("    light count: %zu\n",   s->light_count);

    puts("camera:");
    printf("    position:  %s\n",   vec2str(s->camera.position));
    printf("    forward:   %s\n",   vec2str(s->camera.forward));
    printf("    up:        %s\n",   vec2str(s->camera.up));
    printf("    fov:       %.3f\n", s->camera.fov);

#define DUMP_ARRAY(Scene, ItemField, CountField, Func)  \
    for (size_t i = 0; i < (Scene)->CountField; i++) {  \
        printf(#ItemField " %zu:\n", i);                \
        Func((Scene)->ItemField + i);                       \
    }
    DUMP_ARRAY(s, lights, light_count, dump_light);
    DUMP_ARRAY(s, meshs, mesh_count, dump_mesh);
    DUMP_ARRAY(s, mtls, mtl_count, dump_material);
    DUMP_ARRAY(s, objects, object_count, dump_object);
#undef DUMP_ARRAY
}

static void dump_scene(const struct scene *s, char* path)
{
    
}

/* out entry point. You'll have to uncomment the render_scene call */
int main(int argc, char** argv)
{
    if (argc != 3) {
        warnx("usage: %s scene.json output.ppm\n", argv[0]);
        return -1;
    }

    /* loads the scene, then dump it */
    struct scene *s = scene_parser.load(argv[1]);
    if (NULL == s) {
        warnx("invalid scene file");
        return -1;
    }

    // your raytracer goes here
    render_scene(s, argv[2]);
    dump_scene(s);

    /* free the scene */
    scene_parser.free(s);
    return 0;
}
