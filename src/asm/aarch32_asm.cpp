 /* This file turns the intermediate repersentation into aarch32 asm. */
#pragma once

#include<stack>
#include<vector>
#include"../intermediate/intermediate.hpp"
#include"../intermediate/intermediate.cpp"

namespace aarch32_asm {
// Operate on 0 registers
// BREAK
// CONTINUE
// QUIT
// Operate on only R0
#define NOT_ASM_BOOL "EOR R0, R0\n"
#define NOT_ASM_INT "CLZ R0, R0\nLSR R0, #5\n"
#define GET_ASM_NORMAL "LDR R0, [R0]\n"
#define GET_ASM_CHAR "LDRB R0, [R0]\n"
#define INCRAMENT_ASM_NORMAL "ADD R0, #1\n"
#define DECRAMENT_ASM_NORMAL "SUB R0, #1\n"
// IF_BEGIN
// WHILE_BEGIN
#define RETURN_ASM_NORMAL "BX LR\n"
// Operate on R0 & R1
#define LESS_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1\n"
#define LESS_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1\n"
#define GREATER_ASM_NORMAL "CMP R0, R1\nMOVLE R0, #0\nMOVGT R0, #1\n"
#define GREATER_EQUAL_ASM_NORMAL "CMP R0, R1\nMOVLQ R0, #0\nMOVGT R0, #1\n"
#define EQUAL_ASM_NORMAL "CMP R0, R1\nMOVEQ R0, #1\nMOVNE R0, #0\n"
#define ADD_ASM_NORMAL "ADD R0, R1\n"
#define	SUB_ASM_NORMAL "SUB R0, R1\n"
#define MUL_ASM_NORMAL "MUL R0, R1\n"
#define AND_ASM_NORMAL "AND R0, R1\n"
#define OR_ASM_NORMAL "ORR R0, R1\n"
#define XOR_ASM_NORMAL "EOR R0, R1\n"
#define LSL_ASM_NORMAL "LSL R0, R1\n"
#define LSR_ASM_NORMAL "LSR R0, R1\n"
#define RESET_RPN_ASM_NORMAL "MOV R0, #0\nMOV R1, R0\n"
// Operate on an unknown amount of registers and or stack
// Function calls // Function calls also need to reset registers

// This struct should be able to be turned into asm just based on it
// Also we should be able to get the type of it easily
struct value_defintion
{
    int const_value; // The value of this function if it's const
    variable_token* accessed_variable = 0; // If we are accessing a variable a pointer to it
    function_token* called_function = 0; // If we are calling a function a pointer to it
    unsigned char final_type = 255; // The type this operation would result in
    value_defintion(int _const_value, variable_token *_accessed_variable, function_token *_called_function, unsigned char _final_type) : const_value(_const_value), accessed_variable(_accessed_variable), called_function(_called_function), final_type(_final_type) {};
};

struct statment_defintion
{
    unsigned char type = 255;
    std::string name = "";
    statment_defintion(unsigned char _type, std::string _name) : type(_type), name(_name) {};
};

// This checks if a type is normal
bool is_normal(unsigned char type)
{
    if (type) { return true; } // TODO: When adding more types this needs to be changed to be correct
    return false;
};

// This either adds asm to "asm_file" or the top function in "asm_functions" based on "in_func"
void add_asm(bool in_func, std::string asm_to_add, std::vector<std::string> &asm_file, std::vector<std::string> &asm_functions)
{
    if (in_func) { asm_functions[asm_functions.size()-1] += asm_to_add; }
    else { asm_file.push_back(asm_to_add); }
}

// This turns a value defintion into its asm
std::string value_into_asm(value_defintion &current_def, int _register)
{
    // If we are defining a const
    if (!current_def.accessed_variable && !current_def.called_function)
    {
        // TODO: This should make sure the const is a valid const in arm asm
        // TODO: Find a better way of making the red squigglies go away on the line below
        return "MOV R" + std::to_string(_register) + ", #" + std::to_string(*(&(current_def.const_value))) + "\n";
    }
    // If we are accessing a variable
    if (current_def.accessed_variable)
    {
        return "LDR R" + std::to_string(_register) + ", [sp, #" + std::to_string(current_def.accessed_variable->stack_location+types_size[current_def.accessed_variable->type]) + "]\n"; 
    }
    // If we are calling a function
    if (current_def.called_function)
    {
        return "BL" + current_def.called_function->name + "\n";
    }
    return "";
}

// This function does a lot of the repetive asm from intermediate
inline void single_intermediate_into_asm(inter current_inter, std::stack<value_defintion> &rpn_stack, std::stack<statment_defintion> &statment_stack, bool in_func, std::vector<std::string> &asm_functions, std::vector<std::string> &asm_file, value_defintion registers_used[2])
{
    unsigned char current_inter_id = current_inter.id;
    if (current_inter_id == ASM)
    {
        add_asm(in_func, current_inter.refrenced_name, asm_file, asm_functions);
    }
    bool was_for_one = false; // Hopefully this saves a bit of time
    // TODO: This things under here should make sure the type on the rpn stack is the same as their type
    if (current_inter_id >= NOT && current_inter_id <= DECRAMENT) 
    {
        // If R0 isn't used
        if (!registers_used) 
        { 
            add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
            registers_used[0] = rpn_stack.top();
            rpn_stack.pop();
        }
        was_for_one = true;
    }
    //if (is_norma(rpn_stack) && was_for_one)
    if (1 && was_for_one)
    {
        if (current_inter_id == NOT) { add_asm(in_func, NOT_ASM_INT, asm_file, asm_functions); }
        if (current_inter_id == GET) { add_asm(in_func, GET_ASM_NORMAL, asm_file, asm_functions); }
        if (current_inter_id == MEM_ADDRS)
        {
            // This needs more than the others 
        }
        if (current_inter_id == INCRAMENT) { add_asm(in_func, INCRAMENT_ASM_NORMAL, asm_file, asm_functions); }
        if (current_inter_id == DECRAMENT) { add_asm(in_func, DECRAMENT_ASM_NORMAL, asm_file, asm_functions); }
        return;
    }
    // NOT -> DECRAMENT			operate on one and leave one
    // AND -> MUL				operate on two and leave one
    // FUNC_CALL				operates on an unknown amount and leaves one. Both an operator and a value
    // Prepping for a comparision with two types
    if (current_inter_id >= AND && current_inter_id <= MUL) 
    {
        // If R0 isn't being used
        if (!registers_used)
        {
            add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
            registers_used[0] = rpn_stack.top();
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
inline std::vector<std::string> intermediates_into_asm(std::vector<inter> &inter_file)
{
    std::vector<std::string> asm_functions; // This is the asm of functions that gets added to the end of "asm_file"
    // std::vector<std::string> asm_data;
    // ^^^ Currently not used
    std::vector<std::string> asm_file = { ".ASM\n.GLOBAL fn_main\n.TEXT\nfn_end:\nMOV R7, #1\nSWI 0\n" };
    std::stack<value_defintion> rpn_stack;
    std::stack<statment_defintion> statment_stack; // 0 -> function call, 1 -> while, 2 -> if, 3 -> return, 4 -> break, 5-> continue
    // TODO: We need a rpn value stack here
    unsigned int current_statment_id = 0; // This is just a little bit of padding added to the end of branch points to make sure they are seperate
    // TODO: I can't think of the word above. "branch point" is not it                            ^^^^^^^^^^^^^
    bool in_func = 0; // If we are inside a function ie. if we are adding the asm to asm_file or asm_functions
    value_defintion registers_used[2] = { value_defintion(0, 0, 0, 0), value_defintion(0, 0, 0, 0) }; // The values in R0-R7. Currently only supports R0 and R1. This should have its own struct because we can't relly on consts anymore we just need the type
    unsigned char current_inter_id; // The current intermediate's id hopefully in a register
    for (inter current_inter : inter_file)
    {
        current_inter_id = current_inter.id;
        std::cout << "ID: " << (int)current_inter_id << "\n";
        single_intermediate_into_asm(current_inter, rpn_stack, statment_stack, in_func, asm_functions, asm_file, registers_used);
        if (current_inter_id == CONST) { rpn_stack.push(value_defintion(current_inter.value, 0, 0, 255)); }
        if (current_inter_id == VARIABLE_ACCESS) 
        {
            rpn_stack.push(value_defintion(0, current_inter.refrenced_variable_token, 0, 0));
        }
        if (current_inter_id == FUNC_BEGIN) 
        {
            if (!statment_stack.empty()) { std::cout << "Functions can only be defined in the global scope.\n"; exit(-1); }
            if (!rpn_stack.empty()) { std::cout << "RPN stack should be empty before defining a function.\n"; exit(-1); }
            in_func = true;
            statment_stack.push(statment_defintion(0, "fn_" + current_inter.refrenced_name));
            asm_functions.push_back("");
            add_asm(in_func, "fn_" + current_inter.refrenced_name + ":\n", asm_file, asm_functions);
        }
        if (current_inter_id == WHILE_BEGIN) 
        { 
            statment_stack.push(statment_defintion(1, "while_" + std::to_string(current_statment_id)));
            add_asm(in_func, "while_" + std::to_string(current_statment_id), asm_file, asm_functions);
            current_statment_id++;
        }
        if (current_inter_id == IF_BEGIN) 
        {
            if (rpn_stack.empty()) { std::cout << "Warning: Something should be on the RPN stack before starting an if statment.\nContinuing...\n"; break; }
            if (!registers_used) // If R0 isn't being used already we off load the value on the stack into R0
            { 
                add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions);
                registers_used[0] = rpn_stack.top();
                rpn_stack.pop();
            }
            statment_stack.push(statment_defintion(2, "CMP R0, #1\nBLE if_" + std::to_string(current_statment_id) + ":\n"));
            current_statment_id++;
        }
        if (current_inter_id == RESET_RPN) 
        {
            rpn_stack = {};
            // TODO: This should scale based on the # of registers used
            add_asm(in_func, RESET_RPN_ASM_NORMAL, asm_file, asm_functions);
            value_defintion registers_used[2] = { value_defintion(0, 0, 0, 0), value_defintion(0, 0, 0, 0) }; // We set all registers to not used
        }
        if (current_inter_id == IF_END)
        {
            add_asm(in_func, statment_stack.top().name, asm_file, asm_functions);
            statment_stack.pop();
        }
        if (current_inter_id == WHILE_END)
        {
            // This makes sure we have thing on the stack before the end of a while statment
            if (rpn_stack.empty()) { std::cout << "Warning: something should be on the RPN stack before ending a while statment.\nContinuing...\n"; break; }
            if (!registers_used) { add_asm(in_func, value_into_asm(rpn_stack.top(), 0), asm_file, asm_functions); }
            add_asm(in_func, "CMP R0, #1\nBLE " + statment_stack.top().name + "\n", asm_file, asm_functions);
            statment_stack.pop();
        }
        // Func calls
        if (current_inter_id == FUNC_CALL) { }
    }
    asm_file.insert(asm_file.end(), asm_functions.begin(), asm_functions.end());
    return asm_file;
}
// Undefs
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
}
