#include "interpreter.h"

/* 
    Checks that the program starts with a "{"
    and moves on to the <INSTRCTLIST> part 
*/
void parse_and_interpret_this(program* prog, turtle* turt)
{    
    if (!strsame((prog->tokens[prog->curr_word]), "{")){
        ERROR("Missing an opening curly brace '{' ?")
    }

    prog->curr_word++;
    instrctlst(prog, turt);
}

/* Checks for a "}" | <INSTRUCTION><INSTRCTLST> */
void instrctlst(program* prog, turtle* turt)
{
    if (strsame((prog->tokens[prog->curr_word]), "}")){
        prog->curr_word++;
        return;
    }
    instruction(prog, turt);
    instrctlst(prog, turt);
}

/* 
    Checks for the valid functions defined in <INSTRUCTION>
    and returns an error if none of the defined functions
    return true 
*/
void instruction(program* prog, turtle* turt)
{
    if (fd(prog, turt)) {
        fd_interpret(prog, turt);
        return;
    }
    else if (rt(prog, turt)){
        rt_interpret(turt);
        return;
    }
    else if (lt(prog, turt)){
        lt_interpret(turt);
        return;
    }
    else if (do_loop(prog, turt)){
        return;
    }
    else if (set(prog, turt)){
        return;
    }
    else if (moveto(prog, turt)){
        return;
    }
    else if (pencol_r(prog, turt)){
        return;
    }
    else if (pencol_g(prog, turt)){
        return;
    }
    else if (pencol_b(prog, turt)){
        return;
    }
    else if (mvr(prog, turt)){
        return;
    }
    else if (ldr(prog, turt)){
        return;
    }
    else if (str(prog, turt)){
        return;
    }
    else if (bw_and(prog, turt)){
        return;
    }
    else if (bw_orr(prog, turt)){
        return;
    }
    else if (bw_xor(prog, turt)){
        return;
    }
    else if (bw_not(prog, turt)){
        return;
    }
    else if (bw_lsl(prog, turt)){
        return;
    }
    else if (bw_lsr(prog, turt)){
        return;
    }
    else {
        ERROR("Invalid instruction?")
    }   
}

