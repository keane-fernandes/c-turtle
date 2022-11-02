#include "interpreter.h"

/* Interpreter Functions */
void init_turtle(turtle* turt)
{
    int i;

    turt->curr_x = WWIDTH/2;
    turt->curr_y = WHEIGHT/2;
    turt->curr_angle = ANGLE_MIN;
    
    turt->constant_flag = false;
    turt->constant_value = ZEROF;
    
    turt->variable_flag = false;
    turt->variable_value = ZERO;

    for (i = 0; i < MAXVARS; i++){
        turt->variable_array[i] = ZERO;
    }

    turt->curr_loop = UNDEFINED;
    for (i = 0; i < MAXLOOPS; i++){
        turt->loop_stack[i].decrement = false;
        turt->loop_stack[i].index_end = ZERO;
        turt->loop_stack[i].index_current = ZERO;
        turt->loop_stack[i].prog_start = ZERO;
        turt->loop_stack[i].variable = ZERO;
    }

    turt->set_variable = ZERO;
    turt->current_operator = UNDEFINED;
}

/* 
    Moves the turtle forward by the distance specified 
    in the turt structure
*/
void fd_interpret(program* prog, turtle* turt)
{  
    float new_x, new_y, delta, angle;
    
    /* Convert degrees to radians */
    angle = (turt->curr_angle) * (RADCONVERTER); 

    if (turt->constant_flag == true){
        delta = turt->constant_value; 
        new_x = turt->curr_x + ( delta * ( cos(angle) ) );
        new_y = turt->curr_y + ( delta * ( sin(angle) ) );
    } 
    else if (turt->variable_flag == true) {
        delta = turt->variable_array[turt->variable_value];
        new_x = turt->curr_x + ( delta * ( cos(angle) ) );
        new_y = turt->curr_y + ( delta * ( sin(angle) ) );
    }
    else {
        ERROR("Error in FD instruction execution: \
            was expecting a number or variable?")
    }
    
    if (new_x < ZERO || new_x > WWIDTH || \
            new_y < ZERO || new_y > WHEIGHT){
        ERROR("Cannot draw outside of bounds of SDL Window!")
    }
    
    /*move_turtle(prog, turt, new_x, new_y);*/
    
    /* Added this because of -Wunused-parameter flag during compilation */
    prog->curr_word++;
    prog->curr_word--;

    
    /* Update new co-ordinates of turtle */
    if (!update_coords(turt, new_x, new_y)){  
        ERROR("Could not update new co-ordinates \
            of turtle structure.")
    }
}

/* 
    Moves the turtle on the screen using SDL from 
    current position to the new position
*/
void move_turtle(program* prog, turtle* turt, float new_x, float new_y)
{
    SDL_Delay(MILLISECONDDELAY);
    SDL_RenderDrawLine(prog->sw.renderer, turt->curr_x, \
        turt->curr_y, new_x, new_y);
    Neill_SDL_UpdateScreen(&(prog->sw));
    Neill_SDL_Events((&prog->sw));
}

/* 
    Subtracts the angle from the current angle orientation
    of the turtle. 
*/
void lt_interpret(turtle* turt)
{  
    if (turt){
        if (turt->constant_flag == true){
            turt->curr_angle -= turt->constant_value;
            recalibrate_angle(turt);
        }
        else if (turt->variable_flag == true){
            turt->curr_angle -= \
                turt->variable_array[turt->variable_value];
            recalibrate_angle(turt);
        }
        else {
            ERROR("Error in LT instruction execution: \
                was expecting a number or variable?");
        }
    }
}

/* 
    Adds the angle from the current angle orientation
    of the turtle. 
*/
void rt_interpret(turtle* turt)
{  
    if (turt){
        if (turt->constant_flag == true){
            turt->curr_angle += turt->constant_value;
            recalibrate_angle(turt);
        }
        else if (turt->variable_flag == true){
            turt->curr_angle += \
                turt->variable_array[turt->variable_value];
            recalibrate_angle(turt);
        }
        else {
            ERROR("Error in RT instruction execution: \
                was expecting the constant or variable flag to be set?");
        }
    }
}

/* 
    Recalibrates the current angle of the turtle
    to ensure it stays between 0 and 360
*/

void recalibrate_angle(turtle* turt)
{
    if (turt->curr_angle > ANGLE_MAX || \
        turt->curr_angle < (-ANGLE_MAX)){
        turt->curr_angle = fmod(turt->curr_angle, ANGLE_MAX);
        return;
    }
}

/* 
    The <NUM> branch of the <VARNUM> instruction
    calls this function to set the constant flag
    on and updates the value of the constant 
*/
bool update_constant(turtle* turt, float f)
{
    if (turt){
        turt->constant_flag = true;
        turt->variable_flag = false;
        turt->constant_value = f;
        return true;
    }
    else {
        return false;
    }
}

/* 
    The <VAR> branch of the <VARNUM> instruction
    calls this function to set the variable flag
    on and updates the index of the variable in
    the master A-Z variable array
*/
bool update_variable(turtle* turt, int i)
{
    if (turt){
        turt->variable_flag = true;
        turt->constant_flag = false;
        turt->variable_value = i - ASCII_A;
        return true;
    }
    else{
        return false;
    }
}

/* Updates new position of turtle in the turt structure */
bool update_coords(turtle* turt, float new_x, float new_y)
{
    if (turt){
        turt->curr_x = new_x;
        turt->curr_y = new_y;
        return true;
    }
    else {
        return false;
    }
}

