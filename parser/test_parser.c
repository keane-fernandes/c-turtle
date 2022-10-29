/* 
    This is the testing harness for the parser.
    
    The harness looks scans through a directory specified by
    #define TSTDIR, and looks for files with the extension
    *.ttl. It ignores files with other extensions.
    
    The harness then automates the tunning of these files using
    the executable defined by #define EXEC.
    
    Upon completion, a results summary is presented on the
    terminal window.
    
    If a file has not been parsed correctly, the
    error message and the file name is printed on the
    terminal window.
*/

#include "src/parser.h"
#include <dirent.h>

/*#define TSTDIR      "ttl_files/Black_box"*/
#define TSTDIR      "ttl_files/Unit_tests"
#define EXEC        "parse_s"
#define VALID_SCAN  1
#define RED         "\033[31;1m"
#define BLUE        "\033[22;34m"
#define GREEN       "\033[0;32m" 
#define COLOR_RESET "\033[0m"
#define FILELEN     1500
#define EXT         "ttl"
#define EXT_LEN     4       /* Include one extra space for '.' */

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
                                EXEC, TSTDIR, de->d_name);
                err_flag = system(buffer);
                if (err_flag){
                    err_cnt++;
                    printf("Failed %s%s%s\n\n", RED, de->d_name, COLOR_RESET);
                    
                }
                file_cnt++;
            }
        }
        closedir(dr);
    }
    
    printf("\n\n%sTEST SUMMARY%s\n____________\n", BLUE, COLOR_RESET);
    printf("\nAll assertions passed.\n\n");
    printf("Total Unit Tests Executed: %d\nSuccessful: %s%d%s\nErrors: %s%d%s\n\n\n", 
                file_cnt, GREEN, file_cnt-err_cnt, COLOR_RESET, RED, err_cnt, COLOR_RESET);

    return 0;
}

/* Assert tests for parsing functions */
void assert_tests()
{   
    program* testprog = ncalloc(1, sizeof(program));

    assert(strsame("ABC", "ABC"));
    assert(!strsame("ABC", "aBC"));
    assert(strsame(" ", " "));
    assert(strsame("", ""));
    
    assert(check_ext("file.ttl"));
    assert(!check_ext("file.jpg"));
    assert(!check_ext("file.tttl"));
    assert(!check_ext("file.ttll"));
    assert(!check_ext("filettl"));

    init_program(testprog);

    strcpy(testprog->tokens[0], "{");
    strcpy(testprog->tokens[1], "FD");
    strcpy(testprog->tokens[2], "30");
    strcpy(testprog->tokens[3], "RT");
    strcpy(testprog->tokens[4], "30");
    strcpy(testprog->tokens[5], "LT");
    strcpy(testprog->tokens[6], "30");
    strcpy(testprog->tokens[7], "}");

    testprog->curr_word = 1;

    assert(fd(testprog));
    assert(rt(testprog));
    assert(lt(testprog));

    testprog->curr_word = 1;

    strcpy(testprog->tokens[2], "-30");
    strcpy(testprog->tokens[4], "A");
    strcpy(testprog->tokens[6], "Z");

    assert(fd(testprog));
    assert(rt(testprog));
    assert(lt(testprog));


    testprog->curr_word = 1;
    
    strcpy(testprog->tokens[1], "DO");
    strcpy(testprog->tokens[2], "A");
    strcpy(testprog->tokens[3], "FROM");
    strcpy(testprog->tokens[4], "5");
    strcpy(testprog->tokens[5], "TO");
    strcpy(testprog->tokens[6], "8");
    strcpy(testprog->tokens[7], "{");
    strcpy(testprog->tokens[8], "FD");
    strcpy(testprog->tokens[9], "30");
    strcpy(testprog->tokens[10], "}");
    
    assert(do_loop(testprog));

    testprog->curr_word = 1;

    strcpy(testprog->tokens[1], "SET");
    strcpy(testprog->tokens[2], "A");
    strcpy(testprog->tokens[3], ":=");
    strcpy(testprog->tokens[4], "5");
    strcpy(testprog->tokens[5], "4");
    strcpy(testprog->tokens[6], "+");
    strcpy(testprog->tokens[7], ";");

    assert(set(testprog));
    
    strcpy(testprog->tokens[1], "+");
    strcpy(testprog->tokens[2], "-");
    strcpy(testprog->tokens[3], "/");
    strcpy(testprog->tokens[4], "*");

    testprog->curr_word = 1;

    assert(op(testprog));
    assert(op(testprog));
    assert(op(testprog));
    assert(op(testprog));

    assert(testprog->curr_word == 5);


    free_program(testprog);
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
