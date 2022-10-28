#ifndef AARCH32_ASM_HPP
#define AARCH32_ASM_HPP

#include<vector>
#include<string>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>

namespace aarch32_asm 
{
    // TODO: This should contain the structs that are used by "aarch32_asm.cpp"
    std::vector<std::string> intermediates_into_asm(std::vector<inter> &inter_file);
}

#endif