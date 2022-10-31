/* This file handles things that happen during intermediate to asm */

#include<intermediate/postprocessor.hpp>
#include<intermediate/intermediate.hpp>
#include<iostream>
#include<error.hpp>

// TODO: This should change based on the compilation target

namespace postprocessor
{
    // This is called by "intermediate.cpp" if it hits a '#'
    void process_instruction(std::vector<std::string> &file, std::vector<std::string>::iterator &itr, std::vector<inter> &inter_output)
    {
        // If we are reading asm
        if (*(itr+1) == "asm")
        {
            std::string output_asm;

            while (true)
            {
                itr++;
                if (itr == file.end() || itr+1 == file.end() || itr+2 == file.end()) 
                    error::send_error("Expected to find \"#asm_end\" before the end of the file.\n";

                // If we hit another postprocessor instruction
                if (*itr == "#") 
                {
                    // If the post processor instruction is "#!asm" we return
                    if (*(itr+1) == "!" && (*(itr+2)) == "asm")
                        return;

                    // If we have another post processor instruction we send an error
                    error::send_error("Found unexpected post processor instruction while parsing #asm.\n");
                }

                // If we hit a semicolon we add a new line
                if (*itr == ";")
                {
                    output_asm.append("\n");
                    continue;
                }

                // If it isn't a postprocessor instruction or a semicolon we add it to the output and continue
                output_asm.append(*itr + " ");
            }

            // We incrament the iterator twice to go over "!asm"
            itr += 2;

            // We save the outputed asm into the "refrenced_name" of an "ASM" intermediate
            postprocessor_add_inter((inter(ASM, 0, output_asm)));
        }
    }

}