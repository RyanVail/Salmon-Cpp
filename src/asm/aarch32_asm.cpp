 /* This file turns the intermediate repersentation into aarch32 asm */

#include<asm/aarch32_asm.hpp>
#include<stack>
#include<vector>
#include<intermediate/intermediate.hpp>
#include<symbol_table.hpp>
#include<asm/value_asm.hpp>

// TODO: Branchpoints should be caps

namespace aarch32_asm 
{
    // Operate on 0 registers
    // BREAK
    // CONTINUE
    // QUIT
    // Operate on only R0
    #define NOT_ASM_BOOL "EOR R0, R0"
    #define NOT_ASM_INT "CLZ R0, R0\nLSR R0, #5"
    #define GET_ASM_NORMAL "LDR R0, [R0]"
    #define GET_ASM_CHAR "LDRB R0, [R0]"
    #define INCRAMENT_ASM_NORMAL "ADD R0, #1"
    #define DECRAMENT_ASM_NORMAL "SUB R0, #1"
    // IF_BEGIN
    // WHILE_BEGIN
    #define RETURN_ASM_NORMAL "BX LR"
    // Operate on R0 & R1
    #define LESS_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1"
    #define LESS_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1"
    #define GREATER_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1"
    #define GREATER_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1"
    #define EQUAL_ASM_NORMAL "CMP R0, R1\nMOVEQ R0, #1\nMOVNE R0, #0"
    #define ADD_ASM_NORMAL "ADD R0, R1"
    #define	SUB_ASM_NORMAL "SUB R0, R1"
    #define MUL_ASM_NORMAL "MUL R0, R1"
    #define AND_ASM_NORMAL "AND R0, R1"
    #define OR_ASM_NORMAL "ORR R0, R1"
    #define XOR_ASM_NORMAL "EOR R0, R1"
    #define LSL_ASM_NORMAL "LSL R0, R1"
    #define LSR_ASM_NORMAL "LSR R0, R1"
    #define RESET_RPN_ASM_NORMAL "MOV R0, #0\nMOV R1, R0"

    struct statment_defintion
    {
        unsigned char type = 255;
        std::string name = "";
        statment_defintion(unsigned char _type, std::string _name) : type(_type), name(_name) {};
    };

    value_asm::value_defintion value_in_r0 = value_asm::value_defintion(0, 0, 0, 0);

    // This either adds asm to "asm_file" or the top function in "asm_functions" based on "in_func"
    void add_asm(bool in_func, std::string asm_to_add, std::vector<std::string> &asm_file, std::vector<std::string> &asm_functions)
    {
        if (in_func) { asm_functions[asm_functions.size()-1] += asm_to_add; }
        else { asm_file.push_back(asm_to_add); }
    }

    // This turns a value defintion into its asm
    std::string value_into_asm(value_asm::value_defintion &current_def, int _register)
    {
        // If we are defining a const
        if (current_def.accessed_variable == nullptr && current_def.called_function == nullptr)
        {
            // TODO: This should make sure the const is a valid const in arm asm
            return "MOV R" +
                    std::to_string(_register) +
                    ", #" +
                    std::to_string(*(&(current_def.const_value))); // TODO: Find a better way of making the red squigglies go away
        }
        // If we are accessing a variable
        if (current_def.accessed_variable != 0)
        {
            return "LDR R" +
                    std::to_string(_register) +
                    ", [SP, #" +
                    std::to_string(current_def.accessed_variable->stack_location+types_size[current_def.accessed_variable->type]) +
                    "]";
        }
        // If we are calling a function
        if (current_def.called_function != 0)
        {
            return "BL" +
                    current_def.called_function->name;
        }
        return "";
    }

