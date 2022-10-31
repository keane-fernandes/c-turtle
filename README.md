<h1 align="center">
  <br>
    <img src=./docs/turtle.png alt="turtle.png" width="100"></a>
  <br>
    Turtle Assembler
  <br>
</h1>

<h4 align="center">A C implementation of the LOGO programming language with an ARM Assembly extension.</h4>

<p align="center">
  <a href="#Features">Features</a> |
  <a href="#Overview">Overview</a> |
  <a href="#Design">Design</a>     |
  <a href="#Usage">Usage</a>       |
  <a href="#License">License</a>   |
</p>

# Features
- Classic LOGO commands that enable the user to move the turtle around the screen
- ARM assembly instructions extension to introduce assembly programming to beginners
- Test driven development - custom testing harness that performs black box and white box testing

# Overview
Many attempts have been made to create programming languages which are intuitive and easy to learn. One of the best of these was LOGO which allowed children as young as 3 to learn a computer language. A subset of this language involved a “turtle” which could be driven around the screen using simple instructions.

I noticed the lack of fun and interactive ways to learn ARM assembly and thought I'd do something about it. Moreover, rather than developing something completely novel, it made more sense to repurpose and extend the base LOGO language to incorporate basic assembly instructions and take advantage of its existing features. I believe this is an important aspect in software engineering, where it is important to add value by taking advantage of current technologies and repurposing them.

Moreover, most modern programming languages have pretty much abstracted away the intricacies of code, with modern day developers having almost little, to no clue as to what is happening at the assembly level. This program should hopefully act as some sort of taster to assembly.

I have included some examples in this [folder](../assembler/samples) so kindly do check it out!

# Design
The software package is essentially a [recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser) with an interpreter module.

The parser ensures that the code conforms to the [formal grammar](#formal-bnf-grammar).

# Usage
## Setup 
Ensure you have [GCC](https://gcc.gnu.org) installed on your machine. Once you have done so, run the following command on your terminal to compile and run an example of the basic version:

```bash
make turtle
```

## Navigating the repository
The [ttl_files folder](./ttl_files) contains all of the .ttl files used for testing

samples - contains sample .ttl files with screenshots of the output

make extension - will produce the required executable for the extension

make run_test_extension - will compile and run the test harness test_extension.c on the files in ttl_files/Black_box


## Basic Instructions



## Assembly Instructions
Turtle Assembler provides users with 5 general purpose, 8-bit registers (r0 - r4) and the following assembly instructions:

| Instruction           | Description                                                                       |
| --------------------- | --------------------------------------------------------------------------------- |
| `MVR r_d varnum`      | Moves into register r_d a decimal number or the decimal number represented varnum |
| `LDR r_d r_op1`       | Copies the contents of register r_op1 to r_d                                      |
| `STR r_d var`         | Copies the contents of r_d into a variable var.                                   |
| `AND r_d r_op1 r_op2` | Bitwise AND of r_op1 and r_op2 and result stored in r_d                           |
| `ORR r_d r_op1 r_op2` | Bitwise OR of r_op1 and r_op2 and result stored in r_d                            |
| `XOR r_d r_op1 r_op2` | Bitwise XOR of r_op1 and r_op2 and result stored in r_d                           |
| `NOT r_d`             | Bitwise NOT of r_d                                                                |
| `LSL r_d r_op1`       | Logical Shift Left of r_d by r_op1                                                |
| `LSR r_d r_op1`       | Logical Shift Right of r_d by r_op1                                               |

Additionally, the user has the option of modifying the colour of the pen by teweaking the red, green and blue components.

| Instruction       | Description                                     |
| ----------------- | ----------------------------------------------- |
| `PENCOL_R varnum` | modifies the red component of the line colour   |
| `PENCOL_B varnum` | modifies the blue component of the line colour  |
| `PENCOL_G varnum` | modifies the green component of the line colour |

Each of these components can take a value between 0-255, i.e. an unsigned 8-bit number, which is exactly the size of the registers provided by the Turtle Assembler.

For example, if the B component of the RGB triplet was 0 and if the user would like to make it 255, the following code would do it:
 
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

Essentially, setting each bit going from right to left and can visualise this as we draw using this new pen colour on the screen. Using this example, a user can visualise bitwise operations, elementary assembly, and looping. 

This is indeed a more tedious way of doing things, but I believe the learning potential for the user is a lot higher.

This is just one of the infinite examples possible using this extension.

## Formal BNF Grammar

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

# License

```
Copyright (c) 2021 Keane Fernandes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

