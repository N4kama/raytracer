#include "libparser.h"
#include "vector.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LARGE_DOUBLE 999999999

struct triangle
{
    vec3_t a;
    vec3_t b;
    vec3_t c;
};

struct ray
{
    vec3_t origin;
    vec3_t dir;
};

struct hit
{
    struct triangle triangle;
    vec3_t impact_point;
};

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

vec3_t nearest_vect(vec3_t a, vec3_t b, vec3_t o)
{
    int l_a = get_vector_length(sub_vectors(a, o));
    int l_b = get_vector_length(sub_vectors(b, o));
    return (l_a > l_b) ? b : a;
}

void foreach_pixel(struct img *img, struct scene *s)
{
    vec3_t def_vect = {LARGE_DOUBLE, LARGE_DOUBLE, LARGE_DOUBLE};
    struct triangle def_triangle = get_triangle(def_vect, def_vect, def_vect);
    vec3_t *vertices = s->meshs->vtx;
    size_t count = s->meshs->tri_count;
    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            struct vec2_t coords = {j, i};
            struct vec2_t w_h = {s->width, s->height};
            struct ray r = get_ray(s->camera.position,
                                   get_pixel_pos(s->camera, coords, w_h));
            struct hit imp = nearest_intersection(vertices, count, r);
            if (!equal_vectors(def_vect, imp.impact_point))
            {
                //set_pixel();
            }
        }
    }
}

struct hit nearest_intersection(vec3_t *vert, size_t n, struct ray r)
{
    vec3_t contact = {LARGE_DOUBLE, LARGE_DOUBLE, LARGE_DOUBLE};
    vec3_t best = contact;
    struct triangle t;
    struct triangle best_triangle = get_triangle(contact, contact, contact);
    for (int i = 0; i < n; i++)
    {
        t = get_triangle(vert[i * 3 + 0], vert[i * 3 + 1], vert[i * 3 + 2]);
        contact = hit_triangle(t, r);
        if (!equal_vectors(nearest_vect(contact, best, r.origin), contact))
        {
            best = contact;
            best_triangle = t;
        }
    }
    struct hit res = {best_triangle, best};
    return res;
}

vec3_t hit_triangle(struct triangle t, struct ray r)
{
    vec3_t res = {LARGE_DOUBLE, LARGE_DOUBLE, LARGE_DOUBLE};
    vec3_t edge_a = sub_vectors(t.a, t.c);
    vec3_t edge_b = sub_vectors(t.b, t.a);
    vec3_t edge_c = sub_vectors(t.c, t.b);
    vec3_t plan_n = get_normal_vector(edge_b, edge_c);
    double len_n = get_vector_length(plan_n);

    if (fabs(dot_vectors(plan_n, r.dir)) < 0.01) //change 0.01
    {
        return res;
    }
    double val = dot_vectors(plan_n, t.a);
    double touch = dot_vectors(plan_n, r.origin) / dot_vectors(plan_n, r.dir);
    if (touch < 0)
    {
        return res;
    }
    vec3_t inter = sum_vectors(r.origin, mul_vectors(r.dir, touch));

    vec3_t edge_inter = sub_vectors(inter, t.a);
    if (dot_vectors(plan_n, cross_vectors(edge_b, edge_inter)) < 0)
    {
        return res;
    }
    edge_inter = sub_vectors(inter, t.b);
    if (dot_vectors(plan_n, cross_vectors(edge_c, edge_inter)) < 0)
    {
        return res;
    }
    edge_inter = sub_vectors(inter, t.c);
    if (dot_vectors(plan_n, cross_vectors(edge_a, edge_inter)) < 0)
    {
        return res;
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