#include "interpreter.h"

/*============================================================*/
            /* Assembler Extension */
/*============================================================*/

/* 
    Moves the turtle to the coordinates specified
    after the <MOVETO> instruction
*/
bool moveto(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "MOVETO")){
        prog->curr_word++;
        varnum(prog, turt);
        x_interpret(turt);
        varnum(prog,turt);
        y_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Updates the x coordinate of the turtle after a <MOVETO>
*/
void x_interpret(turtle* turt)
{
    if(turt->constant_flag){
        turt->curr_x = turt->constant_value + WWIDTH/2;
        return;
    }

    if(turt->variable_flag){
        turt->curr_x = turt->variable_array[turt->variable_value] + WWIDTH/2;
    }

}

/* 
    Updates the y coordinate of the turtle after a <MOVETO>
*/
void y_interpret(turtle* turt)
{
    if(turt->constant_flag){
        turt->curr_y = turt->constant_value + WHEIGHT/2;
        return;
    }

    if(turt->variable_flag){
        turt->curr_y = turt->variable_array[turt->variable_value] + WHEIGHT/2;
    }
}

/* 
    Changes the R component in the RGB triplet
    for <PENCOL_R> instruction
*/
bool pencol_r(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "PENCOL_R")){
        prog->curr_word++;
        if (strsame(prog->tokens[prog->curr_word], ":=")){
            prog->curr_word++;
            varnum(prog, turt);
            pencol_r_interpret(turt);
            return true;
        }
        else {
            ERROR("Was expecting a ':=' ?")
        }   
    }
    else {
        return false;
    }
}

/* 
    Changes the G component in the RGB triplet
    for <PENCOL_G> instruction
*/
bool pencol_g(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "PENCOL_G")){
        prog->curr_word++;
        if (strsame(prog->tokens[prog->curr_word], ":=")){
            prog->curr_word++;
            varnum(prog, turt);
            pencol_g_interpret(turt);
            return true;
        }
        else {
            ERROR("Was expecting a ':=' ?")
        } 
    }
    else {
        return false;
    }
}

/* 
    Changes the B component in the RGB triplet after
    for <PENCOL_B> instruction
*/
bool pencol_b(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "PENCOL_B")){
        prog->curr_word++;
        if (strsame(prog->tokens[prog->curr_word], ":=")){
            prog->curr_word++;
            varnum(prog, turt);
            pencol_b_interpret(turt);
            return true;
        }
        else {
            ERROR("Was expecting a ':=' ?")
        }
    }
    else {
        return false;
    }
}

/* 
    Interprets the <PENCOL_R> instruction
*/
void pencol_r_interpret(turtle* turt)
{
    int i;
    i = check_uint8(turt);
    turt->rgb.red = (Uint8)i;
}

/* 
    Interprets the <PENCOL_G> instruction
*/
void pencol_g_interpret(turtle* turt)
{
    int i;
    i = check_uint8(turt);
    turt->rgb.green = (Uint8)i;
}

/* 
    Interprets the <PENCOL_B> instruction
*/
void pencol_b_interpret(turtle* turt)
{
    int i;
    i = check_uint8(turt);
    turt->rgb.blue = (Uint8)i;
}

/* 
    Checks that a number is between 0 - 255
*/
int check_uint8(turtle* turt)
{
    int i;
    
    if (turt->constant_flag){
        i = turt->constant_value;
    }
    
    if (turt->variable_flag){
        i = turt->variable_array[turt->variable_value];
    }

    if (i < UINT8_MIN || i > UINT8_MAX){
        ERROR("Uint8 value can be only be between 0 - 255?")
    }

    return i;
}

/*============================================================*/
                /* Turtle Assembler Extension */
/*============================================================*/

/* 
    Follows the <MVR> part of the grammar
*/
bool mvr(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "MVR")){
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        varnum(prog, turt);
        mvr_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Follows the <LDR> part of the grammar
*/

bool ldr(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "LDR")){
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        turt->rgstrs[turt->dst_reg] = turt->rgstrs[turt->op1_reg]; 
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Follows the <STR> part of the grammar
*/
bool str(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "STR")){
        prog->curr_word++;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        varnum(prog, turt);
        str_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Follows the <AND> part of the grammar
*/
bool bw_and(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "AND")){
        turt->current_bitwise = and;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        reg(prog, turt);
        turt->op2_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    } 
    else {
        return false;
    }
}

