#include "src/parser.h"

int main(int argc, char** argv)
{
    program* prog;

    prog = ncalloc(1, sizeof(program));

    init_program(prog);
    
    import_ttl(prog, argv[1], argc);

    parse_this(prog);
    
    if (prog->curr_word != prog->total_lines){
        ERROR("Tokens after end of file?")
    }

    free_program(prog);
    return 0;
}

