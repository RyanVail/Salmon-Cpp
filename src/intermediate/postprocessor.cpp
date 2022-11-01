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
        if (*(itr+1) == "extern")
        {
            // ==== Name and id ====
            u8 _return_type = into_id(*(itr+2));
            itr++;
            std::string _name = (*itr).substr(0, (*itr).find('(') );

            // ==== Checks for errors ====
            if (!_return_type)
                error::send_error("The external function's return type: " + _return_type + " isn't valid.\n");
            if(!is_str_letters(_name)) 
                error::send_error("The external function name: " + _name + " contains invalid characters.\n");
            if(get_function_token(_name)) 
                error::send_error("The external function name: " + _name + " is already used.\n");
            if(!is_valid_name(_name))
                error::send_error("The external function name: " + _name + " isn't valid.\n");

            // The function inputs are variables with just types
            std::vector<inter> _inputs;

            // ==== Gets function inputs ====
            while (true)
            {
                if (itr == file.end())
                    error::send_error("Error while parsing function defintion: reached end of file.\n");
                std::string token = *itr;
                if (token == ",") 
                { 
                    itr++;
                    continue;
                }
                if (token == ")")
                    break;
                else
                    // This adds the current type into the input of the function
                    if (!into_id(token))
                        error::send_error("Unkown type in external function: " + token + ".\n");
                    _inputs.push_back(variable_token(into_id(token)));
                itr++;
            }
            function_token _func;
            _func.inputs = _inputs;
            _func.name = _name;
            add_function_token(_func);
            itr += 1;
        }
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

            // TODO: We need to store the outputed asm somewhere
            // postprocessor_add_inter((inter(ASM, 0, output_asm)));
        }
    }

}