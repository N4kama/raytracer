#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "libparser.h"
#include "libparser.h"
#include "obj_parser.h"
#include "parser_json.h"

struct scene* parse_json(const char* filename);
void destroy_scene(struct scene* scene);

/* typedef used for get_array function */
typedef int (*parse_item_pfn)(jsmntok_t*, const struct json_file*, void *);

parser_t scene_parser = {
    .load = parse_json,
    .free = destroy_scene,
};

static char* load_json(
    const char *filename,
    jsmntok_t *toks,
    size_t count,
    size_t *token_count)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        warnx("Cannot open file '%s'", filename);
        return NULL;
    }

    char* json = get_all_content(file, filename);
    fclose(file);

    jsmn_parser parser;
    jsmn_init(&parser);

    int r = jsmn_parse(&parser, json, strlen(json), toks, count);
    if (0 > r) {
        free(json);
        warnx("%s could not be loaded.", filename);
        return NULL;
    }

    *token_count = r;
    return json;
}

static int get_camera(
    jsmntok_t* token,
    const struct json_file* f,
    struct camera *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_OBJECT) {
        warnx("%s:%d: invalid 'camera' field\n",
                f->filename, token->start);
        return -1;
    }

    jsmntok_t* pos = get_object_in_json("position", token, f);
    jsmntok_t* u = get_object_in_json("forward", token, f);
    jsmntok_t* v = get_object_in_json("up", token, f);
    jsmntok_t* fov = get_object_in_json("fov", token, f);

    if (0 != parse_vector(pos, f, &out->position))
        return -1;
    if (0 != parse_vector(u, f, &out->forward))
        return -1;
    if (0 != parse_vector(v, f, &out->up))
        return -1;
    if (0 != parse_double(fov, f, &out->fov))
        return -1;

    return 0;
}

static int parse_light_type(
    jsmntok_t* token,
    const struct json_file* f,
    enum type_light *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_STRING) {
        warnx("%s: invalid field light-type\n", f->filename);
        return -1;
    }

    const struct {
        const char* key;
        enum type_light value;
    } pairs[] = {
        { .key = "ambient", .value = AMBIENT },
        { .key = "directional", .value = DIRECTIONAL },
        { .key = "point", .value = POINT },
    };

    for (size_t i = 0; i < sizeof(pairs)/sizeof(pairs[0]); i++) {
        if (0 != strncmp(f->content + token->start,
                         pairs[i].key,
                         token->end - token->start)) {
            continue;
        }

        *out = pairs[i].value;
        return 0;
    }

    warnx("%s: invalid field light-type\n", f->filename);
    return -1;
}

static int parse_light(
    jsmntok_t* token,
    const struct json_file* f,
    struct light *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    jsmntok_t* tk_type = get_object_in_json("type", token, f);
    jsmntok_t* tk_color = get_object_in_json("color", token, f);

    if (0 != parse_light_type(tk_type, f, &out->type))
        return -1;
    if (0 != parse_vector(tk_color, f, &out->color))
        return -1;

    if (out->type == AMBIENT)
        return 0;

    jsmntok_t* tk_vector = get_object_in_json("vector", token, f);
    if (0 != parse_vector(tk_vector, f, &out->vector))
        return -1;
    return 0;
}

static int parse_json_material(
    const char *filename,
    struct material *out)
{
    jsmntok_t tokens[256];
    size_t token_count;
    char *json = load_json(filename, tokens, 256, &token_count);

    struct json_file f = { filename, json };
    jsmntok_t *t;

