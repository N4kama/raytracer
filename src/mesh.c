#include "mesh.h"

#define LARGE_DOUBLE 999999999

vec3_t nearest_vect(vec3_t a, vec3_t b, vec3_t o)
{
    int l_a = get_vector_length(sub_vectors(a, o));
    int l_b = get_vector_length(sub_vectors(b, o));
    return (l_a > l_b) ? b : a;
}

struct ray get_ray(vec3_t o, vec3_t d)
{
    struct ray r;
    r.origin = o;
    r.dir = d;
    return r;
}

struct triangle get_triangle(vec3_t a, vec3_t b, vec3_t c)
{
    struct triangle t;
    t.a = a;
    t.b = b;
    t.c = c;
    return t;
}

void print_ray_and_hit(struct ray r, struct hit imp)
{
    if (imp.touched)//)imp.obj.position.x || imp.obj.position.y || imp.obj.position.z)
    {
        printf("\tShooting ray : Origin at (%f, %f, %f) -- Camera\n\t      Direction at (%f, %f, %f) -- ray\n",
               r.origin.x, r.origin.y, r.origin.z, r.dir.x, r.dir.y, r.dir.z);
        printf("...\n\tRay hitted triangle at coordinates (%f, %f, %f)\n   \tHitted point is in between the triangle at coordinates :\n(%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n   \tObject itself is at coordinates : (%f, %f, %f)\n", imp.impact_point.x, imp.impact_point.y, imp.impact_point.z, imp.triangle.a.x, imp.triangle.a.y, imp.triangle.a.z, imp.triangle.b.x, imp.triangle.b.y, imp.triangle.b.z, imp.triangle.c.x, imp.triangle.c.y, imp.triangle.c.z, imp.obj.position.x, imp.obj.position.y, imp.obj.position.z);
        //char c = ' ';
        //int a = scanf(" %c", &c);
        //printf("%c%d\r", c, a);
    }
}

void foreach_pixel(struct img *img, struct scene *s)
{
    for (unsigned i = 0; i < img->height; i++)
    {
        for (unsigned j = 0; j < img->width; j++)
        {
            //printf("Starting processing pixels (%uc, %uc) of the camera\n", j, i);
            for (unsigned m = 0; m < s->mesh_count; m++)
            {
                vec3_t *vertices = s->meshs[m].vtx;
                size_t count = s->meshs[m].tri_count;
                struct object o = s->objects[m];
                struct vec2_t coords = {j, i};
                struct vec2_t w_h = {s->width, s->height};
                struct ray r = get_ray(s->camera.position,
                                       get_pixel_pos(s->camera, coords, w_h));
                struct hit imp = nearest_intersection(vertices, count, r, o);
                //print_ray_and_hit(r, imp); //DEBUG
                if (imp.touched)
                {
                    struct pixel pixel = get_object_color(s, imp);
                    printf("Index : (%d, %d) with color : (%u, %u, %u)\n", j, i, pixel.r, pixel.g, pixel.b);
                    set_pixel(j, i, img, pixel);
                }
            }
        }
    }
}

struct hit nearest_intersection(vec3_t *vert, size_t n, struct ray r, struct object o)
{
    vec3_t contact = {-1, -1, -1};
    struct triangle best_triangle = get_triangle(contact, contact, contact);
    vec3_t *best = NULL;
    struct triangle t;
    int touch = 0;
    for (size_t i = 0; i < n; i++)
    {
        vec3_t a = scale_vectors(o.scale, sum_vectors(o.position, vert[i * 3 + 0]));
        vec3_t b = scale_vectors(o.scale, sum_vectors(o.position, vert[i * 3 + 1]));
        vec3_t c = scale_vectors(o.scale, sum_vectors(o.position, vert[i * 3 + 2]));
        t = get_triangle(a, b, c);
        vec3_t *hitted = hit_triangle(t, r);
        if (!hitted)
        {
            continue;
        }
        if (!best || equal_vectors(nearest_vect(contact, *best, r.origin), contact))
        {
            best = hitted;
            best_triangle = t;
            touch = 1;
        }
    }
    if (!best || !touch)
    {
        struct hit res = {0, o, best_triangle, contact};
        return res;
    }
    struct hit res = {touch, o, best_triangle, *best};
    return res;
}

vec3_t *hit_triangle(struct triangle t, struct ray r)
{
    vec3_t edge_a = sub_vectors(t.a, t.c);
    vec3_t edge_b = sub_vectors(t.b, t.a);
    vec3_t edge_c = sub_vectors(t.c, t.b);
    vec3_t plan_n = get_normal_vector(edge_b, edge_c);
    //double len_n = get_vector_length(plan_n);

    if (fabs(dot_vectors(plan_n, r.dir)) < 0.01) //change 0.01
    {
        return NULL;
    }
    double val = dot_vectors(plan_n, t.a);
    double touch = (dot_vectors(plan_n, r.origin) + val) / dot_vectors(plan_n, r.dir);
    if (touch < 0)
    {
        return NULL;
    }
    vec3_t *inter = malloc(sizeof(vec3_t));
    *inter = sum_vectors(r.origin, mul_vectors(r.dir, touch));

    vec3_t edge_inter = sub_vectors(*inter, t.a);
    if (dot_vectors(plan_n, cross_vectors(edge_b, edge_inter)) < 0)
    {
        return NULL;
    }
    edge_inter = sub_vectors(*inter, t.b);
    if (dot_vectors(plan_n, cross_vectors(edge_c, edge_inter)) < 0)
    {
        return NULL;
    }
    edge_inter = sub_vectors(*inter, t.c);
    if (dot_vectors(plan_n, cross_vectors(edge_a, edge_inter)) < 0)
    {
        return NULL;
    }
    return inter;
}

vec3_t get_nearest_vertex(struct triangle triangle, vec3_t impact_ray)
{
    vec3_t res = triangle.a;
    if (get_vector_length(sub_vectors(res, impact_ray)) > get_vector_length(sub_vectors(triangle.b, impact_ray)))
    {
        res = triangle.b;
    }
    if (get_vector_length(sub_vectors(res, impact_ray)) > get_vector_length(sub_vectors(triangle.c, impact_ray)))
    {
        res = triangle.c;
    }
    return res;
}