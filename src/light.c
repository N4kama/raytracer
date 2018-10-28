#include "light.h"

vec3_t adjust_vec(vec3_t vec)
{
    vec.x = vec.x > 1.0 ? 1.0 : vec.x;
    vec.y = vec.y > 1.0 ? 1.0 : vec.y;
    vec.z = vec.z > 1.0 ? 1.0 : vec.z;
    vec.x = vec.x < 0.0 ? 0.0 : vec.x;
    vec.y = vec.y < 0.0 ? 0.0 : vec.y;
    vec.z = vec.z < 0.0 ? 0.0 : vec.z;
    return vec;
}

vec3_t compute_dir_light(struct hit hit, struct light light,
                         struct material mat)
{
    light.vector.y *= -1;
    vec3_t l = mul_vectors(normalize_vector(light.vector), -1);
    vec3_t ac = sub_vectors(hit.triangle.c, hit.triangle.a);
    vec3_t ab = sub_vectors(hit.triangle.b, hit.triangle.a);
    vec3_t normal = normalize_vector(mul_vectors(get_normal_vector(ab, ac),
                                                 1));
    double ld = dot_product(l, normal);
    ld = ld < 0.0 ? 0 : ld;
    vec3_t res = mul_vectors(mat.diffuse, ld);
    res = scale_vectors(light.color, res);
    return res;
}

vec3_t compute_point_light(struct hit hit, struct light light,
                         struct material mat)
{
    vec3_t r = sub_vectors(hit.impact_point, light.vector);
    r = normalize_vector(r);
    vec3_t l = mul_vectors(r, -1);
    vec3_t ac = sub_vectors(hit.triangle.c, hit.triangle.a);
    vec3_t ab = sub_vectors(hit.triangle.b, hit.triangle.a);
    vec3_t normal = normalize_vector(mul_vectors(get_normal_vector(ab, ac),
                                                 1));
    double ld = dot_product(l, normal);
    ld = ld < 0.0 ? 0 : ld;
    vec3_t res = mul_vectors(mat.diffuse, ld);
    res = scale_vectors(light.color, res);
    return res;
}

vec3_t get_lightning(struct scene *s, struct hit hit, unsigned m)
{
    vec3_t res =
    {
        0, 0, 0
    };
    vec3_t cur = res;
    struct material mat = s->mtls[m];
    for (size_t i = 0; i < s->light_count; i++)
    {
        struct light light = s->lights[i];
        if (light.type == POINT)
        {
            cur = compute_point_light(hit, light, mat);
        }
        else
        {
            if (light.type == DIRECTIONAL)
            {
                cur = compute_dir_light(hit, light, mat);
            }
            else
            {
                cur = scale_vectors(mat.diffuse, light.color);
            }
        }
        res = sum_vectors(cur, res);
    }
    res = adjust_vec(res);
    return res;
}