    do {
#define PARSE_TOKEN(Type, Variable, Tokens, File, TokenName, Storage)    \
    t = get_object_in_json(TokenName, Tokens, File);                     \
    if (0 != parse_ ## Type(Variable, File, &(Storage)))                 \
        break;

        PARSE_TOKEN(vector, t, tokens, &f, "diffuse", out->diffuse);
        PARSE_TOKEN(vector, t, tokens, &f, "specular", out->specular);
        PARSE_TOKEN(double, t, tokens, &f, "spec-level", out->specular_level);
        PARSE_TOKEN(double, t, tokens, &f, "shininess", out->shininess);
        PARSE_TOKEN(double, t, tokens, &f, "reflectivity", out->reflectivity);
        PARSE_TOKEN(double, t, tokens, &f, "opacity", out->opacity);
        PARSE_TOKEN(double, t, tokens, &f, "refr-index", out->refr_index);
        PARSE_TOKEN(vector, t, tokens, &f, "fog-color", out->fog_color);
        PARSE_TOKEN(double, t, tokens, &f, "fog-density", out->fog_density);
#undef PARSE_TOKEN

#define PARSE_TEXT(Variable, Tokens, File, TokenName, Storage)          \
    Variable = get_object_in_json(TokenName, Tokens, File);             \
    if ((Variable)->type != JSMN_STRING)                                \
        break;                                                          \
    Storage = strndup((File)->content + (Variable)->start,              \
                      (Variable)->end - (Variable)->start)

        PARSE_TEXT(t, tokens, &f, "tex-diffuse", out->tex_diffuse);
        PARSE_TEXT(t, tokens, &f, "tex-normal", out->tex_normal);
        PARSE_TEXT(t, tokens, &f, "tex-opacity", out->tex_opacity);
#undef PARSE_TEXT

        free(json);
        return 0;
    } while (0);

    free(json);
    return -1;
}

static int parse_material(
    jsmntok_t* token,
    const struct json_file* f,
    struct material *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_STRING) {
        warnx("%s: invalid material entry\n", f->filename);
        return -1;
    }

    const size_t token_len = token->end - token->start + 1;
    char *filename = alloca(token_len);
    memcpy(filename, f->content + token->start, token_len);
    filename[token_len - 1] = 0;

    if (0 != parse_json_material(filename, out)) {
        return -1;
    }


    return 0;
}

static int parse_mesh(
    const jsmntok_t* token,
    const struct json_file* f,
    struct mesh *out)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_STRING) {
        warnx("%s: invalid mesh entry\n", f->filename);
        return -1;
    }

    const size_t token_len = token->end - token->start + 1;
    char *filename = alloca(token_len);
    memcpy(filename, f->content + token->start, token_len);
    filename[token_len - 1] = 0;

    obj_scene_data raw;
    if (1 != parse_obj_scene(&raw, filename)) {
        return -1;
    }

    out->vtx = calloc(sizeof(*out->vtx), raw.face_count * 3);
    out->nrm = calloc(sizeof(*out->nrm), raw.face_count * 3);
    out->uv = calloc(sizeof(*out->uv), raw.face_count * 3);
    if (NULL == out->vtx || NULL == out->nrm || NULL == out->uv) {
        free(out->vtx);
        free(out->nrm);
        free(out->uv);
        delete_obj_data(&raw);
        return -1;
    }

    size_t uv_count = 0;
    size_t nrm_count = 0;

    for (int i = 0; i < raw.face_count; i++) {
        obj_face *f = raw.face_list[i];

        for (size_t j = 0; j < 3; j++) {
            int vt_id = f->vertex_index[j];
            int nrm_id = f->normal_index[j];
            int uv_id = f->texture_index[j];
            assert(vt_id >= 0);

            out->vtx[i * 3 + j] = *raw.vertex_list[vt_id];
            if (uv_id >= 0) {
                out->uv[i * 3 + j] = *raw.vertex_texture_list[uv_id];
                uv_count += 1;
            }
            if (nrm_id >= 0) {
                out->nrm[i * 3 + j] = *raw.vertex_normal_list[nrm_id];
                nrm_count += 1;
            }
        }
    }

    if (nrm_count == 0) {
        free(out->nrm);
        out->nrm = NULL;
    }

    if (uv_count == 0) {
        free(out->uv);
        out->uv = NULL;
    }

    out->tri_count = raw.face_count;
    delete_obj_data(&raw);

    return 0;
}

