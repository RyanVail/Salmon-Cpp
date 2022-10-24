/* This file handles things that happen during intermediate to asm */

#include<postprocessor.hpp>
#include<aarch32_asm.hpp>

// TODO: This should change based on the compilation target

namespace postprocessor
{
    // This is called by "intermediate.cpp" if it hits a '#'
    void process_instruction(std::vector<std::string> file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output, value_defintion )
    {
        // If we are reading asm
        if (*(itr+1) == "asm")
        {
            std::string output_asm;
            while (1)
            {
                itr++;
                if (itr == file.end() || itr+1 == file.end() || itr+2 == file.end()) { std::cout << "Expected to find \"#asm_end\" before the end of the file.\n"; exit(-1); }
                // If we hit another postprocessor instruction
                if (*itr == "#") 
                {
                    // If the post processor instruction is "#!asm" we return
                    if (*(itr+1) == "!" && (*(itr+2)) == "asm") { return; }
                    // If we have another post processor instruction we send an error
                    std::cout << "Found unexpected post processor instruction while parsing #asm.\n"; 
                    exit(-1);
                }
                // If we hit a semicolon we add a new line
                if (*itr == ";")
                {
                    output_asm.append("\n")
                    continue;
                }
                // If it isn't a postprocessor instruction or a semicolon we add it to the output and continue
                output_asm.append(*itr + " ");
            }
            // We incrament the iterator twice to go over "!asm"
            itr += 2;
            // We save the outputed asm into the "refrenced_name" of an "ASM" intermediate
            inter_output.push_back(inter(ASM, 0, output_asm));
        }
    }

}