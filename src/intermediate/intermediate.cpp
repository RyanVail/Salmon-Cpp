/* This file turns tokens into their intermiedate form and does a few optimization passes along the way -Ryan Vail Sep. 4th 2022 */

#pragma once

#include<vector>
#include<iostream>
#include<stack>
#include"../symbol_table.cpp"
#include"../var_types.cpp"
#include"../main.cpp"
#include"preprocessor.cpp"
#include"intermediate.hpp"

#if Debug
	void print_inter(inter to_print);
#endif

// TODO: We don't need to be returning the intermediate anymore since we are passing a refrence to the outputed intermediate
// This turns a single set of select tokens into their intermediate form
inline inter single_into_inter(std::string *itr, std::vector<inter> &inter_output, int rpn_size, int current_owner, int &current_stack)
{
	std::cout << "asdf: "<< *itr << "\n";
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
		// TODO: If the variable is in the global scope we need to add it to .data
		// If the last intermediate token was a type we are defining a variable
		// Otherwise we check the symbol table and assume we are accessing a variable
		if (inter_output.size() && inter_output[inter_output.size()-1].id == VARIABLE_TYPE)
		{
			if (rpn_size) { std::cout << "There must be nothing on the RPN stack to define a variable.\n"; exit(-1); }
			current_stack -= types_size[into_id(inter_output[inter_output.size()-1].refrenced_name)];
			add_variable_token(*itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);
			return inter(VARIABLE_DECLERATION, 0, *itr);
		}
		// We check if the variable is in the symbol table, if not we send an error
		if (!get_variable_token(*itr)) { std::cout << "Unknown variable: " << *itr << ".\n"; exit(-1); }
		return inter(VARIABLE_ACCESS, 0, *itr, get_variable_token(*itr));
	}
	// This checks if it's a type
	if (is_type(*itr) != -1)
	{
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
	// TODO: All errors should show what they got instead of just what they expected
	std::stack<inter> statment_stack;
	std::vector<inter> inter_output;
	// TODO: Intermediate should only be operating on two values at a time to make optimizations faster
	int rpn_size = 0;
	int current_owner = -1; // If there is a variable defintion this will be the value of the owner
	int current_stack = 0; // The current stack location
	for (std::vector<std::string>::iterator itr = file.begin(); itr != file.end(); itr++)
	{
		if (rpn_size < 0) { std::cout << "RPN stack size fell below zero.\n"; exit(-1); }
		// These till const are called when we are doing "preproccesing"
		preprocessor::try_to_pass_comments(file, itr);
		if (*itr == "#")
		{
			std::cout << *(itr+1) << "\n"; exit(0);
			preprocessor::process_instruction(file, &(*itr), inter_output);
			std::cout << *itr << "\n"; exit(-1);
		}
		// If we have a constant
		if (is_str_num(*itr))
		{
			inter_output.push_back(inter(CONST, get_str_num(*itr), ""));
			rpn_size++;
			continue;
		}
		// If we hit a '$' meaning function call
		if (*itr == "$") 
		{
			itr++; // Incraments itr placing us at the function name
			// Makes sure the function call name is valid
			if (!get_function_token(*itr)) { std::cout << "Unknown function: " << *itr << ".\n"; exit(-1); }
			inter_output.push_back(inter(FUNC_CALL, get_function_token(*itr)->id, *itr));
			rpn_size -= get_function_token(*itr)->inputs.size(); // Shrinks the RPN stack size by the input variable size
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
			// If there is nothing in the stack we don't need to reset it
			if (!rpn_size && optimization_level > 0) { continue; }
			inter_output.push_back(inter(RESET_RPN, 0, ""));
			rpn_size = 0;
			continue;
		}
		// If we have an if statment
		if (*itr == "if")
		{
			inter_output.push_back(inter(IF_BEGIN, 0, ""));
			statment_stack.push(inter(IF_END, 0, ""));
			itr++;
			if (*(itr) != "{") { std::cout << "Expected '{' after an if statment.\n"; exit(-1); }
			rpn_size--;
			continue;
		}
		// If we have a else statment
		if (*itr == "else")
		{
			if (inter_output[inter_output.size()-1].id != IF_END) { std::cout << "Expected an end to an if statment before an else statment.\n"; exit(-1); }
			itr++;
			if (*(itr) != "{" && *(itr) != "if") { std::cout << "Expected '{' or if statment after else statment.\n"; exit(-1); }
			inter_output.push_back(inter(ELSE_BEGIN, 0, ""));
			statment_stack.push(inter(ELSE_END, 0, ""));
			continue;
		}
		// If we have a while loop
		if (*itr == "while")
		{
			inter_output.push_back(inter(WHILE_BEGIN, 0, ""));
			statment_stack.push(inter(WHILE_END, 0, ""));
			if (*(itr+1) != "{") { std::cout << "Expected '{' after an if statment.\n"; exit(-1); }
			rpn_size--;
			continue;
		}
		// If we are defining a function
		if (*itr == "fn")
		{
			if(current_owner != -1) { std::cout << "Function defintion inside another function is not valid.\n"; exit(-1); }
			if(rpn_size) { std::cout << "Expected the nothing on the RPN stack before a function definiton.\n"; exit(-1); }
			itr++;
			std::string name = (*itr).substr(0, (*itr).find('(') );
			// This checks for a lot of possible errors
			if(!is_str_letters(name)) { std::cout << "The function name: " << name << " contains invalid characters.\n"; exit(-1); }
			if(get_function_token(name)) { std::cout << "The function name: " << name << " is already used.\n"; exit(-1); }
			if(!is_valid_name(name)) { std::cout << "The function name: " << name << " isn't valid.\n"; exit(-1); }
			add_function_token(name);
			symbol_table.functions[symbol_table.functions.size()-1].stack_space_needed = 16;
			// This reads through the input variables of the function
			unsigned char current_type = 0; // The type of the current variable
			int stack_space_needed = 0; // The stack space needed for all the input variables to the function
			// TODO: Input variables shouldn't use stack space by default, registers would be a lot better
			while (true)
			{
				if (itr == file.end()) { std::cout << "Error while parsing function defintion: reached end of file.\n"; exit(-1); }
				std::string token = *itr;
				if (token == ",") { itr++; continue; }
				if (token == ")") { break; }
				// TODO: Add default varaible assignment
				if (token == "=") { std::cout << "Variable default assignemt has not been added yet.\n"; exit(-1); }
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
			inter_output.push_back(inter(FUNC_BEGIN, 0, name));
			statment_stack.push(inter(FUNC_END, 0, name));
			std::cout << *itr << "\n";
			if (*(itr) != "{") { std::cout << "Expected '{' after a function defintion.\n"; exit(-1); }
			continue;
		}
		// This turns any remaining tokens into their intermediate forms
		inter_output.push_back(single_into_inter(&(*itr),inter_output,rpn_size,current_owner,current_stack));
		// TODO: When reordering the constants order them in a way these two function look nicer
		if (inter_output[inter_output.size()-1].id == EQUAL) { rpn_size--; }
		if ((inter_output[inter_output.size()-1].id >= VARIABLE_DECLERATION && inter_output[inter_output.size()-1].id <= VARIABLE_ACCESS)) { rpn_size++; }
		if ((inter_output[inter_output.size()-1].id >= 20 && inter_output[inter_output.size()-1].id <= 23) || (inter_output[inter_output.size()-1].id >= 8 && inter_output[inter_output.size()-1].id <= 19)) { rpn_size--; }
	}
	// Makes sure the statment stack is done
	if (statment_stack.size()) { std::cout << "Expected all statments to be closed before the end of the file.\n"; exit(-1); }
	// Prints all intermediates if we are in debug mode
	#if Debug
		for (std::vector<inter>::iterator itr = inter_output.begin(); itr != inter_output.end(); itr++) { print_inter(*itr); }
	#endif
	symbol_table.stack_space_needed = current_stack;
	return inter_output;
}
