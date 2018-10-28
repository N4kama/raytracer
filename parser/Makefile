LIB = libparser.a
CFLAGS += -Wall -Wextra -Werror -pedantic -O3

OBJ_JSON =	\
	jsmn.o

OBJ_WAVEFRONT = 	\
	obj_parser.o	\
	list.o			\
	string_extra.o


OBJ_PARSER =			\
	parser.o			\
	parser_json.o		\
	parser_utils.o

$(LIB): $(LIB)($(OBJ_WAVEFRONT) $(OBJ_PARSER) $(OBJ_JSON))

proper:
	$(RM) $(OBJ_WAVEFRONT) $(OBJ_PARSER) $(OBJ_JSON)
clean: proper
	$(RM) $(LIB)