/* 
    Checks if FD command follows the right format
    and returns true/false 
*/
bool fd(program* prog, turtle* turt)
{
    if ( strsame((prog->tokens[prog->curr_word]), "FD") ){
        prog->curr_word++;
        varnum(prog, turt);
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
bool lt(program* prog, turtle* turt)
{
    if ( strsame((prog->tokens[prog->curr_word]), "LT") ){
        prog->curr_word++;
        varnum(prog, turt);
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
bool rt(program* prog, turtle* turt)
{
    if ( strsame((prog->tokens[prog->curr_word]), "RT") ){
        prog->curr_word++;
        varnum(prog, turt);
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Checks the <DO> part of the grammar
*/
bool do_loop(program* prog, turtle* turt)
{
    if ( strsame((prog->tokens[prog->curr_word]), "DO") ){
        prog->curr_word++;
        turt->curr_loop++;
        
        if (turt->curr_loop >= MAXVARS){
            ERROR("Maximum number of loop variables assigned.")
        }
        
        var(prog, turt);
        if(!do_loop_start(turt)){
            ERROR("A variable needs to be set for a <DO> instruction?")
        }
        if ( strsame((prog->tokens[prog->curr_word]), "FROM") ){
            prog->curr_word++;
            varnum(prog, turt);
            if(!do_loop_from(turt)){
                ERROR("Expecting a variable or number after \
                        FROM instruction?")
            }
            if ( strsame((prog->tokens[prog->curr_word]), "TO") ){
                prog->curr_word++;
                varnum(prog, turt);
                if(!do_loop_to(turt)){
                    ERROR("Expecting a variable or number after \
                        TO instruction?")
                }
                if ( strsame((prog->tokens[prog->curr_word]), "{") ){
                    prog->curr_word++;
                    
                    /* Starting token for current loop frame */
                    turt->loop_stack[turt->curr_loop].prog_start = prog->curr_word;

                    /* Set the decrement counter if necessaey */
                    do_loop_pre(turt);

                    do{
                        /* Set the current token to the loop start token */
                        prog->curr_word = \
                            turt->loop_stack[turt->curr_loop].prog_start;
                        
                        /* Set the current index of the loop to present value of variable */
                        turt->loop_stack[turt->curr_loop].index_current = 
                            turt->variable_array[turt->loop_stack[turt->curr_loop].variable];
                        
                        instrctlst(prog, turt);
                         
                        /* Set the current index of the loop to present value of variable 
                        it was assigned as it may have changed during loop execution */
                        do_loop_post(turt);
                        turt->loop_stack[turt->curr_loop].index_current = 
                            turt->variable_array[turt->loop_stack[turt->curr_loop].variable];
                                             
                    } while (!do_loop_finished(turt));
                    /* Decrement loop stack */
                    turt->curr_loop--;
                    return true;
                }
                else {
                    ERROR("Missing an opening bracket '{' ?")
                }
            }
            else {
                ERROR("Missing a 'TO' ?")
            }
        }
        else {
            ERROR("Missing a 'FROM' ?")
        }
    }
    else {
        return false;
    }
}

/* 
    Checks the <VAR> part of the grammar 
*/
void var(program* prog, turtle* turt)
{
    int i;

    if ( strlen(prog->tokens[prog->curr_word]) != VAR_LEN ){
        ERROR("Variables can be one defined as A-Z only?")
    } 
    
    i = prog->tokens[prog->curr_word][FIRST_CHAR];
    
    if ( (i >= ASCII_A) && (i <= ASCII_Z) ){
        update_variable(turt, i);
        prog->curr_word++;
        return;
    }

    ERROR("Expected a number or variable?")

}

/* 
    Checks the <VARNUM> part of the grammar 
*/
void varnum(program* prog, turtle* turt)
{
    float f;
    bool num_flag;

    num_flag = sscanf(prog->tokens[prog->curr_word], "%f", &f);
    
    if (num_flag){
        if (update_constant(turt, f)){
            prog->curr_word++;
            return;     
        }
        else {
            ERROR("Error in <VARNUM>: Unable to \
                update constant.")
        }
    }
    var(prog, turt);
}

/* 
    Checks the <SET> part of the grammar 
*/
bool set(program* prog, turtle* turt)
{
    stack rpn;
    
    if (strsame(prog->tokens[prog->curr_word], "SET")){
        prog->curr_word++;
        var(prog, turt);
        if(!set_init(turt)){
            ERROR("The variable has not been set in <SET>")
        }
        if (strsame(prog->tokens[prog->curr_word], ":=") ){
            prog->curr_word++;
            
            if (!polish_init(&rpn)){
                ERROR("Unable to initialise stack for RPN \
                    evaluation in <SET>")
            }
                
            polish(prog, turt, &rpn);

            if(!polish_eval(turt, &rpn)){
                ERROR("There was an error in the RPN \
                    expression in <SET>")
            }

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
void polish(program* prog, turtle* turt, stack* s)
{
    if ( strsame(prog->tokens[prog->curr_word], ";")){
        prog->curr_word++;
        return;
    } 
    else if (op(prog, turt)){
        if(!stack_operate(turt, s)){
            ERROR("There needs to be a minmum of two \
                operands on the stack for RPN evaluation")
        }
        polish(prog, turt, s);
    } 
    else {
        varnum(prog, turt);
        if(!stack_push(turt, s)){
            ERROR("Unable to push to stack for RPN evaluation")
        };
        polish(prog, turt, s);
    }
}

/* 
    Checks if the token is a valid operator
*/
bool op(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "+")){
        turt->current_operator = add;
        prog->curr_word++;
        return true;
    } 
    else if (strsame(prog->tokens[prog->curr_word], "-")){
        turt->current_operator = subtract;
        prog->curr_word++;
        return true;
    }
    else if (strsame(prog->tokens[prog->curr_word], "*")){
        turt->current_operator = multiply;
        prog->curr_word++;
        return true;
    }
    else if (strsame(prog->tokens[prog->curr_word], "/")){
        turt->current_operator = divide;
        prog->curr_word++;
        return true;
    }
    else if (strsame(prog->tokens[prog->curr_word], "|")){
        turt->current_operator = modulo;
        prog->curr_word++;
        return true;
    }
    else {
        return false;
    }
}
