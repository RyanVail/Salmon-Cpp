# The Salmon Programming Language

This is a compiler for an esoteric-like programming language that is proably going to be called Salmon.
<br><br>
# Contents

- [Concepts](#concepts)
    - [Reverse Polish Notation](#reverse-polish-notation-rpn)
    - [Semicolons](#semicolons)
    - [New Lines](#new-lines)
- [Examples](#examples)
    - [Defining a Variable](#defining-a-variable)
    - [The Fibonacci Sequence](#the-fibonacci-sequence)
    - [3n-1](#3n-1)
- [Quirks](#quirks)
- [Problems and TODOs](#problems-and-todos)

# Concepts

- ### [Reverse Polish Notation (RPN)](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
    - RPN is a mathematical notation in which operators follow their operands
    - Infix Notation (normal notation):
        ```
        1 + 2 - 3
        ```
    - Reverse Polish Notation
        ```
        3 2 1 + -
        ```
- ### Semicolons
    - Semicolons will clear the current stack
        ```
        1 2 ;
        ```
- ### New Lines
    - New lines should be added when the stack is empty (It will still compile fine if they aren't)
        ```
        u32 a 3 =
        u32 b 1 =
        1 2 + ;
        a a b + =
        ```

# Examples

- ### Defining a Variable
    - After assigning a value to a variable both the variable and the value get removed from the stack
        ```
        u32 a 2 =
        u32 b 1 =
        ```
    - Redefining a variable
        ```
        u32 a 23 =
        a 2 =
        ```



- ### The Fibonacci sequence
    ```    
    u32 a 1 = 
    u32 b 1 =
    u32 c 1 =  
    1 while {
        c a = 
        a a b + = 
        b c = 
        1
    }
    ```
- ### 3n-1
    ```
    u32 a 7 = 
    1 while 
    { 
        a 1 & if { 
            a 1 a 3 * + = 
        } 
        a 1 & ! if 
        { 
            a a 2 * = 
        } 
        a 1 != 
    }
    ```

# Quirks

# Problems and TODOs
- ## General
    - ### **TYPES ARE JUST NOT USED LIKE AT ALL!**
    - ### There is no support for signed types
    - ### Add namespaces to all files so as to not clutter up the global space
    - ### Add some tests that are run automatically and are scored automatically
    - ### Make it so "main.cpp" stops reading a file when it gets zero bytes
    - ### "aarch32_asm.cpp" & "intermediate.cpp" contain a lot of inlined functions designed to make it simpler, but now after a few hundred more lines of code it just compilcates things **REMOVE IT!**
    - ### Arrays have defined lengths right now which is just a dumb
- ## Comments
    - ### Comment removal should be moved from being done in the intermediate step to the tokenizer
        - #### The lack of pre comment removal means that things like checking for '{' after "if" is broken
- ## Functions
    - ### Function defintions and function calls just don't work right now
    - ### Functions should keep the value they get as input (if it is one value) in R0 rather than offloading it to the stack
- ## Intermediate
    - ### Intermediate codes should use an enum for intermediate codes to make the code a little cleaner
    - ### Intermediate should use a stack to represent values like "aarch32_asm.cpp" does now
        - #### Add a way of adding psuedo break points that are done in intermediate.cpp that stop compiling the program and print the # of things on the stack and what is one the stack
        - #### A lot of things involving stacks can be removed from "aarch32_asm.cpp"
- ## ASM
    - ### "aarch32_asm" Uses an array of two registers to represent the values in R0 and R1 but R1 index is never used, it would be better to use a single value_definition
- ## README
    - ### [Quirks](#quirks) should contain the weird quirks involving the starting and ending of if and while statments