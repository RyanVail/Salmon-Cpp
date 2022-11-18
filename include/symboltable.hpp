/* This file holds the symbol table and some useful function that involve symbols */

#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include<main.hpp>
#include<string>
#include<vector>
#include<iostream>
#include<vartypes.hpp>
#include<typedefs.hpp>

struct symbols;
struct variable_token;
struct function_token;

// TODO: Variable and function ids are signed because we pass and return -1
// but -1 is the only negative that is every used so just make it unsigned and
// use U32_MAX.

// This is going to be used once by a static symbol table
struct symbols
{
	std::vector<variable_token> variables;
	std::vector<function_token> functions;
	i32 stack_space_needed; // The amount of stack space needed for static variables
};

struct variable_token // TODO: Variables need to be able to change type during compilation and we should be able to track scope
{
	std::string name;
	i32 id;
	u8 type = 0;
	i32 owner = -1; // -1 means global variable 0 < means it's the id of a function
	i32 stack_location; // The negative location on the stack from the current point
	variable_token(std::string _name, u8 _type, i32 _owner, i32 _stack_location)
	: name(_name), type(_type), owner(_owner), stack_location(_stack_location) {};
	// This constructor is only used for function input variable definition
	variable_token(u8 _type) : type(_type) {};
};

struct function_token
{
	std::string name;
	i32 id;
	std::vector<variable_token*> inputs; // This is a vector of the variables the function takes as an input
	u8 output; // This is the output type
	i32 stack_space_needed = 0; // The amount of bytes needed of stack space
};

// The static symbol table
static symbols symbol_table;

#if DEBUG
	symbols* get_symbol_table();
#endif

// This returns a pointer to the variable token by the same name/id or a null pointer
variable_token* get_variable_token(std::string name, i32 id = 0);

// This calculates the value that will be subtraced from the stack pointer during each function and the global scope
void calc_function_stack_size();

// This returns a pointer to the variable token by the same name/id in the same scope
// TODO: This only checks owners not scope. Allow this to check scope
variable_token* get_local_variable_token(i32 owner, std::string name, i32 id = 0);

// This returns a pointer to the function token by the same name/id or a null pointer
function_token* get_function_token(std::string name, i32 id = 0);

// This adds a variable to the list of variables based on names and types
void add_variable_token(std::string name, u8 type, i32 owner, i32 stack_location);

// This adds a function to the list of functions based on name, returns, and inputs
void add_function_token(std::string _name, u8 _output, std::vector<variable_token*> _inputs);

// This returns the stack space needed for the specified function id or global scope
i32 get_stack_space_needed(i32 id);

#endif
