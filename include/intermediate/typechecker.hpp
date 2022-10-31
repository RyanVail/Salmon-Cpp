/* This does type checking during the tokens to intermediate phase */

#ifndef TYPECHECKER_HPP
#define TYPECHECKER_HPP

#include<intermediate/intermediate.hpp>

namespace typechecker
{
    // This will send an error if an operation cannot be done
    void operation_can_be_done();

    // TODO: This should use "get_local_variable_token"
    u8 get_type_of_inter(inter _inter);

    // Can turn "type_a" into "type_b"
    // 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
    u8 can_be_converted_to(u8 type_a, u8 type_b);
}

#endif