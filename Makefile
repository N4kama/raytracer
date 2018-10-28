CC ?= clang
override CFLAGS += -g -Wall -Wextra -std=c99 -pedantic \
		 -D_POSIX_C_SOURCE=200809L \
		 -I./parser

LDFLAGS = -L.
LDLIBS = -lm -ldl
VPATH = src

RT_NAME = rt
RT_OBJS =			\
	main.o color.o img.o vector.o vector2.o mesh.o light.o raytrace.o	\

LIBPARSER=libparser.a


all: $(RT_NAME)

$(RT_NAME): $(RT_OBJS) $(LIBPARSER)
	$(CC) $(RT_OBJS) $(LIBPARSER) $(LDFLAGS) $(LDLIBS) $(LIBS) -o $@

parser/$(LIBPARSER):
	make -C parser

$(LIBPARSER): parser/$(LIBPARSER)
	mv parser/$(LIBPARSER) $@

proper:
	$(RM) $(RT_OBJS) $(LIBMATH_OBJS)

clean: proper
	$(RM) $(RT_NAME) $(LIBMATH) $(LIBPARSER)
	make -C parser clean

.PHONY: all $(RT_NAME) $(LIBPARSER) parser/$(LIBPARSER) proper clean