/* Assigns a variable to the current loop frame */
bool do_loop_start(turtle* turt)
{
    if ( (turt->variable_flag) == true ){
        turt->loop_stack[turt->curr_loop].variable = 
            turt->variable_value;
        
        return true;
    }
    
    return false;   
}

/* 
    Checks if the parsed token is a variable
    or a constant and assigns this to the 
    current index of the current loop frame
*/
bool do_loop_from(turtle* turt)
{
    if (turt->constant_flag == true){
        turt->loop_stack[turt->curr_loop].index_current = 
            turt->constant_value;
    } else if (turt->variable_flag == true){
        turt->loop_stack[turt->curr_loop].index_current = 
            turt->variable_array[turt->variable_value];
    } 
    else {
        return false;
    }

    /* Copy this value into global variable array */
    turt->variable_array[turt->loop_stack[turt->curr_loop].variable] = 
        turt->loop_stack[turt->curr_loop].index_current;
    
    return true;
}

/* 
    Checks if the parsed token is a variable
    or a constant and assigns this to the 
    end index of the current loop frame
*/
bool do_loop_to(turtle* turt)
{
    if (turt->constant_flag == true){
        turt->loop_stack[turt->curr_loop].index_end = 
            turt->constant_value;
        
        return true;
    }
    else if (turt->variable_flag == true){
        turt->loop_stack[turt->curr_loop].index_end = 
            turt->variable_array[turt->variable_value];
        
        return true;
    }
    else {
        return false;
    }    
}

/* 
    Checks if starting index is greater than end index
    and sets the decrement flag to instruct the interpreter
    to decrement at the end of each iteration
*/
void do_loop_pre(turtle* turt)
{
    if (turt->loop_stack[turt->curr_loop].index_current > \
        turt->loop_stack[turt->curr_loop].index_end){
        
        turt->loop_stack[turt->curr_loop].decrement = true;
    }
}

/* Increments / Decrements the loop variable */
void do_loop_post(turtle* turt)
{
    if (turt->loop_stack[turt->curr_loop].decrement == true){
        turt->variable_array[turt->loop_stack[turt->curr_loop].variable]--;
        return;
    }
    turt->variable_array[turt->loop_stack[turt->curr_loop].variable]++;
}

/* Checks if the termination criteria for the do loop is reached */
bool do_loop_finished(turtle* turt)
{
    if (turt->loop_stack[turt->curr_loop].decrement == true){
        if (turt->loop_stack[turt->curr_loop].index_current < \
            turt->loop_stack[turt->curr_loop].index_end){
                return true;
            }
    }
    if (turt->loop_stack[turt->curr_loop].decrement == false){
        if (turt->loop_stack[turt->curr_loop].index_current > \
            turt->loop_stack[turt->curr_loop].index_end){
                return true;
            }
    }
    
    return false;
}

/* 
    Checks if the parsed token is a variable
    and assigns it to set_variable in the 
    turt structure
*/
bool set_init(turtle* turt)
{
    if (turt->variable_flag == true)
    {
        turt->set_variable = 
            turt->variable_value;
        return true;
    }
    
    return false;
}

/* Initialises a stack to evaluate a postfix expression */
bool polish_init(stack* s)
{
    int i;
    if (s){
        s->size = ZERO;
        s->capacity = MAXSTACKELEMS;

        for (i = 0; i < MAXSTACKELEMS; i++){
            s->polish_stack[i] = ZERO;
        }
        return true;
    }
    
    return false;
}

/* Pushes a token onto the stack */
bool stack_push(turtle* turt, stack* s)
{
    if (turt == NULL || s == NULL){
        ERROR("Unable to access turtle or stack?")
    }
    
    if (s->size >= MAXSTACKELEMS){
        ERROR("Stack overflow: unable to push to stack")
    }
    
    if (turt->constant_flag == true){
        s->polish_stack[s->size] = turt->constant_value;
        s->size++;
        return true;
    } 
    
    if (turt->variable_flag == true){
        s->polish_stack[s->size] = 
            turt->variable_array[turt->variable_value];
        s->size++;
        return true;
    }

    return false;
}

/* Pops a token off the stack */
bool stack_pop(float* dst, stack* s)
{
    if (dst == NULL || s == NULL){
        return false;
    }
    
    if (s->size == 0){
        return false;
    }
    
    *dst = s->polish_stack[s->size-1];
    s->size--;
    return true;
}

/* 
    When an operator is scanned, two elements are popped
    off the stack and operated on depending on the operator
*/
bool stack_operate(turtle* turt, stack* s)
{
    float operand1, operand2, result;
    
    if( (!stack_pop(&operand2, s)) || (!stack_pop(&operand1, s)) ){
        return false;
    }

    switch (turt->current_operator){
        case add:
            result = operand1 + operand2;
            break;
        case subtract:
            result = operand1 - operand2;
            break;
        case multiply:
            result = operand1 * operand2;
            break;
        case divide:
            result = operand1 / operand2;
            break;
        default:
            return false;
    }
    
    if(!update_constant(turt, result)){
        return false;
    };

    if(!stack_push(turt, s)){
        return false;
    }

    return true;
}

/* Evaluates the current state of the stack and frees it */
bool polish_eval(turtle* turt, stack* s)
{
    if (s){
        if (s->size != 1) {
            ERROR("The RPN formula has an error")
        }

        turt->variable_array[turt->set_variable] = 
            s->polish_stack[s->size-1];

        return true;
    }

    return false;
}
