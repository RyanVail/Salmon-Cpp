/* This file turns tokens into their intermiedate forms */

#include<intermediate/intermediate.hpp>
#include<vector>
#include<iostream>
#include<stack>
#include<string>
#include<vartypes.hpp>
#include<symboltable.hpp>
#include<main.hpp>
#include<intermediate/postprocessor.hpp>
#include<debug.hpp>
#include<operanddefinition.hpp>
#include<error.h>

static std::stack<operand::operand_def> operand_stack;
static std::stack<inter> statment_stack;
static std::vector<inter> inter_output;
static i32 current_owner;
static i32 current_stack;

inline void add_const_operand(u8 _type, u32 _const)
{
	operand_stack.push(operand::operand_def(_const, 0, 0, _type));
}
inline void add_variable_operand(variable_token *to_add)
{
	u8 _variable_type = to_add->type;
	operand_stack.push(operand::operand_def(0, to_add, 0, _variable_type));
}
inline void add_function_call_operand(function_token *to_add)
{
	u8 _func_return_type = to_add->output;
	operand_stack.push(operand::operand_def(0, 0, to_add, _func_return_type));
}

// This takes the top operand off and generates its intermediate repersentation
void top_operand_to_inter()
{
	// TODO: Add this
}

// This function turns single char operations into their intermediate form
// Returns true if it added something
inline bool single_char_operators_into_inter(i8 operator_char)
{
	switch(operator_char)
	{
	case '!':
		inter_output.push_back(inter(NOT, 0, ""));
		break;
	case '@':
		inter_output.push_back(inter(GET, 0, ""));
		break;
	case '%':
		inter_output.push_back(inter(MEM_ADDRS, 0, ""));
		break;
	case '&':
		inter_output.push_back(inter(AND, 0, ""));
		break;
	case '|':
		inter_output.push_back(inter(OR, 0, ""));
		break;
	case '^':
		inter_output.push_back(inter(XOR, 0, ""));
		break;
	case '=':
		inter_output.push_back(inter(EQUAL, 0, ""));
		break;
	case '<':
		inter_output.push_back(inter(LESS, 0, ""));
		break;
	case '>':
		inter_output.push_back(inter(GREATER, 0, ""));
		break;
	case '+':
		inter_output.push_back(inter(ADD, 0, ""));
		break;
	case '-':
		inter_output.push_back(inter(SUB, 0, ""));
		break;
	case '/':
		inter_output.push_back(inter(DIV, 0, ""));
		break;
	case '*':
		inter_output.push_back(inter(MUL, 0, ""));
		break;
	default:
		return false;
	}
	return true;
}

// This handles turning operators into their intermediate repersentation
// Returns true if it added something
inline bool operators_into_inter(std::string *token_itr)
{
	if (single_char_operators_into_inter((*token_itr)[0]))
		return true;

	u32 tmp = inter_output.size();

	if (*token_itr == "<<")
		inter_output.push_back(inter(LSL, 0, ""));
	if (*token_itr == ">>")
		inter_output.push_back(inter(LSR, 0, ""));
	if (*token_itr == "==")
		inter_output.push_back(inter(IS_EQUAL, 0, ""));
	if (*token_itr == "<=")
		inter_output.push_back(inter(LESS_EQUAL, 0, ""));
	if (*token_itr == "++")
		inter_output.push_back(inter(INCRAMENT, 0, ""));
	if (*token_itr == "--")
		inter_output.push_back(inter(DECRAMENT, 0, ""));
	if (*token_itr == "return")
		inter_output.push_back(inter(RETURN, 0, ""));
	if (*token_itr == "break")
		inter_output.push_back(inter(BREAK, 0, ""));
	if (*token_itr == "continue")
		inter_output.push_back(inter(CONTINUE, 0, ""));
	if (*token_itr == ">=")
		inter_output.push_back(inter(GREATER_EQUAL, 0, ""));

	if (inter_output.size() == tmp)
		return false;
	return true;
}

