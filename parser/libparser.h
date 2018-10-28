#pragma once

#define LIBPARSER_INCLUDED
#include <stddef.h>

typedef struct
{
    double x;
    double y;
    double z;
} vec3_t;

struct camera
{
    vec3_t position;    /* world-position of the camera             */
    vec3_t forward;     /* the forward unit vector for the camera   */
    vec3_t up;          /* the up unit vector for the camera        */
    double fov;         /* focal of the camera, in degree           */
};

enum type_light
{
    AMBIENT,
    POINT,
    DIRECTIONAL,
};

struct light
{
    enum type_light type;   /* type of the light (see enum above)         */
    vec3_t color;           /* color of the light, values in [ 0.0, 1.0 ] */

    /* the field 'vector' depends on the type:
     *  - AMBIENT:      not used, undefined value
     *  - POINT:        the position of the light
     *  - DIRECTIONAL:  the direction of the light
     */
    vec3_t vector;
};

struct material {
    /* stage 0 */
    vec3_t diffuse;           /* diffuse color, values in [ 0.0, 1.0 ] */

    /* Needed for stage 1:
     *
     * specular: color of the specular highlight: range [0. ; 1.]
     * specular_level: strengh of the specular highlight. range [0. ; 1.]
     *      MORE: specular highlight is more visible
     *      LESS: specular highlight is less visible
     * shininess: control width of the specular highlight. range [0. ; 1.]
     *            MORE = smaller but powerful bright spot
     *            LESS = large and diffuse bright spot
     *              (think glossy plastic vs concrete)
     *
     * see: https://en.wikipedia.org/wiki/Specular_highlight
     *
     *
     * reflectivity: MORE = clear reflections can be seen
     *               LESS = small/no reflection are seen
     *                  (think mirror vs concrete)
     */
    vec3_t specular;
    double specular_level;
    double shininess;
    double reflectivity;

    /* Needed for stage 2
     *
     * opacity: control the transparency
     *          MORE = less transparency
     *          LESS = more transparency
     *              (think concrete vs glass)
     * refr_index: refraction index.
     *  oustide is considered 1 (air)
     *  see: https://en.wikipedia.org/wiki/Refractive_index
     */
    double opacity;
    double refr_index;

    /* Needed for bonuses
     *
     *  fog_color:      the color of the inside of the object
     *  fog_density: the power of the fog inside.
     *      MORE = cloudy material
     *      LESS = clear material
     *          (think tinted glass vs clear glass)
     * see: https://en.wikipedia.org/wiki/Transparency_and_translucency
     *
     * tex_diffuse: the path to the diffuse ppm texture
     * tex_normal: the path to the normal-map ppm texture
     *      see: https://en.wikipedia.org/wiki/Normal_mapping
     * tex_opacity: the path to the opacity-map ppm texture
     *      see: http://wiki.polycount.com/wiki/Transparency_map
     */
    vec3_t fog_color;
    double fog_density;
    char *tex_diffuse;
    char *tex_normal;
    char *tex_opacity;
};

struct mesh {
    /*
     * tri_count: the triangle count in the mesh
     * vtx: an array of vertices.
     *      - There is n * 3 vertices
     *      - each group of 3 vertices form a triangle
     *      - they are sorted clockwise
     *          - A, B, C three vertices
     *          - triangle T = {A, B, C}
     *          - normal(T) = cross(AC, AB);
     *          - -normal(T) = cross(AB, AC);
     *
     *          This is important for back-face culling
     *          see: https://en.wikipedia.org/wiki/Back-face_culling
     *
     *  WARNING: vertex position is a model-position.
     *           You'll have to convert them to a world-position
     * nrm: an array of normals.
     *  WARNING: if this array is NULL, the model has no
     *           normals. You'll have to compute them yourself
     *           (cross product with a, b and c, three vertices)
     */
    size_t tri_count;
    vec3_t *vtx;
    vec3_t *nrm;

     /* Needed for bonuses
     *  uv: an array of UV coordinates
     *      - There is n * 3 uv coordinates
     *      - 1 uv coordinate per vertice
     *      - x is equivalent to u
     *      - y is equivalenet to v
     *      - z is unused, but should be w
     *  see: https://en.wikipedia.org/wiki/UV_mapping
     *  WARNING: if this array is NULL, no UV coordinates
     */
    vec3_t *uv;
};

struct object {
    /* the world-position of the object, needed to convert mesh's vtx pos */
    vec3_t position;

    /* the world-rotation of the object */
    vec3_t rotation;

    /* the world-scale of the object */
    vec3_t scale;

    /* the index of the mesh in scene->objects array */
    size_t mesh_id;

    /* the index of the material in scene->mtls array */
    size_t mtl_id;
};

struct scene
{
    /* height: height in pixel of the output */
    int height;

    /* width: width in pixel of the output */
    int width;

    /* the camera for this scene */
    struct camera camera;

    /*
     * Example:
     * Imagine a scene with 2 cubes
     *  - a RED cube, in the middle, 1mx1x
     *  - a GREEN rectangle, above the red one, and rotated 45° on Y axis
     *
     *  You'll find:
     *      - one mesh:
     *          0 - a simple cube (1m x 1m)
     *      - two materials:
     *          - 0: diffuse=red
     *          - 1: diffuse=green
     *      - two objects:
     *          - cube-1
     *              - mtl_id = 0
     *              - mesh_id = 0
     *              - position: (0, 0, 0)
     *              - rotation: (0, 0, 0)
     *              - scale:    (1, 1, 1)
     *          - cube-2
     *              - mtl_id = 1
     *              - mesh_id = 0
     *              - position: (0, 1, 0)
     *              - rotation: (0, 45, 0)   rotate 45° on Y axis
     *              - scale:    (2, 1, 1)    scale x2 on X axis
     */

    /* the array of mesh used in this scene */
    struct mesh *meshs;
    size_t mesh_count;

    /* the array of lights present in this scene */
    struct light* lights;
    size_t light_count;

    /* the array of materials used in this scene */
    struct material *mtls;
    size_t mtl_count;

    /* the array of objects present in the scene */
    struct object *objects;
    size_t object_count;
};

/*
 *  the interface available for the parser
 *   see: src/main.c
 */

typedef struct {
    struct scene* (*load)(const char*);
    void  (*free)(struct scene*);
    void *usable_ptr;

} parser_t;

/* the parser interface is a global exposed by libparser.so */
extern parser_t scene_parser;
