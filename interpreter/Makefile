CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDLCFLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

interp : src/interpreter.h interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/neillsdl2.c $(COMMON) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -o interp

interp_s : src/interpreter.h interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)  -o interp_s

interp_no_SDL : src/interpreter.h interpreter_no_SDL.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  interpreter_no_SDL.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS)  -o interp_no_SDL

test_interp : src/interpreter.h test_interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/neillsdl2.h src/neillsdl2.c
	$(CC)  test_interpreter.c src/auxiliary_funcs.c src/parse_funcs.c src/interp_funcs_no_SDL.c src/neillsdl2.c $(SANITIZE) $(SDLCFLAGS) $(SDLLIBS) $(LDLIBS) -o test_interp

all : interp interp_s interp_no_SDL test_interp

run_test_interp : interp_no_SDL test_interp
		./test_interp

clean:
	rm -f interp interp_s interp_no_SDL test_interp

