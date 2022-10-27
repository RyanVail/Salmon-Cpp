/* This does type checking during the tokens to intermediate phase */

#include<vartypes.hpp>
#include<iostream>

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
    // Can turn "type_a" into "type_b"
    // 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
    unsigned char can_be_converted_to(unsigned char type_a, unsigned char type_b)
    {
        // If one of these types is signed and the other isn't or "type_b" has more bits than "type_a"
        if (type_a & 1 != type_a & 1 || type_a > type_b)
            return 1;
    }
}