// This turns a single set of select tokens into their intermediate form
inline void single_into_inter(std::string *itr, i32 rpn_size, i32 current_owner, i32 &current_stack)
{
	if (operators_into_inter(itr))
		return;

	// This checks if the string is a valid variable
	if (is_str_letters(*itr))
	{
		// TODO: If the variable is in the global scope we need to add it to .data
		// If the last intermediate token was a type we are defining a variable
		// Otherwise we check the symbol table and assume we are accessing a variable
		if (inter_output.size() && inter_output[inter_output.size()-1].id == VARIABLE_TYPE)
		{
			if (rpn_size)
			{
				std::cout << "There must be nothing on the RPN stack to define a variable.\n"; 
				exit(-1);
			}

			current_stack -= types_size[into_id(inter_output[inter_output.size()-1].refrenced_name)];

			//add_variable_token(*itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);
			add_variable_token(*itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);

			inter_output.push_back(inter(VARIABLE_DECLERATION, 0, *itr, get_variable_token(*itr)));
		}

		// We check if the variable is in the symbol table, if not we send an error
		if (!get_variable_token(*itr))
			error::send_error("Unknown variable: " + *itr + ".\n");

		inter_output.push_back(inter(VARIABLE_ACCESS, 0, *itr, get_variable_token(*itr)));
		return;
	}

	// This checks if it's a type
	if (is_type(*itr) != -1)
	{
		inter_output.push_back(inter(VARIABLE_TYPE, 0, *itr));
		// If we have a type we need to make the last token into this type
			// If the last token is a variable name offload the type decleration to the symbol table
			// If the last token is a constant just set the first char of the string to the type's id
		return;
	}

	error::send_error("Unknown token: " + *itr + ".\n");
}

