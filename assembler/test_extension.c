/* 
    This is the testing file for the extension.
*/

#include "src/interpreter.h"
#include <dirent.h>

#define TSTDIR      "ttl_files/Black_box"     /* Directory of *.ttl files */
#define EXECUTABLE  "extension_no_SDL"   /* Testing executable */
#define VALID_SCAN  1
#define RED         "\033[31;1m"
#define BLUE        "\033[22;34m"
#define GREEN       "\033[0;32m" 
#define COLOR_RESET "\033[0m"
#define FILELEN     1500
#define EXT         "ttl"

void assert_tests();
bool check_ext(char* filename);

int main(void)
{
    int file_cnt, err_flag, err_cnt;
    char buffer[FILELEN];
    struct dirent *de;
    
    /* Directory where *.ttl files are */
    DIR* dr = opendir(TSTDIR);

    /* Performs assert testing on parser functions */
    assert_tests();

    /* 
        Initialisation of black box testing
        with all *.ttl files found in directory
    */
    file_cnt = 0;
    err_cnt = 0;

    /* Checks for valid directory */
    if (dr) {
        /* Checks that file being read is valid */
        while ( (de = readdir(dr))!= NULL ) {
            /* Checks if regular file and *.ttl file */
            if ( (de->d_type == DT_REG) && (check_ext(de->d_name)) ){
                /* Print system command to buffer */
                snprintf(buffer, FILELEN, "./%s %s/%s", 
                                EXECUTABLE, TSTDIR, de->d_name);
                err_flag = system(buffer);
                if (err_flag){
                    err_cnt++;
                    printf("Interpretation of %s%s%s unsuccessful\n\n", 
                                RED, de->d_name, COLOR_RESET);
                    
                }
                file_cnt++;
            }
        }
        closedir(dr);
    }
    
    printf("\n\n%sTEST SUMMARY%s\n____________\n", BLUE, COLOR_RESET);
    printf("\nAll assertions passed.\n\n");
    printf("Total Turtle Files Interpreted: %d\nSuccessful: %s%d%s\nErrors: %s%d%s\n\n\n", 
                file_cnt, GREEN, file_cnt-err_cnt, COLOR_RESET, RED, err_cnt, COLOR_RESET);
    return 0;
}


