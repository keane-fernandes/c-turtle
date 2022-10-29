#include "parser.h"

/* 
    Checks that the program starts with a "{"
    and moves on to the <INSTRCTLIST> part 
*/
void parse_this(program* prog)
{
    if (!strsame((prog->tokens[prog->curr_word]), "{")){
        ERROR("Missing an opening curly brace '{' ?")
    }
    prog->curr_word++;
    instrctlst(prog);
}

/* Checks for a "{" | <INSTRUCTION><INSTRCTLST> */
void instrctlst(program* prog)
{
    if (strsame((prog->tokens[prog->curr_word]), "}")){
        prog->curr_word++;
        return;
    }
    instruction(prog);
    instrctlst(prog);
}

/* 
    Checks for the valid functions defined in <INSTRUCTION>
    and returns an error if none of the defined functions
    return true 
*/
void instruction(program* prog)
{
    if (fd(prog)) {
        return;
    }
    else if (rt(prog)){
        return;
    }
    else if (lt(prog)){
        return;
    }
    else if (do_loop(prog)){
        return;
    }
    else if (set(prog)){
        return;
    }
    else {
        ERROR("Invalid instruction, was expecting FD, RT, LT, DO or SET?")
    }   
}

/* 
    Checks if FD command follows the right format
    and returns true/false 
*/
bool fd(program* prog)
{
    if ( strsame((prog->tokens[prog->curr_word]), "FD") ){
        prog->curr_word++;
        varnum(prog);
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Checks if LT command follows the right format
    and returns true/false 
*/
bool lt(program* prog)
{
    if ( strsame((prog->tokens[prog->curr_word]), "LT") ){
        prog->curr_word++;
        varnum(prog);
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Checks if RT command follows the right format
    and returns true/false 
*/
bool rt(program* prog)
{
    if ( strsame((prog->tokens[prog->curr_word]), "RT") ){
        prog->curr_word++;
        varnum(prog);
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Checks the <DO> part of the grammar
*/
bool do_loop(program* prog)
{
    if ( strsame((prog->tokens[prog->curr_word]), "DO") ){
        prog->curr_word++;
        var(prog);
        if ( strsame((prog->tokens[prog->curr_word]), "FROM") ){
            prog->curr_word++;
            varnum(prog);
            if ( strsame((prog->tokens[prog->curr_word]), "TO") ){
                prog->curr_word++;
                varnum(prog);
                if ( strsame((prog->tokens[prog->curr_word]), "{") ){
                     prog->curr_word++;
                     instrctlst(prog);
                     return true;
                }
                else {
                    ERROR("Missing an opening bracket '{' ?")
                }
            }
            else {
                ERROR("Missing a TO?")
            }
        }
        else {
            ERROR("Missing a FROM?")
        }
    }
    else {
        return false;
    }
}

/* 
    Checks the <VAR> part of the grammar 
*/
void var(program* prog)
{
    char ch;

    if ( strlen(prog->tokens[prog->curr_word]) != VAR_LEN ){
        ERROR("Variables can be one defined as A-Z only?")
    } 
    
    ch = prog->tokens[prog->curr_word][FIRST_CHAR];
    
    if ( (ch >= 'A') && (ch <= 'Z') ){
        prog->curr_word++;
        return;
    }

    ERROR("Expected a number or variable?")

}

/* 
    Checks the <VARNUM> part of the grammar 
*/
void varnum(program* prog)
{
    float f;
    bool num_flag; 

    num_flag = sscanf(prog->tokens[prog->curr_word], "%f", &f);
    
    if (num_flag){
        prog->curr_word++;
        return;
    }

    var(prog);

}

/* 
    Checks the <SET> part of the grammar 
*/
bool set(program* prog)
{
    if (strsame(prog->tokens[prog->curr_word], "SET")){
        prog->curr_word++;
        var(prog);
        if ( strsame(prog->tokens[prog->curr_word], ":=") ){
            prog->curr_word++;
            polish(prog);
            return true;
        }
        else {
            ERROR("Missing a ':=' ?")
        }
    }
    else {
        return false;
    }
}

/* 
    Checks the <POLISH> part of the grammar 
*/
void polish(program* prog)
{
    if ( strsame(prog->tokens[prog->curr_word], ";")){
        prog->curr_word++;
        return;
    } 
    else if (op(prog)){
        polish(prog);
    } 
    else {
        varnum(prog);
        polish(prog);
    }
}

/* 
    Checks if the token is a valid operator
*/
bool op(program* prog)
{
    if (strsame(prog->tokens[prog->curr_word], "+")){
        prog->curr_word++;
        return true;
    } 
    else if (strsame(prog->tokens[prog->curr_word], "-")){
        prog->curr_word++;
        return true;
    }
    else if (strsame(prog->tokens[prog->curr_word], "*")){
        prog->curr_word++;
        return true;
    }
    else if (strsame(prog->tokens[prog->curr_word], "/")){
        prog->curr_word++;
        return true;
    }
    else {
        return false;
    }
}
