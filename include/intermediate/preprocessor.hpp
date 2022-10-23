/* This file holds a lot of preprocessor esk things. */

#ifndef INTERMEDIATE_PREPROCESSOR_HPP
#define INTERMEDIATE_PREPROCESSOR_HPP

#include<intermediate/intermediate.hpp>
#include<string>
#include<vector>
#include<iostream>

namespace preprocessor 
{
    // This is called by "intermediate.cpp" and goes over comments
    bool try_to_pass_comments(std::vector<std::string> file, std::vector<std::string>::iterator &itr);

    // This is called once from intermediate.cpp so it's inlined
    void process_instruction(std::vector<std::string> file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output);
}

#endif