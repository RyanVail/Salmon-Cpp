/* This file holds the symbol table and some useful function that involve symbols */

#include<symbol_table.hpp>
#include<string>
#include<vector>
#include<iostream>
#include<var_types.hpp>

// This returns a pointer to the variable token by the same name/id or a null pointer
variable_token* get_variable_token(std::string name, int id)
{
	for (std::vector<variable_token>::iterator itr = symbol_table.variables.begin(); itr != symbol_table.variables.end(); itr++)
	{
		if (id) { if (itr->id == id) { return &*itr; } }
		else { if (itr->name == name) { return &*itr; } }
	}
	return 0;
}

// This calculates the value that will be subtraced from the stack pointer during each function and the global scope
void calc_function_stack_size()
{
	// Calculates all function's needed stack spaces
	for (function_token current_function : symbol_table.functions)
	{
		for (variable_token current_variable : current_function.inputs)
		{
			current_function.stack_space_needed += types_size[current_variable.type];
		}
	}
	// Calculates global stack's size
	for (variable_token current_variable : symbol_table.variables)
	{
		if (current_variable.owner == -1)
		{
			symbol_table.stack_space_needed += types_size[current_variable.type];
		}
	}
}

// This returns a pointer to the variable token by the same name/id in the same scope
// TODO: This only checks owners not scope. Allow this to check scope
variable_token* get_local_variable_token(int owner_to_check, std::string name, int id)
{
	for (std::vector<variable_token>::iterator itr = symbol_table.variables.begin(); itr != symbol_table.variables.end(); itr++)
	{
		if (id)
		{
			// If we have the same id as the variable we are checking and the variable is in this scope or global scope
			if (itr->id == id && (itr->owner == owner_to_check || itr->owner == -1)) { return &*itr; }
		}
		else
		{
			if (itr->name == name && (itr->owner == owner_to_check || itr->owner == -1)) { return &*itr; }
		}
	}
	return 0;
}

// This returns a pointer to the function token by the same name/id or a null pointer
function_token* get_function_token(std::string name, int id)
{
	for (std::vector<function_token>::iterator itr = symbol_table.functions.begin(); itr != symbol_table.functions.end(); itr++)
	{
		if (id) { if (itr->id == id) { return &*itr; } }
		else { if (itr->name == name) { return &*itr; } }
	}
	return 0;
}

// This adds a variable to the list of variables based on names and types
void add_variable_token(std::string name, unsigned char type, int owner, int stack_location)
{
	// This should make sure it is a valid variable name
	if (get_variable_token(name) != 0) { std::cout << "The variable name: " << name << " is already used."; exit(-1); }
	variable_token new_token = variable_token(name, type, owner, stack_location);
	symbol_table.variables.push_back(new_token);
}

// This returns the stack space needed for the specified function id or global scope
int get_stack_space_needed(int id)
{
	if (id == -1) { return symbol_table.stack_space_needed; }
	return get_function_token("", id)->stack_space_needed;
}


// This adds a function to the list of functions based on name, returns, and inputs
void add_function_token(std::string name)
{
	// This should make sure it is a valid token name
	if (get_function_token(name) != 0) { std::cout << "The function name: " << name << " is already used."; exit(-1); }
	function_token new_function;
	new_function.name = name;
	// This sets the id to one more than the last function in symbol_table
	if (symbol_table.functions.size()) { new_function.id = symbol_table.functions[symbol_table.functions.size()-1].id + 1; }
	else { new_function.id = 0; }
	symbol_table.functions.push_back(new_function);
}
