/* This does type checking during the tokens to intermediate phase */

#include<intermediate/typechecker.hpp>
#include<vartypes.hpp>
#include<iostream>
#include<typedefs.hpp>
#include<intermediate/intermediate.hpp>

/*
 * first bit is signed flag
 * 0 0 0 0 0 0 0 0
 *
 * bool
 * 0 1 0 0 0 0 0 0
 *
 * 8 bit
 * 0 0 1 0 0 0 0 0
 *
 * 16 bit
 * 0 1 1 0 0 0 0 0
 *   
 * 32 bit
 * 0 0 0 1 0 0 0 0
 *
 * 64 bit
 * 0 1 0 1 0 0 0 0
 *
 * 128 bit
 * 0 0 1 1 0 0 0 0
 */

namespace typechecker
{
    // This will send an error if an operation cannot be done
    void operation_can_be_done()
    {

    }

    // TODO: This should use "get_local_variable_token"
    u8 get_type_of_inter(inter _inter)
    {
        if (_inter.id == CONST)
            return _inter.type;
        if (_inter.id == FUNC_CALL)
            return _inter.func->output;
        if (_inter.id == VARIABLE_ACCESS)
            return _inter.var->type;

    }

    // Can turn "type_a" into "type_b"
    // 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
    u8 can_be_converted_to(u8 type_a, u8 type_b)
    {
        // If one of these types is signed and the other isn't or "type_b" has more bits than "type_a"
        if (type_a & 1 != type_a & 1 || type_a > type_b)
            return 1;
    }
}
