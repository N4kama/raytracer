#include <string.h>
#include "parser_json.h"

int pass_object(jsmntok_t* token)
{
    if (!token || token->type == JSMN_STRING || token->type == JSMN_PRIMITIVE)
        return 1;
    int res = 0;
    for (int i = 0; i < token->size; ++i)
    {
        res += pass_object(token + 1 + res);
        if (token->type == JSMN_OBJECT)
            res += pass_object(token + 1 + res);
    }
    return res + 1;
}

jsmntok_t* get_object_in_json(const char* data, jsmntok_t* token,
                              const struct json_file* f)
{
    if (!token || token->type != JSMN_OBJECT)
    {
        fprintf(stderr, "%s:%d: Unable to find %s in json\n", f->filename,
                token->start, data);
        return NULL;
    }
    int pass = 0;
    for (int i = 0; i < token->size; ++i)
    {
        jsmntok_t* name = token + 1 + pass;
        if (!strncmp(data, f->content + name->start, name->end - name->start))
            return name + 1;
        pass += pass_object(name + 1) + 1;
    }
    fprintf(stderr, "%s:%d: Unable to find %s in json\n", f->filename,
            token->start, data);
    return NULL;
}
