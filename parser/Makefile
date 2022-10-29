CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

parse : src/parser.h src/parser_funcs.c src/auxiliary_funcs.c parser.c
	$(CC)  parser.c src/auxiliary_funcs.c src/parser_funcs.c $(PRODUCTION) $(LDLIBS) -o parse

parse_s : src/parser.h src/parser_funcs.c src/auxiliary_funcs.c parser.c
	$(CC)  parser.c src/auxiliary_funcs.c src/parser_funcs.c $(SANITIZE) $(LDLIBS) -o parse_s

test_parse : src/parser.h src/parser_funcs.c test_parser.c
	$(CC)  test_parser.c src/auxiliary_funcs.c src/parser_funcs.c $(SANITIZE) $(LDLIBS) -o test_parse

run_test_parse : parse_s test_parse
				./test_parse

all: parse parse_s test_parse

clean :
	rm -f parse parse_s test_parse