static int parse_object(
    jsmntok_t* tokens,
    const struct json_file* f,
    struct object *out)
{
    assert(tokens != NULL && "tokens must be a valid pointer");
    assert(f != NULL && "f must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    jsmntok_t* tk;

    do {
#define PARSE_TOKEN(Type, Variable, Tokens, File, TokenName, Storage)    \
    Variable = get_object_in_json(TokenName, Tokens, File);              \
    if (0 != parse_ ## Type(Variable, File, &(Storage)))                 \
        break;

        PARSE_TOKEN(vector, tk, tokens, f, "position", out->position);
        PARSE_TOKEN(vector, tk, tokens, f, "rotation", out->rotation);
        PARSE_TOKEN(vector, tk, tokens, f, "scale", out->scale);

        int tmp;
        PARSE_TOKEN(int, tk, tokens, f, "mesh", tmp);
        out->mesh_id = tmp;
        PARSE_TOKEN(int, tk, tokens, f, "mtl", tmp);
        out->mtl_id = tmp;

        return 0;
    } while(0);

    return -1;
}

static int get_array(
    jsmntok_t* token,
    const struct json_file* f,
    void **out,
    size_t *count,
    size_t elt_size,
    parse_item_pfn parse_item)
{
    assert(token != NULL && "token must be a valid pointer");
    assert(out != NULL && "out must be a valid pointer");

    if (token->type != JSMN_ARRAY) {
        warnx("%s: invalid array field\n", f->filename);
        return -1;
    }

    if (token->size == 0) {
        *count = 0;
        *out = NULL;
        return 0;
    }

    char *elts = calloc(elt_size, token->size);
    if (NULL == elts)
        return -1;

    jsmntok_t* tk_elt = token + 1;
    for (int i = 0; i < token->size; i++)
    {
        if (0 != parse_item(tk_elt, f, elts + (elt_size * i))) {
            free(elts);
            return -1;
        }
        tk_elt += pass_object(tk_elt);
    }

    *out = elts;
    *count = token->size;
    return 0;
}

static struct scene* get_scene(
    jsmntok_t* tokens,
    const struct json_file* f)
{
    if (tokens[0].type != JSMN_OBJECT) {
        warnx("%s: invalid root object", f->filename);
        return NULL;
    }

    struct scene* scene = calloc(sizeof(struct scene), 1);
    if (NULL == scene) {
        return NULL;
    }
    jsmntok_t* height = get_object_in_json("height", &tokens[0], f);
    jsmntok_t* width = get_object_in_json("width", &tokens[0], f);
    jsmntok_t* json_camera = get_object_in_json("camera", &tokens[0], f);
    jsmntok_t* json_meshs = get_object_in_json("meshs", &tokens[0], f);
    jsmntok_t* json_mtls = get_object_in_json("materials", &tokens[0], f);
    jsmntok_t* json_lights = get_object_in_json("lights", &tokens[0], f);
    jsmntok_t* json_objects = get_object_in_json("objects", &tokens[0], f);

    if (!height || !width || !json_camera || !json_meshs || !json_lights ||
        !json_mtls)
        return NULL;

    do {
        if (0 != parse_int(height, f, &scene->height))
            break;
        if (0 != parse_int(width, f, &scene->width))
            break;
        if (0 != get_camera(json_camera, f, &scene->camera))
            break;

        int r;
        r = get_array(
                json_lights,
                f,
                (void*)&scene->lights,
                &scene->light_count,
                sizeof(struct light),
                (parse_item_pfn)parse_light
            );

        if (r != 0)
            break;

        r = get_array(
                json_mtls,
                f,
                (void*)&scene->mtls,
                &scene->mtl_count,
                sizeof(struct material),
                (parse_item_pfn)parse_material
            );

        if (r != 0)
            break;

        r = get_array(
                json_meshs,
                f,
                (void*)&scene->meshs,
                &scene->mesh_count,
                sizeof(struct mesh),
                (parse_item_pfn)parse_mesh
            );
        if (r != 0)
            break;

        r = get_array(
                json_objects,
                f,
                (void*)&scene->objects,
                &scene->object_count,
                sizeof(struct object),
                (parse_item_pfn)parse_object
            );

        return scene;
    } while (0);

    free(scene);
    return NULL;
}

static void destroy_meshs(struct mesh *meshs, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        free(meshs[i].vtx);
        free(meshs[i].nrm);
        free(meshs[i].uv);
    }
    free(meshs);
}

static void destroy_materials(struct material *mtls, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        free(mtls[i].tex_diffuse);
        free(mtls[i].tex_normal);
        free(mtls[i].tex_opacity);
    }
    free(mtls);
}

void destroy_scene(struct scene* scene)
{
    if (!scene)
        return;

    destroy_meshs(scene->meshs, scene->mesh_count);
    destroy_materials(scene->mtls, scene->mtl_count);
    free(scene->objects);
    free(scene->lights);
    free(scene);
}

struct scene* parse_json(const char* filename)
{
    jsmntok_t tokens[256];
    size_t token_count;

    char *json = load_json(filename, tokens, 256, &token_count);
    if (NULL == json) {
        warnx("unable to load the file %s", filename);
        return NULL;
    }

    struct json_file f = { filename, json };
    struct scene* scene = get_scene(tokens, &f);

    free(json);
    return scene;
}
