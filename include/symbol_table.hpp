/* This file holds the symbol table and some useful function that involve symbols */

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include<string>
#include<vector>
#include<iostream>
#include<var_types.hpp>

struct symbols;
struct variable_token;
struct function_token;

// This is going to be used once by a static symbol table
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
	variable_token(std::string _name, unsigned char _type, int _owner, int _stack_location) : name(_name), type(_type), owner(_owner), stack_location(_stack_location) {};
};

struct function_token
{
	std::string name;
	int id;
	std::vector<variable_token> inputs; // This is a vector of the variables the function takes as an input
	unsigned char output; // This is the output type
	int stack_space_needed = 0; // The amount of bytes needed of stack space
};

// The static symbol table
static symbols symbol_table;

// This returns a pointer to the variable token by the same name/id or a null pointer
variable_token* get_variable_token(std::string name, int id = 0);

// This calculates the value that will be subtraced from the stack pointer during each function and the global scope
void calc_function_stack_size();

// This returns a pointer to the variable token by the same name/id in the same scope
// TODO: This only checks owners not scope. Allow this to check scope
variable_token* get_local_variable_token(int owner_to_check, std::string name, int id = 0);

// This returns a pointer to the function token by the same name/id or a null pointer
function_token* get_function_token(std::string name, int id = 0);

// This adds a variable to the list of variables based on names and types
void add_variable_token(std::string name, unsigned char type, int owner, int stack_location);

// This adds a function to the list of functions based on name, returns, and inputs
void add_function_token(std::string name);

// This returns the stack space needed for the specified function id or global scope
int get_stack_space_needed(int id);

#endif