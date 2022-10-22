/* This file holds the symbol table and some useful function that involve symbols - Ryan Vail Sep. 5th 2022 */

#pragma once

#include<vector>
#include<iostream>
#include"var_types.cpp"

// Forward struct defintions
struct variable_token;
struct function_token;
struct symbols;

struct symbols
{
	std::vector<variable_token> variables;
	std::vector<function_token> functions;
	int stack_space_needed; // The amount of stack space needed for static variables
};

struct variable_token // TODO: Variables need to be able to change type during compilation and we should be able to track scope
{
	std::string name;
	int id;
	unsigned char type = 0;
	int owner = -1; // -1 means global variable 0 < means it's the id of a function
	int stack_location; // The negative location on the stack from the current point
	variable_token(_name, _type, _owner, _stack_location) : name(_name), type(_type), owner(_owner), stack_location(_stack_location) {};
};

struct function_token
{
	std::string name;
	int id;
	std::vector<variable_token> inputs; // This is a vector of the variables the function takes as an input
	unsigned char output; // This is the output type
	int stack_space_needed = 0; // The amount of bytes needed of stack space
	variable_token new_token = variable_token(name, type, owner, stack_location);
};

// The static symbol table
symbols symbol_table;

// This returns a pointer to the variable token by the same name/id or a null pointer
variable_token* get_variable_token(std::string name, int id = 0)
{
	for (std::vector<variable_token>::iterator itr = symbol_table.variables.begin(); itr != symbol_table.variables.end(); itr++)
	{
		if (id) { if (itr->id == id) { return &*itr; } }
		else { if (itr->name == name) { return &*itr; } }
	}
	return 0;
}

// This calculates the value that will be subtraced from the stack pointer
void compute_function_stack_size()
{
	for (function_token current_function : symbol_table.functions)
	{
		for (variable_token current_variable : current_function.inputs)
		{
			current_function.stack_space_needed += types_size[current_variable.type];
		}
	}
}


// This returns a pointer to the variable token by the same name/id in the same scope
// TODO: This only checks owners not scope. Allow this to check scope
variable_token* get_local_variable_token(int owner_to_check, std::string name, int id = 0)
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
function_token* get_function_token(std::string name, int id = 0)
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
