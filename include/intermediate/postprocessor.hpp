/* This file handles things that happen during intermediate to asm */

#include<string>
#include<vector>
#include<intermediate/intermediate.hpp>

namespace postprocessor
{
    // This is called once from intermediate.cpp so it's inlined
    void process_instruction(std::vector<std::string> file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output);
}