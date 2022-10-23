/* This file holds a lot of preprocessor esk things. */

#include<intermediate/preprocessor.hpp>
#include<intermediate/intermediate.hpp>
#include<string>
#include<vector>
#include<iostream>

namespace preprocessor 
{
    // This is called by "intermediate.cpp" and goes over comments
    bool try_to_pass_comments(std::vector<std::string> file, std::vector<std::string>::iterator &itr)
    {
        unsigned int nested = 0; // TODO: Fix nested comments
        if (*itr == "/" && itr+1 <= file.end() && *(itr+1) == "*")
		{
			while (1)
			{
                itr++;
                if (itr+1 >= file.end()) { std::cout << "Expected to find \"*/\".\n"; exit(-1); }
                if (*itr == "/" && *(itr+1) == "*") { nested++; }
				if (*itr == "*" && *(itr+1) == "/") 
                {
                    if (!nested) { itr++; return true; }
                    nested--;
                }
			}
		}
        return false;
    }
    
    void process_instruction(std::vector<std::string> file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output)
    {
        if (*(itr+1) == "asm")
        {
            itr++;
            std::string output_asm;
            while (1)
            {
                // TODO: This should skip comments
                itr++;
                if (itr == file.end() || itr+1 == file.end() || itr+2 == file.end()) { std::cout << "Expected to find \"#asm_end\" before the end of the file.\n"; exit(-1); }
                if (*itr == "#") 
                {
                    if (*(itr+1) == "!" && (*(itr+2)) == "asm") { break; }
                }
                output_asm.append(*itr + " ");
            }
            itr += 2;
            inter_output.push_back(inter(ASM, 0, output_asm));
        }
    }
}