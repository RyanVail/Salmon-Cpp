/* This file handles things that happen during intermediate to asm */

#include<postprocessor.hpp>
#include<aarch32_asm.hpp>

// TODO: This should change based on the compilation target

namespace postprocessor
{
    void process_instruction(std::vector<std::string> file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output, value_defintion )
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