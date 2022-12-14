/* This file turns the intermediate repersentation into aarch32 asm */

#include<asm/aarch32_asm.hpp>
#include<stack>
#include<vector>
#include<intermediate/intermediate.hpp>
#include<symboltable.hpp>
#include<operanddefinition.hpp>
#include<error.hpp>

// TODO: Branchpoints should be caps
// Current name mangaling is just terrible and is not needed in the first place.

namespace aarch32_asm
{
    // Operate on 0 registers
    // BREAK
    // CONTINUE
    // QUIT
    // Operate on only R0
    #define NOT_ASM_BOOL "EOR R0, R0"
    #define NOT_ASM_INT "CLZ R0, R0\nLSR R0, #5"
    #define GET_ASM_NORMAL "LDR R0,[R0]"
    #define GET_ASM_CHAR "LDRB R0,[R0]"
    #define INCRAMENT_ASM_NORMAL "ADD R0, #1"
    #define DECRAMENT_ASM_NORMAL "SUB R0, #1"
    // IF_BEGIN
    // WHILE_BEGIN
    // Operate on R0 & R1
    #define LESS_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1"
    #define LESS_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1"
    #define GREATER_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1"
    #define GREATER_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1"
    #define EQUAL_ASM_NORMAL "CMP R0, R1\nMOVEQ R0, #1\nMOVNE R0, #0"
    #define ADD_ASM_NORMAL "ADD R0, R1"
    #define SUB_ASM_NORMAL "SUB R0, R1"
    #define MUL_ASM_NORMAL "MUL R0, R1"
    #define AND_ASM_NORMAL "AND R0, R1"
    #define OR_ASM_NORMAL "ORR R0, R1"
    #define XOR_ASM_NORMAL "EOR R0, R1"
    #define LSL_ASM_NORMAL "LSL R0, R1"
    #define LSR_ASM_NORMAL "LSR R0, R1"
    #define RESET_RPN_ASM_NORMAL "MOV R0, #0\nMOV R1, R0"

    struct statment_defintion
    {
        u8 type = 255;
        std::string name = "";
        statment_defintion(u8 _type, std::string _name) : type(_type), name(_name) {};
    };

    operand::operand_def value_in_r0 = operand::operand_def(0, 0, 0, 0);

    // This either adds asm to "asm_file" or the top function in "asm_functions" based on "in_func"
    void add_asm(bool in_func, std::string asm_to_add, std::vector<std::string> &asm_file, std::vector<std::string> &asm_functions)
    {
        if (in_func)
            asm_functions.push_back(asm_to_add);
        else
            asm_file.push_back(asm_to_add);
    }

    // This turns a value defintion into its asm
    std::string value_into_asm(operand::operand_def &current_def, i32 _register)
    {
        // If we are defining a const
        if (current_def.accessed_variable == nullptr && current_def.called_function == nullptr)
            // TODO: This should make sure the const is a valid const in arm asm
            return "MOV R" +
                   std::to_string(_register) +
                   ", #" +
                   std::to_string(*(&(current_def.const_value))); // TODO: Find a better way of making the red squigglies go away

        // If we are accessing a variable
        if (current_def.accessed_variable != 0)
            return "LDR R" +
                   std::to_string(_register) +
                   ", [SP,#" +
                   std::to_string(current_def.accessed_variable->stack_location + types_size[current_def.accessed_variable->type]) +
                   "]";

        // If we are calling a function
        if (current_def.called_function != 0)
            return "BL" +
                   current_def.called_function->name;

        // This is just a fail safe
        return "";
    }