// This turns a file into a series of inters
std::vector<inter> file_into_inter(std::vector<std::string> file)
{
	// TODO: All errors should show what they got instead of just what they expected
	current_owner = -1;
	for (std::vector<std::string>::iterator itr = file.begin(); itr != file.end(); itr++)
	{
		// If we are doing postprocessing
		if (*itr == "#")
		{
			postprocessor::process_instruction(file, itr, inter_output);
			continue;
		}

		// If we have a constant
		if (is_str_num(*itr))
		{
			// TODO: This assumes the type is always u32, it shouldn't do that!
			add_const_operand(7, get_str_num(*itr));
			continue;
		}

		// If we hit a '$' meaning function call
		if (*itr == "$")
		{
			// itr is the function name now
			itr++;

			// Makes sure the function call name is valid
			if (!get_function_token(*itr))
				error::send_error("Unknown function: " + *itr + ".\n");

			inter_output.push_back(inter(FUNC_CALL, get_function_token(*itr)->id, *itr));

			rpn_size -= get_function_token(*itr)->inputs.size();

			continue;
		}

		// If we hit a '}'
		if (*itr == "}")
		{
			if (rpn_size && statment_stack.top().id != WHILE_END)
				error::send_error("RPN stack size should be zero before ending a statment.\n");

			if (rpn_size >= 2) 
				error::send_error("Expected one thing operand after ending a while statment but got: " + std::to_string(operand_stack.size()) + ".\n");
			
			if (statment_stack.empty())
			{
				std::cout << "Unexpected '}'.\n"; 
				exit(-1);
			}

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
			if (*(itr) != "{") 
			{ 
				std::cout << "Expected '{' after an if statment.\n"; 
				exit(-1); 
			}
			rpn_size--;
			continue;
		}

		// If we have a else statment
		if (*itr == "else")
		{
			if (inter_output[inter_output.size()-1].id != IF_END)
			{
				std::cout << "Expected an end to an if statment before an else statment.\n"; 
				exit(-1);
			}

			itr++;

			if (*(itr) != "{" && *(itr) != "if")
			{
				std::cout << "Expected '{' or if statment after else statment.\n"; 
				exit(-1);
			}

			inter_output.push_back(inter(ELSE_BEGIN, 0, ""));
			statment_stack.push(inter(ELSE_END, 0, ""));
			continue;
		}

		// If we have a while loop
		if (*itr == "while")
		{
			inter_output.push_back(inter(WHILE_BEGIN, 0, ""));
			statment_stack.push(inter(WHILE_END, 0, ""));
			itr++;

			if (*(itr) != "{") 
			{ 
				std::cout << "Expected '{' after a while statment.\n"; 
				exit(-1); 
			}

			rpn_size--;
			continue;
		}

		// If we are defining a function
		if (*itr == "fn")
		{
			if(current_owner != -1) 
			{ 
				std::cout << "Function defintion inside another function is not valid.\n"; 
				exit(-1); 
			}

			if(rpn_size)
			{ 
				std::cout << "Expected the nothing on the RPN stack before a function definiton.\n"; 
				exit(-1); 
			}

			itr++;
			std::string name = (*itr).substr(0, (*itr).find('(') );

			if(!is_str_letters(name)) 
			{ 
				std::cout << "The function name: " << name << " contains invalid characters.\n"; 
				exit(-1); 
			}

			if(get_function_token(name)) 
			{ 
				std::cout << "The function name: " << name << " is already used.\n"; 
				exit(-1); 
			}

			if(!is_valid_name(name)) 
			{ 
				std::cout << "The function name: " << name << " isn't valid.\n"; 
				exit(-1); 
			}

			add_function_token(name);
			symbol_table.functions[symbol_table.functions.size()-1].stack_space_needed = 0;
			// This reads through the input variables of the function
			u8 current_type = 0; // The type of the current variable
			i32 stack_space_needed = 0; // The stack space needed for all the input variables to the function
			// TODO: Input variables shouldn't use stack space by default, registers would be a lot better
			while (true)
			{
				if (itr == file.end())
				{
					std::cout << "Error while parsing function defintion: reached end of file.\n"; 
					exit(-1);
				}

				std::string token = *itr;

				if (token == ",") { itr++; continue; }

				if (token == ")")
					break;

				// TODO: Add default varaible assignment
				if (token == "=")
				{
					std::cout << "Default function input variable assignemt has not yet been added.\n"; 
					exit(-1);
				}

				if (current_type)
				{
					// This checks if there is another variable with the same name
					// TODO: Each function should have its own list of local and global vars rather than just every var. Add a function called get_local_variable_token which just checks for variables in this owner and global owner
					// TODO: This should check if the variable name is valid
					if (get_variable_token(token)) 
					{ 
						std::cout << "A variable with that name already exists.\n"; 
						exit(-1); 
					}
					stack_space_needed -= types_size[current_type];
					add_variable_token(token, current_type, symbol_table.functions.size()-1, stack_space_needed);
					current_type = 0;
				}
				// If this variable is the type
				else
					current_type = into_id(token);

				itr++;
			}

			itr += 1;
			inter_output.push_back(inter(FUNC_BEGIN, 0, name));
			statment_stack.push(inter(FUNC_END, 0, name));
			std::cout << *itr << "\n";

			if (*(itr) != "{")
			{
				std::cout << "Expected '{' after a function defintion.\n"; 
				exit(-1);
			}

			continue;
		}

		// This turns any remaining tokens into their intermediate forms
		single_into_inter(&(*itr),rpn_size,current_owner,current_stack);

		// Updating the size of the RPN stack
		if (inter_output[inter_output.size()-1].id == EQUAL)
			rpn_size--;
		
		if ((inter_output[inter_output.size()-1].id >= VARIABLE_DECLERATION && inter_output[inter_output.size()-1].id <= VARIABLE_ACCESS))
			rpn_size++;
		
		if ((inter_output[inter_output.size()-1].id >= 20 && inter_output[inter_output.size()-1].id <= 23) || (inter_output[inter_output.size()-1].id >= 8 && inter_output[inter_output.size()-1].id <= 19))
			rpn_size--;
	}

	if (statment_stack.size())
	{
		std::cout << "Expected all statments to be closed before the end of the file.\n"; 
		exit(-1);
	}

	#if DEBUG
		for (inter _inter : inter_output)
			print_inter(_inter);
	#endif

	calc_function_stack_size();

	return inter_output;
}