void assert_tests()
{   
    turtle t;
    program* p;
    stack s;
    float f1 , f2; 

    /******************************************/
    /*          Interpreter testing           */
    /******************************************/


    p = ncalloc(1, sizeof(program));
    
    init_program(p);
    init_turtle(&t);

    /* Tests for update_constant */
    f1 = 23.11f;
    update_constant(&t, f1);
    assert(t.constant_flag);
    assert(!t.variable_flag);
    assert((fabs(f1 - t.constant_value) < EPSILON));

    f1 = -23.11f;
    update_constant(&t, f1);
    assert(t.constant_flag);
    assert(!t.variable_flag);
    assert((fabs(f1 - t.constant_value) < EPSILON));

    /* Tests for update_variable */
    update_variable(&t, 'A');
    assert(!t.constant_flag);
    assert(t.variable_flag);
    assert(t.variable_value == 0);

    update_variable(&t, 'M');
    assert(!t.constant_flag);
    assert(t.variable_flag);
    assert(t.variable_value == 12);

    update_variable(&t, 'Z');
    assert(!t.constant_flag);
    assert(t.variable_flag);
    assert(t.variable_value == 25);

    /* Tests for update_coords */
    f1 = 23.01242;
    f2 = 49.68921;
    update_coords(&t, f1, f2);
    assert(fabs(t.curr_x - f1) < EPSILON);
    assert(fabs(t.curr_y - f2) < EPSILON);

    strcpy(p->tokens[0], "DO");
    strcpy(p->tokens[1], "B");
    strcpy(p->tokens[2], "FROM");
    strcpy(p->tokens[3], "1");
    strcpy(p->tokens[4], "TO");
    strcpy(p->tokens[5], "8");

    /* Tests for recalibrate_angle */

    t.curr_angle = 361.0f;
    recalibrate_angle(&t);
    assert(fabs(t.curr_angle - 1.0f) < EPSILON);
    
    t.curr_angle = 721.0f;
    recalibrate_angle(&t);
    assert(fabs(t.curr_angle - 1.0f) < EPSILON);    
    
    t.curr_angle = -361.0f;
    recalibrate_angle(&t);
    assert(fabs(t.curr_angle - (-1.0f)) < EPSILON);

    t.curr_angle = -721.0f;
    recalibrate_angle(&t);
    assert(fabs(t.curr_angle - (-1.0f)) < EPSILON);
    
    /* Tests for <DO> part of the grammar */
    
    /* Tests for do_loop_start */
    
    p->curr_word = 1;
    t.curr_loop = 0;
    
    var(p, &t);
    assert(do_loop_start(&t));
    assert(t.loop_stack[0].variable == 1);

    p->curr_word = 1;
    strcpy(p->tokens[1], "A");
    var(p, &t);

    assert(do_loop_start(&t));
    assert(t.loop_stack[0].variable == 0);

    p->curr_word = 1;
    strcpy(p->tokens[1], "Z");
    var(p, &t);

    assert(do_loop_start(&t));
    assert(t.loop_stack[0].variable == 25);

    /* Tests for do_loop_from */

    p->curr_word = 3;
    varnum(p, &t);
    assert(t.constant_flag);
    assert(fabs(t.constant_value - 1.0f) < EPSILON);
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 1.0f) < EPSILON);
    
    p->curr_word = 3;
    strcpy(p->tokens[3], "C");
    t.variable_array['C' - ASCII_A] = 5.0f;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('C'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 5.0f) < EPSILON);

    p->curr_word = 3;
    strcpy(p->tokens[3], "Z");
    t.variable_array['Z' - ASCII_A] = 7.0f;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('Z'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 7.0f) < EPSILON);


    p->curr_word = 3;
    strcpy(p->tokens[3], "A");
    t.variable_array['A' - ASCII_A] = 9.0f;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('A'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 9.0f) < EPSILON);

    /* Tests for do_loop_to */

    p->curr_word = 5;
    varnum(p, &t);
    assert(t.constant_flag);
    assert(fabs(t.constant_value - 8.0f) < EPSILON);
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 8.0f) < EPSILON);    
    
    p->curr_word = 5;
    strcpy(p->tokens[5], "D");
    t.variable_array['D' - ASCII_A] = 10;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('D'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 10.0f) < EPSILON);

    p->curr_word = 5;
    strcpy(p->tokens[5], "D");
    t.variable_array['D' - ASCII_A] = 10;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('D'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 10.0f) < EPSILON);

    p->curr_word = 5;
    strcpy(p->tokens[5], "Z");
    t.variable_array['Z' - ASCII_A] = 20.0f;
    varnum(p, &t);
    assert(t.variable_flag);
    assert(t.variable_value == ('Z'-ASCII_A));
    assert(do_loop_from(&t));
    assert(fabs(t.loop_stack[0].index_current - 20.0f) < EPSILON);

    /* Tests for <SET> part of the grammar */

    /* Tests for set_init */
    strcpy(p->tokens[0], "SET");
    strcpy(p->tokens[1], "A");
    strcpy(p->tokens[2], ":=");
    strcpy(p->tokens[3], "5");
    strcpy(p->tokens[4], "10");
    strcpy(p->tokens[5], "+");
    strcpy(p->tokens[6], "5");
    strcpy(p->tokens[7], "-");
    strcpy(p->tokens[8], "5");
    strcpy(p->tokens[9], "*");
    strcpy(p->tokens[10], "5");
    strcpy(p->tokens[11], "/");
    strcpy(p->tokens[12], ";");

    p->curr_word = 1;
    varnum(p, &t);
    assert(set_init(&t));

    p->curr_word = 1;
    strcpy(p->tokens[1], "M");
    assert(set_init(&t));

    p->curr_word = 1;
    strcpy(p->tokens[1], "Z");
    assert(set_init(&t));

    /* Tests for polish_init */
    assert(polish_init(&s));
    assert(s.size == 0);
    assert(s.capacity == MAXSTACKELEMS);

    /* Tests for stack_push, stack_pop, stack_operate */
    
    p->curr_word = 3;
    varnum(p, &t);
    assert(t.constant_flag);
    assert(fabs(t.constant_value - 5.0f) < EPSILON);
    assert(stack_push(&t, &s));
    assert(s.capacity == MAXSTACKELEMS);
    assert(s.size == 1);
    assert(fabs(s.polish_stack[s.size-1] - 5.0f) < EPSILON);

    varnum(p, &t);
    assert(t.constant_flag);
    assert(fabs(t.constant_value - 10.0f) < EPSILON);
    assert(stack_push(&t, &s));
    assert(s.capacity == MAXSTACKELEMS);
    assert(s.size == 2);
    assert(fabs(s.polish_stack[s.size-1] - 10.0f) < EPSILON);    

    p->curr_word = 5;
    assert(op(p, &t));
    assert(t.current_operator == add);
    assert(stack_operate(&t, &s));
    assert(s.size == 1);
    assert(s.polish_stack[s.size -1] - 15.0f < EPSILON);
    
    varnum(p, &t);
    assert(stack_push(&t, &s));
    assert(op(p, &t));
    assert(t.current_operator == subtract);
    assert(s.size == 2);
    assert(stack_operate(&t, &s));
    assert(s.size == 1);
    assert(s.polish_stack[s.size -1] - 10.0f < EPSILON);

    assert(p->curr_word == 8);
    
    varnum(p, &t);
    assert(stack_push(&t, &s));
    assert(op(p, &t));
    assert(t.current_operator == multiply);
    assert(s.size == 2);
    assert(stack_operate(&t, &s));
    assert(s.size == 1);
    assert(s.polish_stack[s.size -1] - 50.0f < EPSILON);    

    assert(p->curr_word == 10);
    varnum(p, &t);
    assert(stack_push(&t, &s));
    assert(op(p, &t));
    assert(t.current_operator == divide);
    assert(s.size == 2);
    assert(stack_operate(&t, &s));
    assert(s.size == 1);
    assert(s.polish_stack[s.size -1] - 10.0f < EPSILON);
    
    assert(polish_eval(&t, &s));
    assert(fabs(t.variable_array[t.set_variable] - 10.0f) < EPSILON);

    strcpy(p->tokens[0], "SET");
    strcpy(p->tokens[1], "Z");
    strcpy(p->tokens[2], ":=");
    strcpy(p->tokens[3], "A");
    strcpy(p->tokens[4], "B");
    strcpy(p->tokens[5], "+");
    strcpy(p->tokens[6], "C");
    strcpy(p->tokens[7], "-");
    strcpy(p->tokens[8], "D");
    strcpy(p->tokens[9], "*");
    strcpy(p->tokens[10], "E");
    strcpy(p->tokens[11], "/");
    strcpy(p->tokens[12], ";");
    
    t.variable_array['A'-ASCII_A] = 5.0f;
    t.variable_array['B'-ASCII_A] = 10.0f;
    t.variable_array['C'-ASCII_A] = 5.0f;
    t.variable_array['D'-ASCII_A] = 5.0f;
    t.variable_array['E'-ASCII_A] = 5.0f;

    p->curr_word = 0;
    assert(set(p, &t));
    assert(fabs(t.variable_array['Z'-ASCII_A] - 10.0f) < EPSILON);

    /*====================================================*/
    /*       TESTING FOR TURTLE ASSEMBLER (Extension)     */   
    /*====================================================*/

    /* Testing for moveto */
    strcpy(p->tokens[0], "MOVETO");
    strcpy(p->tokens[1], "123");
    strcpy(p->tokens[2], "456");
    
    p->curr_word = 0;
    t.curr_x = 123;
    t.curr_y = 456;

    f1 = t.curr_x;
    f2 = t.curr_y;

    assert(moveto(p, &t));
    assert(fabs(t.curr_x - WWIDTH/2 - f1) < EPSILON);
    assert(fabs(t.curr_y - WHEIGHT/2 - f2) < EPSILON);

    /* Testing for pencol_r, pencol_g, pencol_b */
    strcpy(p->tokens[0], "PENCOL_R");
    strcpy(p->tokens[1], ":=");
    strcpy(p->tokens[2], "0");

    p->curr_word = 0;
    assert(pencol_r(p, &t));

    strcpy(p->tokens[0], "PENCOL_G");
    strcpy(p->tokens[1], ":=");
    strcpy(p->tokens[2], "100");
    p->curr_word = 0;
    assert(pencol_g(p, &t));

    strcpy(p->tokens[0], "PENCOL_B");
    strcpy(p->tokens[1], ":=");
    strcpy(p->tokens[2], "255");
    p->curr_word = 0;
    assert(pencol_b(p, &t));    

    /* Testing for ldr */

    strcpy(p->tokens[0], "LDR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r2");

    p->curr_word = 0;

    t.rgstrs[r2] = 7;
    
    assert(ldr(p, &t));
    assert(t.dst_reg == r0);
    assert(t.op1_reg == r2);
    assert(t.rgstrs[r0] == 7);

    /* Testing for mvr */
    strcpy(p->tokens[0], "MVR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "A");

    t.variable_array[0] = 7;

    p->curr_word = 0;
    assert(mvr(p, &t));
    assert(t.rgstrs[r0] == 7);

    strcpy(p->tokens[0], "MVR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "255");

    p->curr_word = 0;
    assert(mvr(p, &t));
    assert(t.rgstrs[r0] == 255);

    /* Testing for bw_and */    
    strcpy(p->tokens[0], "AND");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");
    strcpy(p->tokens[3], "r2");

    t.rgstrs[r0] = 6;
    t.rgstrs[r1] = 255;
    t.rgstrs[r2] = 0;
    
    p->curr_word = 0;
    assert(bw_and(p, &t));
    assert(t.rgstrs[r0] == 0);

    t.rgstrs[r0] = 6;
    t.rgstrs[r1] = 255;
    t.rgstrs[r2] = 255;

    p->curr_word = 0;
    assert(bw_and(p, &t));
    assert(t.rgstrs[r0] == 255);

    /* Testing for bw_or */
    strcpy(p->tokens[0], "ORR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");
    strcpy(p->tokens[3], "r2");

    t.rgstrs[r0] = 6;
    t.rgstrs[r1] = 255;
    t.rgstrs[r2] = 255;

    p->curr_word = 0;
    assert(bw_orr(p, &t));
    assert(t.rgstrs[r0] == 255);

    t.rgstrs[r0] = 6;
    t.rgstrs[r1] = 0;
    t.rgstrs[r2] = 255;

    p->curr_word = 0;
    assert(bw_orr(p, &t));
    assert(t.rgstrs[r0] == 255);

    t.rgstrs[r0] = 0;
    t.rgstrs[r1] = 0;
    t.rgstrs[r2] = 255;

    p->curr_word = 0;
    assert(bw_orr(p, &t));
    assert(t.rgstrs[r0] == 255);

    /* Testing for bw_xor */
    strcpy(p->tokens[0], "XOR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");
    strcpy(p->tokens[3], "r1");    

    t.rgstrs[r0] = 6;
    t.rgstrs[r1] = 255;

    p->curr_word = 0;
    assert(bw_xor(p, &t));
    assert(t.rgstrs[r0] == 0);

    /* Testing for bw_not */
    strcpy(p->tokens[0], "NOT");
    strcpy(p->tokens[1], "r0");

    t.rgstrs[r0] = 4;

    p->curr_word = 0;
    assert(bw_not(p, &t));
    assert(t.rgstrs[r0] == 251);
    
    /* Testing for bw_lsl */
    strcpy(p->tokens[0], "LSL");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 1;
    t.rgstrs[r1] = 4;
    p->curr_word = 0;
    
    assert(bw_lsl(p, &t));
    assert(t.rgstrs[r0] == 16);
    
    strcpy(p->tokens[0], "LSL");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 2;
    t.rgstrs[r1] = 5;
    p->curr_word = 0;
    
    assert(bw_lsl(p, &t));
    assert(t.rgstrs[r0] == 64);
    
    strcpy(p->tokens[0], "LSL");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 128;
    t.rgstrs[r1] = 1;
    p->curr_word = 0;

    assert(bw_lsl(p, &t));
    assert(t.rgstrs[r0] == 0);
    
    /* Testing for bw_lsr */
    strcpy(p->tokens[0], "LSR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 128;
    t.rgstrs[r1] = 4;
    p->curr_word = 0;
    
    assert(bw_lsr(p, &t));
    assert(t.rgstrs[r0] == 8);
    
    strcpy(p->tokens[0], "LSR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 64;
    t.rgstrs[r1] = 5;
    p->curr_word = 0;

    assert(bw_lsr(p, &t));
    assert(t.rgstrs[r0] == 2);
    
    strcpy(p->tokens[0], "LSR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "r1");

    t.rgstrs[r0] = 1;
    t.rgstrs[r1] = 1;
    p->curr_word = 0;

    assert(bw_lsr(p, &t));
    assert(t.rgstrs[r0] == 0);

    /* Testing for str */
    strcpy(p->tokens[0], "STR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "A");

    t.variable_array[0] = 7;
    t.rgstrs[r0] = 15;
    p->curr_word = 0;
    
    assert(str(p, &t));
    assert(fabs(t.variable_array[0]-15) < EPSILON);

    strcpy(p->tokens[0], "STR");
    strcpy(p->tokens[1], "r0");
    strcpy(p->tokens[2], "Z");

    t.variable_array[25] = 7;
    t.rgstrs[r0] = 255;
    p->curr_word = 0;
    
    assert(str(p, &t));
    
    assert(fabs(t.variable_array[25]-255) < EPSILON);
    
    free_program(p);

}

/* Checks that a filename has specified extension */
bool check_ext(char* filename)
{
    char* ptr;

    /* Finds pointer to last '.' character in filename */
    ptr = strrchr(filename, '.');

    if (!ptr){
        return false;
    }

    if (strsame(++ptr, EXT)){
        return true;
    }
    else {
        return false;
    }
}
