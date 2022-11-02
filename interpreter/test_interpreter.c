/* 
    This is the testing file for the interpreter. Unmodified and reused parser
    functions were not tested. The testing is made up of two parts:

    1. Assert testing    - Functions with a boolean return value are assert tested.
    2. Black box testing - Various *.ttl files were created and are batch tested. 
                           Upon successful completion, the results are printed on 
                           the terminal screen.
                           If a file has not been parsed correctly, the 
                           error message and the file name is printed on the
                           terminal window.


*/

#include "src/interpreter.h"
#include <dirent.h>

#define TSTDIR      "ttl_files/Black_box"     /* Directory of *.ttl files */
#define EXECUTABLE  "interp_no_SDL"   /* Testing executable */
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

/******************************************/
/* Assert tests for interpreter functions */
/******************************************/
void assert_tests()
{   
    turtle t;
    program* p;
    stack s;
    float f1 , f2; 

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
