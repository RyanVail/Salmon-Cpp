/* This file holds the symbol table and some useful function that involve symbols - Ryan Vail Sep. 5th 2022 */

#pragma once

#include<vector>
#include<iostream>

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

struct variable_token
{
	std::string name;
	int id;
	unsigned char type = 0;
	int owner = -1; // -1 means global variable 0 < means it's the id of a function
	// TODO: These should have an owner so we know if it's in scope or not
	int stack_location; // The negative location on the stack from the current point
};

struct function_token
{
	std::string name;
	int id;
	std::vector<variable_token> inputs; // This is a vector of the variables the function takes as an input
	unsigned char output; // This is the output type
	int stack_space_needed; // The amount of bytes needed of stack space
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
	// TODO: Replace this with the fancy new way of declaring a struct
	variable_token new_token;
	new_token.name = name;
	new_token.type = type;
	new_token.owner = owner;
	new_token.stack_location = stack_location;
	symbol_table.variables.push_back(new_token);
}
// This adds a function to the list of functions based on name, returns, and inputs
void add_function_token(std::string name)
{
	// This should make sure it is a valid token name
	if (get_function_token(name) != 0) { std::cout << "The function name: " << name << " is already used."; exit(-1); }
	// TODO: Replace this with the fancy new way of declaring a struct
	function_token new_function;
	new_function.name = name;
	// This sets the id to one more than the last function in symbol_table
	if (symbol_table.functions.size()) { new_function.id = symbol_table.functions[symbol_table.functions.size()-1].id + 1; }
	else { new_function.id = 0; }
	symbol_table.functions.push_back(new_function);
}
