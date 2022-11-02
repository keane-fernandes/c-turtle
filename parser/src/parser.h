#pragma once

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <assert.h>

/* Constants */
#define MAXLINES        1000
#define MAXCHARS        100
#define VALID_SCAN      1
#define REQ_ARGS        2
#define VAR_LEN         1
#define FIRST_CHAR      0
#define ERROR(PHRASE)   {fprintf(stderr, \
                        "Fatal Error: %s occured in %s, line %d\n", \
                        PHRASE, __FILE__, __LINE__); exit(1);}

/* Structs and Typedefs */
struct program{
    char** tokens;
    int  curr_word;
    int  total_lines;
};
typedef struct program program;

typedef enum bool{false, true} bool;

/* Auxiliary functions */
bool strsame(char* str1, char* str2);
void* nfopen(char* fname, char* mode);
void* ncalloc(int n, size_t size);
char** n2dcalloc(int h, int w, size_t szelem);
void n2dfree(char**p, int h);
void on_error(const char* s);
void init_program(program* prog);
void import_ttl(program* prog, char* filename, int n_args);
void free_program(program* prog);

/* Parser functions */
void parse_this(program* prog);
void instrctlst(program* prog);
void instruction(program* prog);
bool fd(program* prog);
bool lt(program* prog);
bool rt(program* prog);
bool do_loop(program* prog);
void var(program* prog);
void varnum(program* prog);
bool set(program* prog);
void polish(program* prog);
bool op(program* prog);
