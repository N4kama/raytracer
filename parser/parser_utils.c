#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "parser_json.h"

char* get_all_content(FILE* file, const char* filename)
{
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(fsize + 1);
    if (fread(content, fsize, 1, file) == (size_t)-1)
    {
        free(content);
        fclose(file);
        err(1, "Cannot read file '%s'", filename);
    }
    content[fsize] = '\0';
    return content;
}

int parse_double(jsmntok_t* token, const struct json_file* f, double *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_PRIMITIVE)
    {
        warnx("%s: '%.*s' token is not a double",
            f->filename, token->size, f->content + token->start);
        return -1;
    }

    size_t token_size = token->end - token->start + 1;
    char *t = alloca(sizeof(char) * token_size);
    memcpy(t, f->content + token->start, token_size);
    t[token_size - 1] = 0;
    *out = atof(t);
    return 0;
}

int parse_int(jsmntok_t* token, const struct json_file* f, int *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_PRIMITIVE)
    {
        warnx("%s: '%.*s' token is not an int",
            f->filename, token->size, f->content + token->start);
        return -1;
    }

    size_t token_size = token->end - token->start + 1;
    char *t = alloca(sizeof(char) * token_size);
    strncpy(t, f->content + token->start, token_size);
    *out = atoi(t);
    return 0;
}

int parse_vector(jsmntok_t* token, const struct json_file* f, vec3_t *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_ARRAY || token->size != 3)
    {
        warnx("%s:%d: syntax error, invalid float array",
             f->filename, token->start);
        return -1;
    }

    double *ptr = &out->x;
    for (size_t i = 0; i < 3; i++) {
        jsmntok_t* tok = token + i + 1;
        if (0 != parse_double(tok, f, ptr + i)) {
            return -1;
        }
    }
    return 0;
}
