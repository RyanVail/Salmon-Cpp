#ifndef AARCH32_ASM_HPP
#define AARCH32_ASM_HPP

#include<vector>
#include<string>
#include<intermediate/intermediate.hpp>
namespace aarch32_asm 
{
    std::vector<std::string> intermediates_into_asm(std::vector<inter> &inter_file);
}

#endif