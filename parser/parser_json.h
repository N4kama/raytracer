#ifndef PARSER_JSON_H_
#define PARSER_JSON_H_

#include <stdio.h>

#include "jsmn.h"
#include "libparser.h"
#include "obj_parser.h"

struct json_file
{
    const char* filename;
    const char* content;
};

jsmntok_t* get_object_in_json(const char* data, jsmntok_t* token,
                              const struct json_file* f);
char* get_all_content(FILE* file, const char* filename);
jsmntok_t* get_object_in_json(const char* data, jsmntok_t* token,
                              const struct json_file* f);
int pass_object(jsmntok_t* token);

int parse_int(jsmntok_t* token, const struct json_file* f, int *out);
int parse_double(jsmntok_t* token, const struct json_file* f, double *out);
int parse_vector(jsmntok_t* token, const struct json_file* f, vec3_t *out);

#endif /* ! PARSER_JSON_H_ */