/* 
    Follows the <ORR> part of the grammar
*/
bool bw_orr(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "ORR")){
        turt->current_bitwise = or;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        reg(prog, turt);
        turt->op2_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Follows the <XOR> part of the grammar
*/
bool bw_xor(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "XOR")){
        turt->current_bitwise = xor;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        reg(prog, turt);
        turt->op2_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Follows the <NOT> part of the grammar
*/
bool bw_not(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "NOT")){
        turt->current_bitwise = not;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

bool bw_lsl(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "LSL")){
        turt->current_bitwise = lsl;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Follows the <LSR> part of the grammar
*/
bool bw_lsr(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "LSR")){
        turt->current_bitwise = lsr;
        prog->curr_word++;
        reg(prog, turt);
        turt->dst_reg = turt->current_reg;
        reg(prog, turt);
        turt->op1_reg = turt->current_reg;
        bw_interpret(turt);
        return true;
    }
    else {
        return false;
    }
}

/* 
    Interprets the <MVR> part of the grammar
*/
void mvr_interpret(turtle* turt)
{
    int i;
    i = check_uint8(turt);
    turt->rgstrs[turt->dst_reg] = (Uint8)i; 
}

/* 
    Interprets the <STR> part of the grammar
*/
void str_interpret(turtle* turt)
{
    if (!turt->variable_flag){
        ERROR("Expect an STR instruction to be STR <REG> <VAR> ?");
    }

    turt->variable_array[turt->variable_value] = \
    turt->rgstrs[turt->op1_reg];
}

/* 
    Interprets the all of the bitwise
    operations of the grammar
*/
void bw_interpret(turtle* turt)
{
    bitwise bw;
    bw = turt->current_bitwise;

    switch (bw){
        case and:
            turt->rgstrs[turt->dst_reg] = \
            turt->rgstrs[turt->op1_reg] & turt->rgstrs[turt->op2_reg];
            return;
        case or:
            turt->rgstrs[turt->dst_reg] = \
            turt->rgstrs[turt->op1_reg] | turt->rgstrs[turt->op2_reg];
            return;
        case xor:
            turt->rgstrs[turt->dst_reg] = \
            turt->rgstrs[turt->op1_reg] ^ turt->rgstrs[turt->op2_reg];
            return;
        case not:
            turt->rgstrs[turt->dst_reg] = \
            ~(turt->rgstrs[turt->dst_reg]);
            return;
        case lsl:
            turt->rgstrs[turt->dst_reg] = \
            turt->rgstrs[turt->dst_reg] << turt->rgstrs[turt->op1_reg];
            return;
        case lsr:
            turt->rgstrs[turt->dst_reg] = \
            turt->rgstrs[turt->dst_reg] >> turt->rgstrs[turt->op1_reg];
            return;         
        default:
            ERROR("Bitwise operator not set?");
    }
}

/* 
    Interprets the <REG> part of the grammar
*/
void reg(program* prog, turtle* turt)
{
    if (strsame(prog->tokens[prog->curr_word], "r0")){
        turt->current_reg = r0;
        prog->curr_word++;
        return;
    } 
    else if (strsame(prog->tokens[prog->curr_word], "r1")){
        turt->current_reg = r1;
        prog->curr_word++;
        return;
    }
    else if (strsame(prog->tokens[prog->curr_word], "r2")){
        turt->current_reg = r2;
        prog->curr_word++;
        return;
    }
    else if (strsame(prog->tokens[prog->curr_word], "r3")){
        turt->current_reg = r3;
        prog->curr_word++;
        return;
    }
    else if (strsame(prog->tokens[prog->curr_word], "r4")){
        turt->current_reg = r4;
        prog->curr_word++;
        return;
    }
    else {
        ERROR("Enter a valid register between r0 - r4");
    }    
}
