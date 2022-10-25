/* This file holds things that help with values in registers and on the stack during intermediate to asm */

#ifndef VALUE_ASM_HPP
#define VALUE_ASM_HPP

#include<symbol_table.hpp>
#include<intermediate/intermediate.hpp>

namespace value_asm
{
    struct value_defintion
    {
        int const_value; // The value of this function if it's const
        variable_token* accessed_variable = 0; // If we are accessing a variable a pointer to it
        function_token* called_function = 0; // If we are calling a function a pointer to it
        unsigned char final_type = 0; // The type this operation would result in
        value_defintion(int _const_value, variable_token *_accessed_variable, function_token *_called_function, unsigned char _final_type) : const_value(_const_value), accessed_variable(_accessed_variable), called_function(_called_function), final_type(_final_type) {};
    };
}

#endif