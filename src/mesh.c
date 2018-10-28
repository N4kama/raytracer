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

static vec3_t vector_uni(void)
{
    vec3_t res = {1, 1, 1};
    return res;
}

struct hit nearest_intersection(vec3_t *vert, size_t n, struct ray r,
                                struct object o)
{
    vec3_t contact = vector_uni();
    struct triangle best_triangle = get_triangle(contact, contact, contact);
    vec3_t *best = NULL;
    struct triangle t;
    for (size_t i = 0; i < n; i++)
    {
        vec3_t ori = o.position;
        vec3_t a = scale_vectors(o.scale, sum_vectors(ori, vert[i * 3 + 0]));
        vec3_t b = scale_vectors(o.scale, sum_vectors(ori, vert[i * 3 + 1]));
        vec3_t c = scale_vectors(o.scale, sum_vectors(ori, vert[i * 3 + 2]));
        t = get_triangle(a, b, c);
        vec3_t *hitted = hit_triangle(t, r);
        if (hitted)
        {
            if (!best || equal_vectors(nearest_vect(*hitted, *best, r.origin),
                                       *hitted))
            {
                free(best);
                best = hitted;
                best_triangle = t;
            }
            else
            {
                free(hitted);
            }
        }
    }
    if (!best)
    {
        struct hit res = {0, o, best_triangle, contact};
        return res;
    }
    struct hit res = {1, o, best_triangle, *best};
    free(best);
    return res;
}

vec3_t *hit_triangle(struct triangle t, struct ray r)
{
    float thres = 0.00000001;

    vec3_t edge_b = sub_vectors(t.b, t.a);
    vec3_t edge_c = sub_vectors(t.c, t.a);

    vec3_t plan_n = cross_vectors(r.dir, edge_c);

    double divid = 1.0 / dot_vectors(edge_b, plan_n);
    vec3_t a_v = sub_vectors(r.origin, t.a);
    double touch = dot_vectors(a_v, plan_n) * divid;
    if (touch < 0 || touch > 1)
    {
        return NULL;
    }
    vec3_t b_v = cross_vectors(a_v, edge_b);
    double touch2 = dot_vectors(r.dir, b_v) * divid;
    if (touch2 < 0 || touch + touch2 > 1)
    {
        return NULL;
    }
    double touch3 = dot_vectors(edge_c, b_v) * divid;
    if (touch3 > thres)
    {
        vec3_t *inter = malloc(sizeof(vec3_t));
        *inter = sum_vectors(r.origin, mul_vectors(r.dir, touch3));
        return inter;
    }
    return NULL;
}

vec3_t get_nearest_vertex(struct triangle triangle, vec3_t impact_ray)
{
    vec3_t res = triangle.a;
    if (get_vector_length(sub_vectors(res, impact_ray))
        > get_vector_length(sub_vectors(triangle.b, impact_ray)))
    {
        res = triangle.b;
    }
    if (get_vector_length(sub_vectors(res, impact_ray))
        > get_vector_length(sub_vectors(triangle.c, impact_ray)))
    {
        res = triangle.c;
    }
    return res;
}