    // This function does a lot of the repetive asm from intermediate
    void single_intermediate_into_asm(inter current_inter, std::stack<value_asm::value_defintion> &rpn_stack, std::stack<statment_defintion> &statment_stack, bool in_func, std::vector<std::string> &asm_functions, std::vector<std::string> &asm_file)
    {
        unsigned char current_inter_id = current_inter.id;
        if (current_inter_id == ASM)
        {
            add_asm(in_func, current_inter.refrenced_name, asm_file, asm_functions);
        }
        bool was_for_one = false; // Hopefully this saves a bit of time
        if (current_inter_id >= NOT && current_inter_id <= DECRAMENT) 
        {
            // If R0 isn't used
            if (!value_in_r0.final_type) 
            { 
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                value_in_r0 = rpn_stack.top();
                rpn_stack.pop();
            }
            was_for_one = true;
        }
        // If we are doing a single computation on a byte type
        //if (is_char(rpn_stack))
        if (0)
        {
            if (current_inter_id == GET) { add_asm(in_func, GET_ASM_CHAR, asm_file, asm_functions); }
        }
        //if (is_normal(rpn_stack) && was_for_one)
        if (1 && was_for_one)
        {
            if (current_inter_id == NOT) { add_asm(in_func, NOT_ASM_INT, asm_file, asm_functions); }
            if (current_inter_id == GET) { add_asm(in_func, GET_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == MEM_ADDRS)
            {
                // TODO: This only works for variable that are on the stack
                add_asm(in_func, 
                            "ADD R0,SP,#" +
                            get_variable_token(current_inter.refrenced_name)->stack_location
                        , asm_file, asm_functions);
            }
            if (current_inter_id == INCRAMENT) { add_asm(in_func, INCRAMENT_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == DECRAMENT) { add_asm(in_func, DECRAMENT_ASM_NORMAL, asm_file, asm_functions); }
            return;
        }
        if (current_inter_id == EQUAL)
        {
            // If there is nothing in R0 we offload the top value
            if (!value_in_r0.final_type) 
            {
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                rpn_stack.pop();
            }
            // If the variable is 32 bit
            if (is_normal(rpn_stack.top().final_type))
            {
                add_asm(in_func, 
                            "STR R0, [SP,#" +
                            std::to_string(rpn_stack.top().accessed_variable->stack_location+types_size[rpn_stack.top().final_type]) +
                            "]" +
                            std::to_string(get_function_token(current_inter.refrenced_name)->stack_space_needed)
                        , asm_file, asm_functions);
            }
            // If variable is 8 bit
            else if (is_char(rpn_stack.top().final_type))
            {
                add_asm(in_func, 
                            "STRB R0, [SP,#" +
                            std::to_string(rpn_stack.top().accessed_variable->stack_location+types_size[rpn_stack.top().final_type]) +
                            "]" +
                            std::to_string(get_function_token(current_inter.refrenced_name)->stack_space_needed)
                        , asm_file, asm_functions);
            }
            else { std::cout << "Error while defining a variable's value.\n"; exit(-1); }
            rpn_stack.pop();
            value_in_r0 = value_asm::value_defintion(0, 0, 0, 0); // We set R0 to nothing
            return;
        }
        // Prepping for a comparision with two types
        if (current_inter_id >= AND && current_inter_id <= MUL)
        {
            // If R0 isn't being used
            if (!value_in_r0.final_type)
            {
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                value_in_r0 = rpn_stack.top();
                rpn_stack.pop(); 
            }
            // We off load the next in the stack into R1
            add_asm(in_func, value_into_asm(rpn_stack.top(), 1), asm_file, asm_functions);
            rpn_stack.pop();
        }
        // These are double comparisons with two normal types
        // if (is_normal(rpn_stack) && is_normal(rpn_stack-1))
        if (1)
        {
            if (current_inter_id == LESS_EQUAL) { add_asm(in_func, LESS_EQUAL_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == LESS) { add_asm(in_func, LESS_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == GREATER) { add_asm(in_func, GREATER_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == GREATER_EQUAL) { add_asm(in_func, GREATER_EQUAL_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == IS_EQUAL) { add_asm(in_func, EQUAL_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == AND) { add_asm(in_func, AND_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == OR) { add_asm(in_func, OR_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == XOR) { add_asm(in_func, XOR_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == LSL) { add_asm(in_func, LSL_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == LSR) { add_asm(in_func, LSR_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == ADD) { add_asm(in_func, ADD_ASM_NORMAL, asm_file, asm_functions); }
            if (current_inter_id == SUB) { add_asm(in_func, SUB_ASM_NORMAL, asm_file, asm_functions); }
            // if (current_inter_id == DIV) { add_asm(EOR_ASM_NORMAL); } // TODO: Add division for arrch32
            if (current_inter_id == MUL) { add_asm(in_func, MUL_ASM_NORMAL, asm_file, asm_functions); }
        }
    }

    // This turns a intermediate repersentation of a file into its ASM
    std::vector<std::string> intermediates_into_asm(std::vector<inter> &inter_file)
    {
        std::vector<std::string> asm_functions; // This is the asm of functions that gets added to the end of "asm_file"
        // std::vector<std::string> asm_data;
        // ^^^ Currently not used
        std::vector<std::string> asm_file = { ".ASM\n.GLOBAL fn_main\n.TEXT\nfn_end:\nMOV R7, #1\nSWI 0\nfn_main:\nSUB SP, #" + std::to_string(get_stack_space_needed(-1)) };
        std::stack<value_asm::value_defintion> rpn_stack;
        std::stack<statment_defintion> statment_stack; // 0 -> function call, 1 -> while, 2 -> if, 3 -> return, 4 -> break, 5-> continue
        unsigned int current_statment_id = 0; // This is just a little bit of padding added to the end of branch points to make sure they are seperate                          ^^^^^^^^^^^^^
        bool in_func = 0; // If we are inside a function ie. if we are adding the asm to asm_file or asm_functions
        unsigned char current_inter_id; // The current intermediate's id hopefully in a register
        for (inter current_inter : inter_file)
        {
            // TODO: This should track owner and it should only get variables that are local to that owner (and owenr)
            current_inter_id = current_inter.id;
            single_intermediate_into_asm(current_inter, rpn_stack, statment_stack, in_func, asm_functions, asm_file);
            if (current_inter_id == VARIABLE_DECLERATION)
            {
                // TODO: Make variable declerations work
                rpn_stack.push(value_asm::value_defintion(1, current_inter.refrenced_variable_token, 0, 0));
            }
            if (current_inter_id == CONST) { rpn_stack.push(value_asm::value_defintion(current_inter.value, 0, 0, 0)); }
            if (current_inter_id == VARIABLE_ACCESS) 
            {
                // TODO: This should go back and find the "VARIABLE_TYPE" token since it isn't set in "intermediate.cpp" or do it in "intermediate.cpp"
                rpn_stack.push(value_asm::value_defintion(0, current_inter.refrenced_variable_token, 0, get_variable_token(current_inter.refrenced_name)->type));
            }
            if (current_inter_id == FUNC_BEGIN) 
            {
                if (!statment_stack.empty() || in_func) { std::cout << "Functions can only be defined in the global scope.\n"; exit(-1); }
                if (!rpn_stack.empty()) { std::cout << "RPN stack should be empty before defining a function.\n"; exit(-1); }
                in_func = true;
                statment_stack.push(statment_defintion(0, "fn_" + current_inter.refrenced_name));
                asm_functions.push_back("");
                add_asm(in_func, 
                            "fn_" + 
                            current_inter.refrenced_name + 
                            ":\n" + 
                            "SUB SP, #" + 
                            std::to_string(get_function_token(current_inter.refrenced_name)->stack_space_needed)
                        , asm_file, asm_functions);
                int current_register = 0;
                for (variable_token &current_variable : get_function_token(current_inter.refrenced_name)->inputs)
                {
                    // If this is 32 bit
                    if (is_normal(current_variable.type))
                    {
                        add_asm(in_func, 
                                    "STR R" +
                                    std::to_string(current_register) +
                                    ", [SP,#" +
                                    std::to_string(current_variable.stack_location+types_size[current_variable.type]) +
                                    "]" +
                                    std::to_string(get_function_token(current_inter.refrenced_name)->stack_space_needed)
                                , asm_file, asm_functions);
                    }
                    // If this is 8 bit
                    else if (is_char(current_variable.type))
                    {
                        add_asm(in_func, 
                                    "STRB R" +
                                    std::to_string(current_register) +
                                    ", [SP,#" +
                                    std::to_string(current_variable.stack_location+types_size[current_variable.type]) +
                                    "]" +
                                    std::to_string(get_function_token(current_inter.refrenced_name)->stack_space_needed)
                                , asm_file, asm_functions);
                    }
                    //else { std::cout << "Error while offloading function inputs into the stack.\n"; exit(-1); }
                }
            }
            if (current_inter_id == WHILE_BEGIN)
            {
                if (rpn_stack.empty() && !value_in_r0.final_type) { std::cout << "Warning: Something should be on the RPN stack before starting a while statment.\nContinuing...\n"; break; }
                else 
                {
                    if (!value_in_r0.final_type) // If R0 isn't being used already we off load the value on the stack into R0
                    {
                        add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                        rpn_stack.pop();
                    }
                    value_in_r0 = value_asm::value_defintion(0, 0, 0, 0);
                }
                statment_stack.push(statment_defintion(1, std::to_string(current_statment_id)));
                add_asm(in_func, 
                            "while_start_" + 
                            std::to_string(current_statment_id) + 
                            ":\nCMP R0, #0\nBLE while_end_" + 
                            std::to_string(current_statment_id)
                        , asm_file, asm_functions);
                current_statment_id++;
            }
            if (current_inter_id == IF_BEGIN)
            {
                if (rpn_stack.empty() && !value_in_r0.final_type) { std::cout << "Warning: Something should be on the RPN stack before starting an if statment.\nContinuing...\n"; break; }
                else 
                {
                    if (!value_in_r0.final_type) // If R0 isn't being used already we off load the value on the stack into R0
                    {
                        add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                        rpn_stack.pop();
                    }
                    value_in_r0 = value_asm::value_defintion(0, 0, 0, 0);
                }
                add_asm(in_func,
                            "CMP R0, #0\nBLE if_" + 
                            std::to_string(current_statment_id)
                        , asm_file, asm_functions);
                statment_stack.push(statment_defintion(1, "if_" + std::to_string(current_statment_id) + ":"));
                current_statment_id++;
            }
            if (current_inter_id == RESET_RPN) 
            {
                rpn_stack = {};
                // TODO: This should scale based on the # of registers used
                add_asm(in_func, RESET_RPN_ASM_NORMAL, asm_file, asm_functions);
                value_in_r0 = value_asm::value_defintion(0, 0, 0, 0);
            }
            if (current_inter_id == IF_END)
            {
                add_asm(in_func, statment_stack.top().name, asm_file, asm_functions);
                statment_stack.pop();
            }
            if (current_inter_id == WHILE_END)
            {
                // This makes sure we have thing on the stack before the end of a while statment
                if (rpn_stack.empty() && !value_in_r0.final_type) { std::cout << "Warning: Something should be on the RPN stack before ending a while statment.\nContinuing...\n"; break; }
                if (!value_in_r0.final_type) { add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions); }
                add_asm(in_func, 
                            "B while_start_" + 
                            statment_stack.top().name + 
                            "\nwhile_end_" + 
                            statment_stack.top().name + 
                            ":"
                        , asm_file, asm_functions);
                statment_stack.pop();
            }
            if (current_inter_id == FUNC_END)
            {
                // TODO: Add some more things like adding back SP
                add_asm(in_func, 
                            "BX LR"
                        , asm_file, asm_functions);
                statment_stack.pop();
                in_func = false;
            }
            // This handles loading all the values off the stack and calling functions
            if (current_inter_id == FUNC_CALL) 
            { 
                function_token* current_function = get_function_token(current_inter.refrenced_name);
                if (!current_function) { std::cout << "Known function: " << current_inter.refrenced_name << "\n"; exit(-1); }
                if (current_function->inputs.size() > 8) { std::cout << "Error: The function " << current_inter.refrenced_name << " takes more than eight values which is not valid.\n"; exit(-1); }
                int current_register = 0; // The current register we are offloading the inputs of the function into
                // This offloads the needed values as input from the stack into the registers 
                for (variable_token current_variable : current_function->inputs)
                {
                    if (rpn_stack.empty() && !value_in_r0.final_type) { std::cout << "Expected something to be one the stack while calling function: " << current_inter.refrenced_name << "\n"; exit(-1); }
                    // If there is something in R0 right now
                    if (value_in_r0.final_type) 
                    {
                        // If we can't transform R0 into the function input type
                        if (can_be_transformed_into(value_in_r0.final_type, current_variable.type))
                        {
                            std::cout << "Cannot ";
                            if (can_be_transformed_into(value_in_r0.final_type, current_variable.type) == 0) { std::cout << "explicitly "; }
                            std::cout << "transform type " << id_into_string(rpn_stack.top().final_type) << " into " << id_into_string(current_variable.type);
                            exit(-1);
                        }
                        current_register++;
                        continue; // If we can transform the value in R0 into the input type we just continue and incrament "current_register"
                    }
                    // If the current thing on the stack can be transformed into the input type
                    if (!can_be_transformed_into(rpn_stack.top().final_type, current_variable.type))
                    {
                        add_asm(in_func, value_into_asm(rpn_stack.top(), current_register), asm_file, asm_functions);
                        rpn_stack.pop();
                        current_register++;
                    }
                    else 
                    {
                        std::cout << "Cannot ";
                        if (can_be_transformed_into(rpn_stack.top().final_type, current_variable.type) == 0) { std::cout << "explicitly "; }
                        std::cout << "transform type " << id_into_string(rpn_stack.top().final_type) << " into " << id_into_string(current_variable.type);
                        exit(-1);
                    }
                }
                // This actually calls the function
                add_asm(in_func,
                            "BL func_" +
                            current_inter.refrenced_name
                        , asm_file, asm_functions);
            }
        }
        // This puts "asm_file" and "asm_functions" together
        asm_file.insert(asm_file.end(), asm_functions.begin(), asm_functions.end());
        return asm_file;
    }
}
#undef RETURN_ASM_NORMAL
#undef NOT_ASM_BOOL
#undef NOT_ASM_INT
#undef GET_ASM_NORMAL
#undef GET_ASM_CHAR
#undef INCRAMENT_ASM_NORMAL
#undef DECRAMENT_ASM_NORMAL
#undef LESS_EQUAL_ASM_NORMAL
#undef LESS_ASM_NORMAL
#undef GREATER_ASM_NORMAL
#undef GREATER_EQUAL_ASM_NORMAL
#undef EQUAL_ASM_NORMAL
#undef ADD_ASM_NORMAL
#undef SUB_ASM_NORMAL
#undef MUL_ASM_NORMAL
#undef AND_ASM_NORMAL
#undef OR_ASM_NORMAL 
#undef XOR_ASM_NORMAL
#undef LSL_ASM_NORMAL
#undef LSR_ASM_NORMAL
#undef RESET_RPN_ASM_NORMAL
