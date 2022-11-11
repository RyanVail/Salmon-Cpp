/* This file holds the symbol table and some useful function that involve symbols */

#include<symboltable.hpp>
#include<string>
#include<vector>
#include<iostream>
#include<vartypes.hpp>
#include<error.hpp>

#if DEBUG
	symbols* get_symbol_table()
	{
		return &symbol_table;
	}
#endif

// This returns a pointer to the variable token by the same name/id or a null
// pointer.
variable_token* get_variable_token(std::string name, i32 id)
{
	for (variable_token &current_variable : symbol_table.variables)
	{
		if (name == "" && current_variable.id == id)
			return &current_variable;
		if (name != "" && current_variable.name == name)
			return &current_variable;
	}

	return nullptr;
}

// This calculates the value that will be subtraced from the stack pointer 
// during each function and the global scope.
void calc_function_stack_size()
{
	// Calculates all function's needed stack spaces
	for (function_token &current_function : symbol_table.functions)
		for (variable_token &current_variable : current_function.inputs)
			current_function.stack_space_needed += types_size[current_variable.type];

	// Calculates global stack's size
	for (variable_token &current_variable : symbol_table.variables)
		if (current_variable.owner == -1)
			symbol_table.stack_space_needed += types_size[current_variable.type];
}

// This returns a pointer to the variable token by the same name/id in the same
// scope. 
// TODO: This only checks owners not scope. Allow this to check scope
variable_token* get_local_variable_token(i32 owner, std::string name, i32 id)
{
	for (variable_token &current_variable : symbol_table.variables)
	{
		if (id)
			// If we have the same id as the variable we are checking and the 
			// variable is in this scope or global scope.
			if (current_variable.id == id && (current_variable.owner == owner || current_variable.owner == -1))
				return &current_variable;
		else
			// If we have the same name as the variable we are checking and 
			// the variable is in this scope or global scope.
			if (current_variable.name == name && (current_variable.owner == owner || current_variable.owner == -1))
				return &current_variable;
	}

	return nullptr;
}

// This returns a pointer to the function token by the same name/id or a null
// pointer.
function_token* get_function_token(std::string name, i32 id)
{
	for (function_token &current_function : symbol_table.functions)
	{
		if (id && current_function.id == id)
			return &current_function;
		else if (current_function.name == name)
			return &current_function;
	}

	return nullptr;
}

// This adds a variable to the list of variables based on names and types.
void add_variable_token(std::string name, u8 type, i32 owner, i32 stack_location)
{
	// TODO: This should make sure it is a valid variable name
	// TODO: This should check local variable
	if (get_variable_token(name) != 0)
		error::send_error("The variable name: " + name + " is already used.");

	variable_token new_token = variable_token(name, type, owner, stack_location);
	new_token.id = symbol_table.variables.size();
	symbol_table.variables.push_back(new_token);
}

// TODO: This is used once by "aarch32_asm.cpp" and it just gets the global 
// stack space needed.
// This returns the stack space needed for the specified function id or global
// scope.
i32 get_stack_space_needed(i32 id)
{
	if (id == -1)
		return symbol_table.stack_space_needed;

	return get_function_token("", id)->stack_space_needed;
}

// This adds a function to the list of functions based on name, 
// returns, and inputs.
void add_function_token(std::string _name, u8 _output, std::vector<variable_token> _inputs)
{
	if (!is_valid_name(_name))
		error::send_error("The function name: " + _name + "is not valid.");
	if (get_function_token(_name) != 0)
		error::send_error("The function name: " + _name + " is already used.");

	function_token _new_function;
	_new_function.name = _name;
	_new_function.inputs = _inputs;
	_new_function.output = _output;

	// This sets the id to one more than the last function in symbol_table. If there is no function before this we set the id to 0
	if (symbol_table.functions.size())
		_new_function.id = symbol_table.functions[symbol_table.functions.size()-1].id + 1;
	else
		_new_function.id = 0;

	symbol_table.functions.push_back(_new_function);
}
