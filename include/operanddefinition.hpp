/* This file holds things that help with operands */

#ifndef VALUE_ASM_HPP
#define VALUE_ASM_HPP

#include<symboltable.hpp>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>

namespace operand
{
    // struct asm_operand_def // TODO: This struct should contain the register of this value if we subsitute a register for it
    struct operand_def
    {
        i32 const_value; // The value of this function if it's const
        variable_token* accessed_variable = 0; // If we are accessing a variable a pointer to it
        function_token* called_function = 0; // If we are calling a function a pointer to it
        u8 final_type = 0; // The type this operation would result in
        operand_def(i32 _const_value, variable_token *_accessed_variable, function_token *_called_function, u8 _final_type) 
        : const_value(_const_value), accessed_variable(_accessed_variable), called_function(_called_function), final_type(_final_type) {};
    };
}

#endif