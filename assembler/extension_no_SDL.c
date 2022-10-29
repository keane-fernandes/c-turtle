/*
    This is an interpreter WITHOUT SDL output for testing purposes.  
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

    parse_and_interpret_this(prog, &t);
    
    if (prog->curr_word != prog->total_lines){
        ERROR("Tokens after end of file?")
    }

    free_program(prog);
    return 0;
}

