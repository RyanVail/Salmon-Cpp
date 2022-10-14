/* This file turns tokens into their intermiedate form and does a few optimization passes along the way -Ryan Vail Sep. 4th 2022 */

#pragma once

#include<vector>
#include<iostream>
#include<stack>

// TODO: Remove these when reordering the intermediate tokens
//  3			Adds a value to the RPN stack
//  31			Adds a value to the RPN stack
//  20 - 23		Removes a value from the RPN stack

// The lower comments were made with a tab size of four
#define VARIABLE_TYPE 0
#define VARIABLE_DECLERATION 1
#define VARIABLE_REASIGNMENT 2 // TODO: This is currently not used since it is done during the transfer to ASM but once opt. is added it will be needed
#define VARIABLE_ACCESS 3
// TODO: Add a variable [] intermediate instruction
#define NOT 4				// '!'
#define GET 5				// '@'
#define MEM_ADDRS 6			// '%'
#define INCRAMENT 7			// "++"
#define DECRAMENT 8			// "--"
#define AND 9				// '&'
#define OR 10				// '|'
#define XOR 11				// '^'
#define LSL 12				// "<<"
#define LSR 13				// ">>"
#define EQUAL 14			// '='
#define IS_EQUAL 15			// "=="
#define LESS 16				// '<'
#define LESS_EQUAL 17		// "<="
#define GREATER 18			// '>'
#define GREATER_EQUAL 19	// ">="
#define ADD 20				// '+'
#define	SUB 21				// '-'
#define DIV 22				// '/'
#define MUL 23				// '*'
#define FUNC_BEGIN 24
#define FUNC_END 25
#define FUNC_CALL 26		// '$'
#define IF_BEGIN 27
#define IF_END 28
#define VARIABLE_NAME 29
#define RESET_RPN 30		// ';'
#define CONST 31
#define WHILE_BEGIN 32
#define WHILE_END 33
// TODO: The below functions need to be added to "debug.cpp"
#define ELSE_BEGIN 34
#define ELSE_END 35
#define CONTINUE 36
#define BREAK 37
#define RETURN 38

// TODO: Add continue
// TODO: Add break
// TODO: This needs to cross refrence with the symbols table to get variable assignment vs variable declearation

struct inter
{
	// id should be type and be a unsigned char
	int id; // This is the id of the intermediate token
	int value; // This is a value or variable type id
	std::string refrenced_name; // This is the name of the function or variable that is refrenced
	inter(unsigned char id_value, int value_value, std::string refrenced_name_value) : id(id_value), value(value_value), refrenced_name(refrenced_name_value) {};
};

#if Debug
	void print_inter(inter to_print);
#endif