    // This function does a lot of the repetive asm from intermediate
    void single_intermediate_into_asm(inter current_inter, std::stack<operand::operand_def> &rpn_stack, std::stack<statment_defintion> &statment_stack, bool in_func, std::vector<std::string> &asm_functions, std::vector<std::string> &asm_file)
    {
        // This is done in the hopes it is stored in a register but I might be useless
        unsigned char current_inter_id = current_inter.id;

        // TODO: GET THIS ASM WORKING!
        //if (current_inter_id == ASM)
        //    add_asm(in_func, current_inter.refrenced_name, asm_file, asm_functions);

        bool was_for_one = false;

        // If we are doing a computation on a single variable
        if (current_inter_id >= NOT && current_inter_id <= DECRAMENT)
        {
            // If R0 isn't currently being used
            if (!value_in_r0.final_type)
            {
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                value_in_r0 = rpn_stack.top();
                rpn_stack.pop();
            }
            was_for_one = true;
        }

        // If we are doing a single computation on a byte type
        // if (is_char(rpn_stack))
        if (0)
            if (current_inter_id == GET)
                add_asm(in_func, GET_ASM_CHAR, asm_file, asm_functions);

        // if (is_normal(rpn_stack) && was_for_one)
        if (1 && was_for_one)
        {
            switch (current_inter_id)
            {
            case NOT:
                add_asm(in_func, NOT_ASM_INT, asm_file, asm_functions);
                break;
            case GET:
                add_asm(in_func, GET_ASM_NORMAL, asm_file, asm_functions);
                break;
            // TODO: This should do other things for variables that aren't on the stack
            case MEM_ADDRS:
                add_asm(in_func,
                        "ADD R0,SP,#" +
                            current_inter.get_var()->stack_location,
                        asm_file, asm_functions);
                break;
            case INCRAMENT:
                add_asm(in_func, INCRAMENT_ASM_NORMAL, asm_file, asm_functions);
                break;
            case DECRAMENT:
                add_asm(in_func, DECRAMENT_ASM_NORMAL, asm_file, asm_functions);
                break;
            }

            return;
        }

        // If we are seting a variable equal to some value
        // TODO: This should allow setting a memory location equal to a value
        if (current_inter_id == EQUAL)
        {
            // If there is nothing in R0 we offload the top value
            if (!value_in_r0.final_type)
            {
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                rpn_stack.pop();
            }

            // If the variable is 32 bit
            if (is_normal(rpn_stack.top().accessed_variable->type))
                add_asm(in_func,
                        "STR R0, [SP,#" +
                            std::to_string(rpn_stack.top().accessed_variable->stack_location + types_size[rpn_stack.top().accessed_variable->type]) +
                            "]",
                        asm_file, asm_functions);

            // If variable is 8 bit
            else if (is_char(rpn_stack.top().accessed_variable->type))
                add_asm(in_func,
                        "STRB R0, [SP,#" +
                            std::to_string(rpn_stack.top().accessed_variable->stack_location + types_size[rpn_stack.top().accessed_variable->type]) +
                            "]",
                        asm_file, asm_functions);

            else
            {
                std::cout << rpn_stack.top().accessed_variable->type << "\n";
                error::send_error("Error while defining a variable's value.\n");
            }

            rpn_stack.pop();
            value_in_r0 = operand::operand_def(0, 0, 0, 0);
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
            switch (current_inter_id)
            {
            case LESS_EQUAL:
                add_asm(in_func, LESS_EQUAL_ASM_NORMAL, asm_file, asm_functions);
                break;
            case LESS:
                add_asm(in_func, LESS_ASM_NORMAL, asm_file, asm_functions);
                break;
            case GREATER:
                add_asm(in_func, GREATER_ASM_NORMAL, asm_file, asm_functions);
                break;
            case GREATER_EQUAL:
                add_asm(in_func, GREATER_EQUAL_ASM_NORMAL, asm_file, asm_functions);
                break;
            case IS_EQUAL:
                add_asm(in_func, EQUAL_ASM_NORMAL, asm_file, asm_functions);
                break;
            case AND:
                add_asm(in_func, AND_ASM_NORMAL, asm_file, asm_functions);
                break;
            case OR:
                add_asm(in_func, OR_ASM_NORMAL, asm_file, asm_functions);
                break;
            case XOR:
                add_asm(in_func, XOR_ASM_NORMAL, asm_file, asm_functions);
                break;
            case LSL:
                add_asm(in_func, LSL_ASM_NORMAL, asm_file, asm_functions);
                break;
            case LSR:
                add_asm(in_func, LSR_ASM_NORMAL, asm_file, asm_functions);
                break;
            case ADD: 
                add_asm(in_func, ADD_ASM_NORMAL, asm_file, asm_functions);
                break;
            case SUB:
                add_asm(in_func, SUB_ASM_NORMAL, asm_file, asm_functions);
                break;
            // if (current_inter_id == DIV) { add_asm(EOR_ASM_NORMAL); } // TODO: Add division for arrch32
            case MUL:
                add_asm(in_func, MUL_ASM_NORMAL, asm_file, asm_functions);
                break;
            }
        }
    }

