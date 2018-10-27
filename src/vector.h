#ifndef VECTOR_H
# define VECTOR_H

double get_vector_length(vec3_t v);
vec3_t normalize_vector(vec3_t vect);
int equal_vectors(vec3_t a, vec3_t b);
vec3_t sum_vectors(vec3_t a, vec3_t b);
double dot_vectors(vec3_t a, vec3_t b);
vec3_t cross_vectors(vec3_t a, vec3_t b);
vec3_t mul_vectors(vec3_t a, double b);
vec3_t sub_vectors(vec3_t a, vec3_t b);
vec3_t div_vectors(vec3_t a, double b);
vec3_t get_normal_vector(vec3_t a, vec3_t b);

#endif /* VECTOR_H */