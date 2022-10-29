CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

extension : src/interpreter.h extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/extension_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/extension_funcs.c src/neillsdl2.c $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -o extension

extension_s : src/interpreter.h extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/extension_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/extension_funcs.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)  -o extension_s

extension_no_SDL : src/interpreter.h extension_no_SDL.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/extension_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  extension_no_SDL.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/extension_funcs.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)  -o extension_no_SDL

test_extension : src/interpreter.h test_extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/extension_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  test_extension.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/extension_funcs.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -o test_extension

all : extension extension_s extension_no_SDL test_extension

run_test_extension : extension_no_SDL test_extension
		./test_extension

clean:
	rm -f extension extension_s extension_no_SDL test_extension

