/* This file holds a lot of preprocessor esk things. */

#pragma once
#include<string>
#include<vector>
#include<iostream>
#include"intermediate.hpp"

namespace preprocessor 
{
    // This is called by "intermediate.cpp" and goes over comments
    void try_to_pass_comments(std::vector<std::string> file, std::vector<std::string>::iterator &itr)
    {
        unsigned int nested = 0;
        if (*itr == "/" && itr+1 != file.end() && *(itr+1) == "*")
		{
			while (1)
			{
                itr++;
                if (*itr == "/" && itr+1 != file.end() && *(itr+1) == "*") { nested++; }
				if (itr == file.end() || itr+1 == file.end()) { std::cout << "Expected to find */.\n"; exit(-1); }
				if (*itr == "*" && *(itr+1) == "/") 
                {
                    if (!nested) { itr++; break; }
                    nested--;
                }
			}
            itr++;
		}
    }
    
    // This is called once from intermediate.cpp so it's inlined
    inline std::string process_instruction(std::vector<std::string> file, std::string *itr, std::vector<inter> &inter_output)
    {
		std::string output_asm;
		while (1)
		{
            // TODO: This should skip comments
            itr++;
            std::cout << "processing instruction: " << *itr << "\n";
			if (*itr == "#") 
            { 
                if (*(itr+1) == "asm_end") { break; }
                break; 
            }
			output_asm.append(*itr);
		}
		itr++;
		inter_output.push_back(inter(ASM, 0, output_asm));
		std::cout << "asm: " << output_asm << "\n";
		std::cout << *itr << " <- Next token\n"; // TODO: Remove this test line
    }
}