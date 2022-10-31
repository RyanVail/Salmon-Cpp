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
#include<error.hpp>

static std::stack<operand::operand_def> operand_stack;
static std::stack<inter> statment_stack;
static std::vector<inter> inter_output;
static i32 current_owner;
static i32 current_stack;

// Allows postprocessor instructions to add to the static inter_output
void postprocessor_add_inter(inter _inter)
{
	inter_output.push_back(_inter);
}

// These are inline functions that improve readability a bit by abstraction
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

// There should be another "add_inter" function that takes in an inter that is used when we offload statment stack
inline void add_inter(i32 _const, variable_token *_var, function_token *_func)
{
	inter_output.push_back(inter(_const, _var, _func));
}

inline void add_statment(u32 _const, variable_token *_var, function_token *_func)
{
	statment_stack.push(inter(_const, _var, _func));
}

// This takes the top operand off and generates its intermediate repersentation
void top_operand_to_inter()
{
	operand::operand_def top_operand = operand_stack.top()

	u8 _type = top_opearnd.refrenced_name[0];
	
	if (!top_operand.accessed_variable && !top_operand.called_function)
		add_inter(CONST, top_operand.const_value, _type);

	else if (top_opearnd.accessed_variable != nullptr)
		add_inter(VARIABLE_ACCESS, top_operand.accessed_variable, "");
}

// This takes the values needed for a function call off the operand stack
void function_call_inter_prep(function_token &_fn)
{	
	if (operand_stack.size() < _fn.inputs.size())
		error::send_error("Function call doesn't contain enough inputs.\n");

	for (u32 i=0; i < _fn.inputs.size(); i++)
	{
		u8 _can_be = typechecker::can_be_converted_to(operand.top().final_type, _fn.inputs[i]);
		if (_can_be)
		{
			std::cout << "Cannot ";

			if (can_be_transformed_into(value_in_r0.final_type, current_variable.type) == 0)
				std::cout << "explicitly ";

			error::send_error("transform type " + id_into_string(rpn_stack.top().final_type) + " into " + id_into_string(current_variable.type));
		}
		top_operand_to_inter();
	}

	add_inter(FUNC_CALL, _fn.id, *itr);
}

// This function turns single char operations into their intermediate form
// Returns true if it added something
inline bool single_char_operator_into_inter(i8 operator_char)
{
	switch(operator_char)
	{
	case '!':
		add_inter(NOT, 0, "");
		break;
	case '@':
		add_inter(GET, 0, "");
		break;
	case '%':
		add_inter(MEM_ADDRS, 0, "");
		break;
	case '&':
		add_inter(AND, 0, "");
		break;
	case '|':
		add_inter(OR, 0, "");
		break;
	case '^':
		add_inter(XOR, 0, "");
		break;
	case '=':
		add_inter(EQUAL, 0, "");
		break;
	case '<':
		add_inter(LESS, 0, "");
		break;
	case '>':
		add_inter(GREATER, 0, "");
		break;
	case '+':
		add_inter(ADD, 0, "");
		break;
	case '-':
		add_inter(SUB, 0, "");
		break;
	case '/':
		add_inter(DIV, 0, "");
		break;
	case '*':
		add_inter(MUL, 0, "");
		break;
	default:
		return false;
	}
	return true;
}