// TODO: We don't need to be returning the intermediate anymore since we are passing a refrence to the outputed intermediate
// This turns a single set of select tokens into their intermediate form
inline inter single_into_inter(std::string *itr, std::vector<inter> &inter_output, int rpn_size, int current_owner, int &current_stack)
{
	if (*itr == "!") { return inter(NOT, 0, ""); }
	if (*itr == "@") { return inter(GET, 0, ""); }
	if (*itr == "%") { return inter(MEM_ADDRS, 0, ""); }
	if (*itr == "++") { return inter(INCRAMENT, 0, ""); }
	if (*itr == "--") { return inter(DECRAMENT, 0, ""); }
	if (*itr == "&") { return inter(AND, 0, ""); }
	if (*itr == "|") { return inter(OR, 0, ""); }
	if (*itr == "^") { return inter(XOR, 0, ""); }
	if (*itr == "<<") { return inter(LSL, 0, ""); }
	if (*itr == ">>") { return inter(LSR, 0, ""); }
	if (*itr == "=") { return inter(EQUAL, 0, ""); }
	if (*itr == "==") { return inter(IS_EQUAL, 0, ""); }
	if (*itr == "<") { return inter(LESS, 0, ""); }
	if (*itr == "<=") { return inter(LESS_EQUAL, 0, ""); }
	if (*itr == ">") { return inter(GREATER, 0, ""); }
	if (*itr == ">=") { return inter(GREATER_EQUAL, 0, ""); }
	if (*itr == "+") { return inter(ADD, 0, ""); }
	if (*itr == "-") { return inter(SUB, 0, ""); }
	if (*itr == "/") { return inter(DIV, 0, ""); }
	if (*itr == "*") { return inter(MUL, 0, ""); }
	if (*itr == "return") { inter_output.push_back(inter(RETURN, 0, "")); }
	if (*itr == "break") { inter_output.push_back(inter(BREAK, 0, "")); }
	if (*itr == "continue") { inter_output.push_back(inter(CONTINUE, 0, "")); }
	// This checks if the string is a valid variable
	if (is_str_letters(*itr))
	{
		// If the last intermediate token was a type we are defining a variable
			// Otherwise we check the symbol table and assume we are accessing a variable
		if (inter_output.size() && inter_output[inter_output.size()-1].id == VARIABLE_TYPE)
		{
			if (rpn_size) { std::cout << "There must be nothing on the RPN stack to define a variable.\n"; exit(-1); }
			current_stack -= types_size[into_id(inter_output[inter_output.size()-1].refrenced_name)];
			add_variable_token(*itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);
			return inter(VARIABLE_DECLERATION, 0, *itr);
			// We add the variable to the symbol table
		}
		// We check if the variable is in the symbol table, if not we send an error
		if (!get_variable_token(*itr)) { std::cout << "Unkown variable: " << *itr << ".\n"; exit(-1); }
		return inter(VARIABLE_NAME, 0, *itr);
	}
	// This checks if it's a type
	if (is_type(*itr) != -1)
	{
		std::cout << is_type(*itr) << "\n";
		return inter(VARIABLE_TYPE, 0, *itr);
		// If we have a type we need to make the last token into this type
			// If the last token is a variable name offload the type decleration to the symbol table
			// If the last token is a constant just set the first char of the string to the type's id
	}
	std::cout << "Unknown token: " << *itr << ".\n";
	exit(-1);
}
// This turns a file into a series of inters
std::vector<inter> file_into_inter(std::vector<std::string> &file)
{
	std::stack<inter> statment_stack;
	std::vector<inter> inter_output;
	int rpn_size = 0;
	int current_owner = -1; // If there is a variable defintion this will be the value of the owner
	int current_stack = 0; // The current stack location
	for (std::vector<std::string>::iterator itr = file.begin(); itr != file.end(); itr++)
	{
		if (rpn_size < 0) { std::cout << "RPN stack size fell below zero.\n"; exit(-1); }
		// If we have a constant
		// TODO: These two function can be moved into single_into_inter
		if (is_str_num(*itr))
		{
			inter_output.push_back(inter(CONST, get_str_num(*itr), ""));
			rpn_size++;
			continue;
		}
		// If we hit a '$' meaning function call
		// TODO: '$', '%', etc. are not special chars in "tokenizer.cpp" yet
		if (*itr == "$") 
		{
			itr++; // Incraments itr placing us at the function name
			// Makes sure the function call name is valid
			if (!get_function_token(*itr)) { std::cout << "Unknown function: " << *itr << ".\n"; exit(-1); }
			inter_output.push_back(inter(FUNC_CALL, get_function_token(*itr)->id, *itr));
			// TODO: We need to get the input size of the function and subtract rpn size by that
			continue;
		}
		// If we hit a '}'
		if (*itr == "}")
		{
			if (statment_stack.empty()) { std::cout << "Unexpected '}'.\n"; exit(-1); }
			inter_output.push_back(statment_stack.top());
			statment_stack.pop();
			continue;
		}
		// If we hit a ';' and are defining the reset of the RPN stack
		if (*itr == ";")
		{
			inter_output.push_back(inter(RESET_RPN, 0, ""));
			rpn_size = 0;
			continue;
		}
		// If we have an if statment
		if (*itr == "if")
		{
			inter_output.push_back(inter(IF_BEGIN, 0, ""));
			statment_stack.push(inter(IF_END, 0, ""));
			if (*(itr+1) != "{") { std::cout << "Expected '{' after an if statment.\n"; exit(-1); }
		}
		// If we have a else statment
		if (*itr == "else")
		{
			if (inter_output[inter_output.size()-1].id != IF_END) { std::cout << "Expected an end to an if statment before an else statment.\n"; exit(-1); }
			if (*(itr+1) != "{" && *(itr+1) != "if") { std::cout << "Expected '{' or if statment after else statment.\n"; exit(-1); }
			inter_output.push_back(inter(ELSE_BEGIN, 0, ""));
			statment_stack.push(inter(ELSE_END, 0, ""));
		}
		// If we have a while loop
		if (*itr == "while")
		{
			inter_output.push_back(inter(WHILE_BEGIN, 0, ""));
			statment_stack.push(inter(WHILE_END, 0, ""));
		}
		// If we are defining a function
		if (*itr == "fn")
		{
			if (current_owner != -1) { std::cout << "Function defintion inside another function is not valid.\n"; exit(-1); }
			if(rpn_size) { std::cout << "Expected the nothing on the RPN stack before a function definiton.\n"; exit(-1); }
			itr++;
			std::string name = (*itr).substr(0, (*itr).find('(') );
			if(!is_str_letters(name)) { std::cout << "The function name: " << name << " contains invalid characters.\n"; exit(-1); }
			// TODO: This should make sure the name is valid
			add_function_token(name);
			symbol_table.functions[symbol_table.functions.size()-1].stack_space_needed = 16;
			// This reads through the input variables of the function
			unsigned char current_type = 0; // The type of the current variable
			int stack_space_needed = 0; // The stack space needed for all the input variables to the function
			// TODO: Input variables shouldn't use stack space by default rather registers
			while (true)
			{ 
				std::string token = *itr;
				if (token == ",") { itr++; continue; }
				if (token == ")") { break; }
				// TODO: Add default varaible assignment
				if (token == "=") { std::cout << "Variable default assignemt has not been added yet.\n"; exit(-1); }
				// If this token is the variable name
				if (current_type) 
				{
					// This checks if there is another variable with the same name
					// TODO: Each function should have its own list of local and global vars rather than just every var. Add a function called get_local_variable_token which just checks for variables in this owner and global owner
					// TODO: This should check if the variable name is valid
					if (get_variable_token(token)) { std::cout << "A variable with that name already exists.\n"; exit(-1); }
					stack_space_needed -= types_size[current_type];
					add_variable_token(token, current_type, symbol_table.functions.size()-1, stack_space_needed);
					current_type = 0;
				}
				// If this variable is the type
				else { current_type = into_id(token); }
				itr++;
			}
			itr += 1;
			inter_output.push_back(inter(FUNC_BEGIN, 0, ""));
			statment_stack.push(inter(FUNC_END, 0, ""));
			if (*(itr+1) != "{") { std::cout << "Expected '{' after a function defintion.\n"; exit(-1); }
			continue;
		}
		// This turns any remaining tokens into their intermediate forms
		inter_output.push_back(single_into_inter(&(*itr),inter_output,rpn_size,current_owner,current_stack));
		// TODO: When reordering the constants order them in a way these two function look nicer
		if (inter_output[inter_output.size()-1].id == EQUAL) { rpn_size--; }
		if ((inter_output[inter_output.size()-1].id >= VARIABLE_DECLERATION && inter_output[inter_output.size()-1].id <= VARIABLE_ACCESS)) { rpn_size++; }
		if ((inter_output[inter_output.size()-1].id >= 20 && inter_output[inter_output.size()-1].id <= 23) || (inter_output[inter_output.size()-1].id >= 8 && inter_output[inter_output.size()-1].id <= 19)) { rpn_size--; }
	}
	#if Debug
		for (std::vector<inter>::iterator itr = inter_output.begin(); itr != inter_output.end(); itr++) { print_inter(*itr); }
	#endif
	symbol_table.stack_space_needed = current_stack;
	return inter_output;
}