    // This turns a intermediate repersentation of a file into its ASM
    std::vector<std::string> intermediates_into_asm(std::vector<inter> &inter_file)
    {
        std::vector<std::string> asm_functions; // This is the asm of functions that gets added to the end of "asm_file"
        // std::vector<std::string> asm_data;
        // ^^^ Currently not used
        // TODO: These variables should all be static to make inline function calls simpler
        std::vector<std::string> asm_file = {".ASM\n.GLOBAL fn_main\n.TEXT\nfn_end:\nMOV R7, #1\nSWI 0\nfn_main:\nSUB SP,#" + std::to_string(get_stack_space_needed(-1))};
        std::stack<operand::operand_def> rpn_stack;
        std::stack<statment_defintion> statment_stack; // 0 -> function call, 1 -> while, 2 -> if, 3 -> return, 4 -> break, 5-> continue
        u32 current_statment_id = 0;          // This is just a little bit of padding added to the end of branch points to make sure they are seperate                          ^^^^^^^^^^^^^
        bool in_func = 0;                              // If we are inside a function ie. if we are adding the asm to asm_file or asm_functions
        u8 current_inter_id;                // The current intermediate's id hopefully in a register
        for (inter current_inter : inter_file)
        {
            // TODO: This should track owner and it should only get variables that are local to that owner (and owenr)
            current_inter_id = current_inter.id;
            single_intermediate_into_asm(current_inter, rpn_stack, statment_stack, in_func, asm_functions, asm_file);
            
            // Some values that are used in the switch statment
            i32 current_register = 0;
            function_token *current_function;
            switch(current_inter_id)
            {
                case VARIABLE_DECLERATION:
                    rpn_stack.push(operand::operand_def(1, current_inter.get_var(), 0, 0));
                    break;
                case CONST:
                    rpn_stack.push(operand::operand_def(current_inter.value, 0, 0, 0));
                    break;
                case VARIABLE_ACCESS:
                    // TODO: This should go back and find the "VARIABLE_TYPE" token since it isn't set in "intermediate.cpp" or do it in "intermediate.cpp"
                    rpn_stack.push(operand::operand_def(0, current_inter.get_var(), 0, current_inter.get_var()->type));
                    break;
                case WHILE_BEGIN:
                    if (rpn_stack.empty() && !value_in_r0.final_type)
                        std::cout << "Warning: Something should be on the RPN stack before starting a while statment.\nContinuing...\n";
                    else
                    {
                        if (!value_in_r0.final_type) // If R0 isn't being used already we off load the value on the stack into R0
                        {
                            add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                            rpn_stack.pop();
                        }
                        value_in_r0 = operand::operand_def(0, 0, 0, 0);
                    }
                    statment_stack.push(statment_defintion(2, std::to_string(current_statment_id)));
                    add_asm(in_func,
                            "while_start_" +
                                std::to_string(current_statment_id) +
                                ":\nCMP R0, #0\nBLE while_end_" +
                                std::to_string(current_statment_id),
                            asm_file, asm_functions);
                    current_statment_id++;
                    break;
                case IF_BEGIN:
                    if (rpn_stack.empty() && !value_in_r0.final_type)
                        std::cout << "Warning: Something should be on the RPN stack before starting an if statment.\nContinuing...\n";
                    else
                    {
                        if (!value_in_r0.final_type) // If R0 isn't being used already we off load the value on the stack into R0
                        {
                            add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                            rpn_stack.pop();
                        }
                        value_in_r0 = operand::operand_def(0, 0, 0, 0);
                    }
                    add_asm(in_func,
                            "CMP R0, #0\nBLE if_" +
                                std::to_string(current_statment_id),
                            asm_file, asm_functions);
                    statment_stack.push(statment_defintion(1, "if_" + std::to_string(current_statment_id) + ":"));
                    current_statment_id++;
                    break;
                case RESET_RPN:
                    rpn_stack = {};
                    // TODO: This should scale based on the # of registers used
                    add_asm(in_func, RESET_RPN_ASM_NORMAL, asm_file, asm_functions);
                    value_in_r0 = operand::operand_def(0, 0, 0, 0);
                    break;
                case IF_END:
                    add_asm(in_func, statment_stack.top().name, asm_file, asm_functions);
                    statment_stack.pop();
                    break;
                case WHILE_END:
                    if (rpn_stack.empty() && !value_in_r0.final_type)
                        std::cout << "Warning: Something should be on the RPN stack before ending a while statment.\nContinuing...\n";
                    else if (!value_in_r0.final_type)
                        add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);

                    add_asm(in_func,
                            "B while_start_" +
                                statment_stack.top().name +
                                "\nwhile_end_" +
                                statment_stack.top().name +
                                ":",
                            asm_file, asm_functions);
                    statment_stack.pop();
                    break;
                case FUNC_END:
                    // TODO: Add some more things like adding back SP
                    add_asm(in_func,
                            "ADD SP,#" +
                                std::to_string(get_function_token(statment_stack.top().name)->stack_space_needed) +
                                "\nBX LR",
                            asm_file, asm_functions);
                    statment_stack.pop();
                    in_func = false;
                    break;
                case FUNC_CALL:
                    current_function = current_inter.get_func();
                    if (current_function->inputs.size() > 8)
                        error::send_error("The function " + current_function->name + " takes more than eight values which is not valid.\n");
                    current_register = 0; // The current register we are offloading the inputs of the function into
                    // This offloads the needed values as input from the stack into the registers
                    for (variable_token* current_variable : current_function->inputs)
                    {
                        if (rpn_stack.empty() && !value_in_r0.final_type)
                            error::send_error("Expected something to be one the stack while calling function: " + current_function->name + "\n");

                        // If there is something in R0 right now
                        if (value_in_r0.final_type)
                        {
                            // If we can't transform R0 into the function input type
                            if (can_be_transformed_into(value_in_r0.final_type, current_variable->type))
                            {
                                std::cout << "Cannot ";
                                if (can_be_transformed_into(value_in_r0.final_type, current_variable->type) == 0)
                                    std::cout << "explicitly ";
                                error::send_error("transform type " + id_into_string(rpn_stack.top().final_type) + " into " + id_into_string(current_variable->type) + ".\n");
                            }
                            current_register++;
                            continue; // If we can transform the value in R0 into the input type we just continue and incrament "current_register"
                        }
                        // If the current thing on the stack can be transformed into the input type
                        if (!can_be_transformed_into(rpn_stack.top().final_type, current_variable->type))
                        {
                            add_asm(in_func, value_into_asm(rpn_stack.top(), current_register), asm_file, asm_functions);
                            rpn_stack.pop();
                            current_register++;
                        }
                        else
                        {
                            std::cout << "Cannot ";
                            if (can_be_transformed_into(rpn_stack.top().final_type, current_variable->type) == 0)
                                std::cout << "explicitly ";
                            error::send_error("transform type " + id_into_string(rpn_stack.top().final_type) + " into " + id_into_string(current_variable->type) + ".\n");
                        }
                    }
                    // This actually calls the function
                    add_asm(in_func,
                            "BL " +
                                current_inter.get_func()->name,
                            asm_file, asm_functions);
                    break;
                case FUNC_BEGIN:
                    if (!statment_stack.empty() || in_func)
                        error::send_error("Functions can only be defined in the global scope.\n");
                    if (!rpn_stack.empty())
                        error::send_error("RPN stack should be empty before defining a function.\n");
                    in_func = true;
                    statment_stack.push(statment_defintion(0, current_inter.get_func()->name));
                    add_asm(in_func,
                                current_inter.get_func()->name +
                                ":\n" +
                                "SUB SP,#" +
                                std::to_string(current_inter.get_func()->stack_space_needed),
                            asm_file, asm_functions);
                    current_register = 0;
                    for (variable_token* current_variable : current_inter.get_func()->inputs)
                    {
                        // If this is 32 bit
                        if (is_normal(current_variable->type))
                        {
                            add_asm(in_func,
                                    "STR R" +
                                        std::to_string(current_register) +
                                        ", [SP,#" +
                                        std::to_string(current_variable->stack_location + types_size[current_variable->type]) +
                                        "]",
                                    asm_file, asm_functions);
                        }
                        // If this is 8 bit
                        else if (is_char(current_variable->type))
                        {
                            add_asm(in_func,
                                    "STRB R" +
                                        std::to_string(current_register) +
                                        ", [SP,#" +
                                        std::to_string(current_variable->stack_location + types_size[current_variable->type]) +
                                        "]" +
                                        std::to_string(current_inter.get_func()->stack_space_needed),
                                    asm_file, asm_functions);
                        }
                        //else { std::cout << "Error while offloading function inputs into the stack.\n"; exit(-1); }
                    }
                    break;
            }
        }
        // This puts "asm_file" and "asm_functions" together
        asm_file.insert(asm_file.end(), asm_functions.begin(), asm_functions.end());
        return asm_file;
    }
}
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
