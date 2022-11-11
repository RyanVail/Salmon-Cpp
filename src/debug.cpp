/* This file has all of the debug things */

#include<debug.hpp>
#include<string>
#include<vartypes.hpp>
#include<intermediate/intermediate.hpp>

const std::string INTERS[] = 
{ "TYPE", "VARIABLE_DECLERATION", "VARIABLE_REASIGNMENT",
"VARIABLE_ACCESS", "NOT", "GET", "MEM_ADDRS", "INCRAMENT", "DECRAMENT", "AND",
"OR", "XOR", "LSL", "LSR", "EQUAL", "IS_EQUAL", "LESS", "LESS_EQUAL",
"GREATER", "GREATER_EQUAL", "ADD", "SUB", "DIV", "MUL", "FUNC_BEGIN",
"FUNC_END", "FUNC_CALL", "IF_BEGIN", "IF_END", "VARIABLE_NAME", "RESET_RPN",
"CONST", "WHILE_BEGIN", "WHILE_END", "ELSE_BEGIN", "ELSE_END", "CONTINUE",
"BREAK", "RETURN", "ASM" };
const std::vector<std::string> types = { "null", "bool", "ibool", "u8", "i8",
"u16", "i16", "u32", "i32", "u64", "i64", "u128", "i128" };

// This takes in a intermediate id and prints out its data
// TODO: This should use the local variables
void print_inter(inter to_print)
{
	std::cout << INTERS[to_print.id];
	switch (to_print.id)
	{
	case (VARIABLE_TYPE):
		std::cout << " : " << types[to_print.type];
		break;
	case (VARIABLE_DECLERATION):
	case (VARIABLE_REASIGNMENT):
	case (VARIABLE_ACCESS):
	case (VARIABLE_NAME):
		std::cout << " : " << to_print.get_var()->name;
		break;
	case (CONST):
		std::cout << " : " << to_print.value;
		break;
	case (FUNC_BEGIN):
	case (FUNC_END):
		std::cout << " : " << to_print.get_func()->name;
	}
	std::cout << "\n";
}

// This prints the variables and function in the symbol table
void print_symbol_table()
{
	std::cout << "VARIABLE SYMBOL TABLE CONTENTS:\n";

	symbols _symbol_table = *get_symbol_table();

	for (variable_token current_variable : _symbol_table.variables)
		std::cout << "|name| " << current_variable.name 
		<< " |type| " << id_into_string(current_variable.type) 
		<< " |stack location| " << current_variable.stack_location 
		<< " |owner| " << current_variable.owner << "\n";

	std::cout << "FUNCTION SYMBOL TABLE CONTENTS:\n";

	for (function_token current_function: _symbol_table.functions)
		std::cout << "|name| " << current_function.name
		<< " |stack space needed| " << current_function.stack_space_needed
		<< " |id| " << current_function.id << "\n";
}
