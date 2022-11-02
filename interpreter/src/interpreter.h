#pragma once

#include "neillsdl2.h"

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <assert.h>
#include <math.h>

#define MAXLINES        1000
#define MAXCHARS        100
#define VALID_SCAN      1
#define REQ_ARGS        2
#define VAR_LEN         1
#define FIRST_CHAR      0
#define ERROR(PHRASE)   {fprintf(stderr, \
                        "Fatal Error: %s occured in %s, line %d\n", \
                        PHRASE, __FILE__, __LINE__); exit(1);}

#define UNDEFINED       -1
#define ZERO            0
#define ZEROF           0.0
#define RGB_MAX         255 
#define EPSILON         0.00001
#define MAXVARS         26
#define MAXLOOPS        50
#define ASCII_A         'A'
#define ASCII_Z         'Z'
#define MAXSTACKELEMS   100
#define RADCONVERTER    M_PI/180.0
#define ANGLE_MIN       0.00000000000
#define ANGLE_MAX       360.0000000000

/* Structs and Typedefs */
typedef enum bool{false, true} bool;
typedef enum operator {add, subtract, multiply, divide} operator;

struct program{
    char** tokens;
    int  curr_word;
    int  total_lines;
    SDL_Simplewin sw;
};
typedef struct program program;

struct loop{
    bool decrement;     /* Tell the interpreter to decrement in <DO> loop */
    int variable;       /* Variable assigned to loop */
    float index_end;      /* Upper bound of loop */
    float index_current;  /* Current loop index */
    int prog_start;     /* Starting token of loop in program */
};
typedef struct loop loop;

struct stack{
    float polish_stack[MAXSTACKELEMS];
    int size;
    int capacity;
};
typedef struct stack stack;

struct turtle{
    /* Current x, y and orientation of turtle */
    float curr_x; 
    float curr_y;
    float curr_angle;
    
    /* Number part of <VARNUM> */
    bool  constant_flag;    
    float constant_value;

    /* Variable part of <VARNUM> */
    bool variable_flag;
    int  variable_value;    
    /* Master array of values for variables A-Z */
    float variable_array[MAXVARS];
    
    /* Nested loops stack */
    loop loop_stack[MAXLOOPS];
    int curr_loop;

    /* Variable from <SET> */
    int set_variable;
    operator current_operator;
};
typedef struct turtle turtle;

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

/* Modified parser functions which include interpretation */
void parse_and_interpret_this(program* prog, turtle* turt);
void instrctlst(program* prog, turtle* turt);
void instruction(program* prog, turtle* turt);
bool fd(program* prog, turtle* turt);
bool lt(program* prog, turtle* turt);
bool rt(program* prog, turtle* turt);
bool do_loop(program* prog, turtle* turt);
void var(program* prog, turtle* turt);
void varnum(program* prog, turtle* turt);
bool set(program* prog, turtle* turt);
void polish(program* prog, turtle* turt, stack* s);
bool op(program* prog, turtle* turt);

/* Interpreter functions */
void init_turtle(turtle* t);
void fd_interpret(program* prog, turtle* turt);
void lt_interpret(turtle* turt);
void rt_interpret(turtle* turt);
void recalibrate_angle(turtle* turt);
bool update_constant(turtle* turt, float f);
bool update_variable(turtle* turt, int i);
bool update_coords(turtle* turt, float new_x, float new_y);
void move_turtle(program* prog, turtle* turt, float new_x, float new_y);
bool do_loop_start(turtle* turt);
bool do_loop_from(turtle* turt);
bool do_loop_to(turtle* turt);
void do_loop_pre(turtle* turt);
void do_loop_post(turtle* turt);
bool do_loop_finished(turtle* turt);
bool set_init(turtle* turt);
bool polish_init(stack* s);
bool polish_eval(turtle* turt, stack* s);
bool stack_push(turtle* turt, stack* s);
bool stack_pop(float* dst, stack* s);
bool stack_operate(turtle* turt, stack* s);
