# Turtle Assembler
Turtle Assembler provides users with 5 general purpose, 8-bit registers (r0 - r4)
and the following assembly instructions:

1. MVR r_d varnum
   Moves into register r_d a decimal number or the decimal number represented by 
   a variable

2. LDR r_d r_op1
   Copies the contents of register r_op1 to r_d

3. STR r_d var
   Copies the contents of r_d into a variable var.

4. AND r_d r_op1 r_op2
   Bitwise AND of r_op1 and r_op2 and result stored in r_d

5. ORR r_d r_op1 r_op2
   Bitwise OR of r_op1 and r_op2 and result stored in r_d

6. XOR r_d r_op1 r_op2
   Bitwise XOR of r_op1 and r_op2 and result stored in r_d

7. NOT r_d 
   Bitwise NOT of r_d

8. LSL r_d r_op1
   Logical Shift Left of r_d by r_op1

9. LSR r_d r_op1
   Logical Shift Right of r_d by r_op1


# Pen Colour
Additionally, the user has the option of modifying the colour of the pen by teweaking the red, green and blue components.

1. PENCOL_R varnum - modifies the red component of the line colour

2. PENCOL_G varnum - modifies the green component of the line colour

3. PENCOL_B varnum - modifies the blue component of the line colour

Each of these components can take a value between 0-255, 
i.e. an unsigned 8-bit number, which is exactly the size of the registers 
provided by the Turtle Assembler.

For example, if the B component of the RGB triplet was 0 and we would
like to make it 255.  

The following code would do it:
 
```       
    MVR r0 1
    
    DO A FROM 1 TO 7 {
        MVR r1 1
        MVR r2 A
        LSL r1 r2
        ORR r0 r0 r1
    }
    
    STR r0 B
    
    PENCOL_B := B
```

And this would set the line colour to blue.

We are essentially setting each bit going from right to left and can visualise 
this as we draw using this new pen colour on the screen. Using this example, 
a user can visualise bitwise operations, elementary assembly, and looping. 

This is indeed a more tedious way of doing things, but I believe the learning
potential for the user is a lot higher.

This is just one of the infinite examples possible using this extension.

# Rationale for Developement
I noticed the lack of fun and interactive ways to learn ARM assembly and thought
I'd do something about it. Moreover, rather than developing something completely 
novel, it made more sense to repurpose and extend Turtle Graphics to incorporate 
basic assembly instructions and take advantage of its existing features. 
I believe this is an important aspect in software engineering, where it is 
important to add value by taking advantage of current technologies and repurposing 
them, rather than developing new code from scratch just because there is an
abundance of venture capitalist money and developers.

Moreover, most modern programming languages have pretty much abstracted away the 
intricacies of code, with modern day developers having almost little, to no clue 
as to what is happening at the assembly level. This extension should hopefully
act as some sort of taster to assembly.

I have included some examples in this [folder](assembler/samples) so kindly do check it out!

===============================================================================
New Formal Grammar
===============================================================================

I added the modulo operator as well and is denoted by the '|' operator.
That was the only change to the grammar from the interpreter.

Here's the new grammar:

```
<MAIN> ::= "{" <INSTRCTLST>
<INSTRCTLST> ::= <INSTRUCTION><INSTRCTLST> | "}"
<INSTRUCTION> ::= <FD> |
                  <LT> |
                  <RT> |
                  <DO> |
                  <SET> |
                  <MOVETO> |
                  <PENCOL_R> |
                  <PENCOL_G> |
                  <PENCOL_B> |
                  <MVR> |
                  <STR> |
                  <LDR> |
                  <AND> |
                  <ORR> |
                  <XOR> |
                  <NOT> |
                  <LSL> |
                  <LSR> |

<FD> ::= "FD" <VARNUM>
<LT> ::= "LT" <VARNUM>
<RT> ::= "RT" <VARNUM>
<DO> ::= "DO" <VAR> "FROM" <VARNUM> "TO" <VARNUM> "{" <INSTRCTLST>
<VAR> ::= [A-Z]
<VARNUM> ::= number | <VAR>
<SET> ::= "SET" <VAR> ":=" <POLISH>
<POLISH> ::= <OP><POLISH> | <VARNUM><POLISH> | ";"
<OP> ::= "+" | "-" | "*" | "/" | "|"
<MOVETO> ::= <VARNUM> <VARNUM>
<PENCOL_R> ::= "PENCOL_R" ":=" <VARNUM>
<PENCOL_G> ::= "PENCOL_G" ":=" <VARNUM>
<PENCOL_B> ::= "PENCOL_B" ":=" <VARNUM>
<MVR> ::= "MVR" <REG> <VARNUM>
<STR> ::= "STR" <REG> <VAR>
<LDR> ::= "LDR" <REG> <REG>
<AND> ::= "AND" <REG> <REG> <REG>
<ORR> ::= "ORR" <REG> <REG> <REG>
<XOR> ::= "XOR" <REG> <REG> <REG>
<NOT> ::= "NOT" <REG>
<LSL> ::= "LSL" <REG> <REG> <REG>
<LSR> ::= "LSR" <REG> <REG> <REG>
<REG> ::= "r0" | "r1" | "r2" | "r3" | "r4"
```
