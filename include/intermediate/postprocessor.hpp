/* This file handles things that happen during intermediate to asm */

#include<string>
#include<vector>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>

namespace postprocessor
{
    // This is called once from intermediate.cpp so it's inlined
    void process_instruction(std::string *itr, std::vector<inter> &inter_output);
}