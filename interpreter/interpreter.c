/*
    This an interpreter for the turtle grahics coursework. 
    Below are the rules for the user of the program:
    
    -   The maximum allowable size of the program is 
        1000 tokens after which the program will throw an
        error. This can be changed by altering the 
        #define MAXLINES in the turtle.h file.
    -   Any attempt to draw outside of the SDL window 
        will be flagged as an error, and cause the program
        to quit. By default, the size of the window is 
        900 x 900. This can be changed using the #defines 
        WWIDTH and WHEIGHT in neillsdl2.h.
    -   All uninitialised variables by default have a value
        of 0. The user must use the <SET> function to assign
        the variable with the desired value. This behaviour
        is similar to C where sometimes unset variables have
        values of 0.  
*/

#include "src/interpreter.h"
#include "src/neillsdl2.h"

int main(int argc, char** argv)
{
    program* prog;
    turtle t;

    prog = ncalloc(1, sizeof(program));

    init_program(prog);

    init_turtle(&t);
    
    import_ttl(prog, argv[1], argc);

    /* Initialise an SDL Window */
    Neill_SDL_Init(&(prog->sw));
    
    parse_and_interpret_this(prog, &t);
    
    if (prog->curr_word != prog->total_lines){
        ERROR("Tokens after end of file?")
    }

    do{
        SDL_Delay(MILLISECONDDELAY);
        Neill_SDL_Events(&(prog->sw));
    } while(!(prog->sw.finished));

    SDL_Quit();
    atexit(SDL_Quit);

    free_program(prog);
    return 0;
}