// This handles turning operators into their intermediate repersentation
// Returns true if it added something
inline bool operator_into_inter(std::string *token_itr)
{
	if (single_char_operator_into_inter((*token_itr)[0]))
		return true;

	u32 tmp = inter_output.size();

	// Simple string operators
	if (*token_itr == "<<")
		add_inter(LSL, 0, "");
	if (*token_itr == ">>")
		add_inter(LSR, 0, "");
	if (*token_itr == "==")
		add_inter(IS_EQUAL, 0, "");
	if (*token_itr == "<=")
		add_inter(LESS_EQUAL, 0, "");
	if (*token_itr == "++")
		add_inter(INCRAMENT, 0, "");
	if (*token_itr == "--")
		add_inter(DECRAMENT, 0, "");
	if (*token_itr == "return")
		add_inter(RETURN, 0, "");
	if (*token_itr == "break")
		add_inter(BREAK, 0, "");
	if (*token_itr == "continue")
		add_inter(CONTINUE, 0, "");
	if (*token_itr == ">=")
		add_inter(GREATER_EQUAL, 0, "");
`
	// Complex operators
	if (is_type(*token_itr) != -1)
		add_inter(VARIABLE_TYPE, 0, *itr);

	// TODO: Test if this still works
	if (*token_itr == "#")
		postprocessor::process_instruction(file, token_itr, inter_output);

	if (*token_itr == "$")
	{
		// itr is over the function name now
		itr++;

		if (!get_function_token(*itr))
			error::send_error("Unknown function: " + *itr + ".\n");
		
		function_call_inter_prep(*get_function_token(*itr));

		// Skips the next '$' if there is one
		if (*(token_itr+1) == "$")
			token_itr += 1;

		add_function_call_operand(get_function_token(*itr));

		continue;
	}

	if (*token_itr == "}")
	{
		if (rpn_size && statment_stack.top().id != WHILE_END)
			error::send_error("RPN stack size should be zero before ending a statment.\n");
		if (rpn_size >= 2) 
			error::send_error("Expected one thing operand after ending a while statment but got: " + std::to_string(operand_stack.size()) + ".\n");
		if (statment_stack.empty())
			error::send_error("Unexpected '}'.\n");

		inter_output.push_back(statment_stack.top());
		statment_stack.pop();
		continue;
	}

	if (*token_itr == ";")
	{
		add_inter(RESET_RPN, 0, "");
		operand_stack = {};
	}

	if (is_str_num(*token_itr))
		u8 type = 7;
		if (inter_output[inter_output.size()-1].id == VARIABLE_TYPE)
		{
			type = inter_output[inter_output.size()-1].type;
		}
		add_const_operand(type, get_str_num(*token_itr));
	
	if (inter_output.size() == tmp)
		return false;

	return true;
}

inline void function_call_into_inter(std::string *token_itr)
{
	if(current_owner != -1) 
		error::send_error("Function defintion inside another function is not valid.\n");
	if(rpn_size)
		error::send_error("Expected the nothing on the RPN stack before a function definiton.\n");

	token_itr++;
	std::string name = (*token_itr).substr(0, (*token_itr).find('(') );

	if(!is_str_letters(name)) 
		error::send_error("The function name: " + name + " contains invalid characters.\n");
	if(get_function_token(name)) 
		error::send_error("The function name: " + name + " is already used.\n");
	if(!is_valid_name(name))
		error::send_error("The function name: " + name + " isn't valid.\n");

	add_function_token(name);
	symbol_table.functions[symbol_table.functions.size()-1].stack_space_needed = 0;
	// This reads through the input variables of the function
	u8 current_type = 0; // The type of the current variable
	i32 stack_space_needed = 0; // The stack space needed for all the input variables to the function
	// TODO: Input variables shouldn't use stack space by default, registers would be a lot better
	while (true)
	{
		if (token_itr == file.end())
			error::send_error("Error while parsing function defintion: reached end of file.\n");

		std::string token = *token_itr;

		if (token == ",") 
		{ 
			token_itr++; 
			continue;
		}

		if (token == ")")
			break;

		// TODO: Add default varaible assignment
		if (token == "=")
			error::send_error("Default function input variable assignemt has not yet been added.\n");

		if (current_type)
		{
			// This checks if there is another variable with the same name
			// TODO: Each function should have its own list of local and global vars rather than just every var. Add a function called get_local_variable_token which just checks for variables in this owner and global owner
			// TODO: This should check if the variable name is valid
			if (get_variable_token(token)) 
				error::send_error("A variable with that name already exists.\n");

			stack_space_needed -= types_size[current_type];
			add_variable_token(token, current_type, symbol_table.functions.size()-1, stack_space_needed);
			current_type = 0;
		}
		// If this variable is the type
		else
			current_type = into_id(token);

		token_itr++;
	}

	token_itr += 1;
	add_inter(FUNC_BEGIN, 0, name);
	add_statment(FUNC_END, 0, name);
	#if DEBUG
		std::cout << *token_itr << "\n"; // TODO: REMOVE THIS TEST LINE!
	#endif

	if (*(token_itr) != "{")
		error::send_error("Expected '{' after a function defintion.\n");
}

// Returns true if it added something
inline bool statment_into_inter(std::String *token_itr)
{
	if (*token_itr == "if")
	{
		add_inter(IF_BEGIN, 0, "");
		add_statmen(IF_END, 0, "");
		token_itr++;
		if (*(token_itr) != "{") 
			error::send_error("Expected '{' after an if statment.\n");
		rpn_size--;
		return true;
	}
	if (*token_itr == "else")
	{
		if (inter_output[inter_output.size()-1].id != IF_END)
			error::send_error("Expected an end to an if statment before an else statment.\n");
		
		token_itr++;
		
		if (*(token_itr) != "{" && *(token_itr) != "if")
			error::send_error("Expected '{' or if statment after else statment.\n");

		add_inter(ELSE_BEGIN, 0, "");
		add_statment(ELSE_END, 0, "");
		return true;
	}
	if (*token_itr == "while")
	{
		add_inter(WHILE_BEGIN, 0, "");
		add_statment(WHILE_END, 0, "");
		token_itr++;

		if (*(token_itr) != "{") 
			error::send_error("Expected '{' after a while statment.\n");

		rpn_size--;
		return true;
	}
	if (is_str_letters(*token_itr))
	{
		// TODO: Fix this ugly mess!
		// TODO: If the variable is in the global scope we need to add it to .data
		// If the last intermediate token was a type we are defining a variable
		// Otherwise we check the symbol table and assume we are accessing a variable
		if (inter_output.size() && inter_output[inter_output.size()-1].id == VARIABLE_TYPE)
		{
			if (rpn_size)
				error::send_error("There must be no operands before defining a variable.\n");

			current_stack -= types_size[into_id(inter_output[inter_output.size()-1].refrenced_name)];

			//add_variable_token(*token_itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);
			add_variable_token(*token_itr, into_id(inter_output[inter_output.size()-1].refrenced_name),current_owner,current_stack);

			add_inter(VARIABLE_DECLERATION, 0, *token_itr, get_variable_token(*token_itr));
		}

		// We check if the variable is in the symbol table, if not we send an error
		if (!get_variable_token(*token_itr))
			error::send_error("Unknown variable: " + *token_itr + ".\n");

		add_inter(VARIABLE_ACCESS, 0, *token_itr, get_variable_token(*token_itr));
		return true;
	}
	if (*token_itr == "fn")
	{
		function_call_into_inter(token_itr);
		return true;
	}

	return false;
}

// This turns a file into a series of inters
std::vector<inter> file_into_inter(std::vector<std::string> file)
{
	// TODO: All errors should show what they got instead of just what they expected
	current_owner = -1;
	for (std::vector<std::string>::iterator itr = file.begin(); itr != file.end(); itr++)
	{
		if (operator_into_inter(itr))
			continue;

		if (statment_into_inter(itr))
			continue;

		error::send_error("Unknown token: " + *itr + ".\n");
	}

	if (statment_stack.size())
		error::send_error("Expected all statments to be closed before the end of the file.\n");

	#if DEBUG
		for (inter _inter : inter_output)
			print_inter(_inter);
	#endif

	calc_function_stack_size();

	return inter_output;
}
