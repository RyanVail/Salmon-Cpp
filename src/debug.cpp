/* This file has all of the debug things */

#include<debug.hpp>
#include<string>
#include<vartypes.hpp>
#include<intermediate/intermediate.hpp>

const std::string INTERS[] = 
{ "VARIABLE_TYPE", "VARIABLE_DECLERATION", "VARIABLE_REASIGNMENT",
"VARIABLE_ACCESS", "NOT", "GET", "MEM_ADDRS", "INCRAMENT", "DECRAMENT", "AND",
"OR", "XOR", "LSL", "LSR", "EQUAL", "IS_EQUAL", "LESS", "LESS_EQUAL",
"GREATER", "GREATER_EQUAL", "ADD", "SUB", "DIV", "MUL", "FUNC_BEGIN",
"FUNC_END", "FUNC_CALL", "IF_BEGIN", "IF_END", "VARIABLE_NAME", "RESET_RPN",
"CONST", "WHILE_BEGIN", "WHILE_END", "ELSE_BEGIN", "ELSE_END", "CONTINUE",
"BREAK", "RETURN", "ASM" };

// This takes in a intermediate id and prints out its data
// TODO: This should use the local variable
void print_inter(inter to_print)
{
	std::cout << INTERS[to_print.id];
	if (to_print.value)
		std::cout << " : " << to_print.value;
	if (to_print.func)
		std::cout << " : " << to_print.func->name;
	if (to_print.var)
		std::cout << " : " << to_print.var->name;
	std::cout << "\n";
}

// This prints the variables and function in the symbol table
void print_symbol_table()
{
	std::cout << "VARIABLE SYMBOL TABLE CONTENTS:\n";

	for (variable_token current_variable : symbol_table.variables)
		std::cout << "|name| " << current_variable.name 
		<< " |type| " << id_into_string(current_variable.type) 
		<< " |stack location| " << current_variable.stack_location 
		<< " |owner| " << current_variable.owner << "\n";

	std::cout << "FUNCTION SYMBOL TABLE CONTENTS:\n";

	for (function_token current_function: symbol_table.functions)
		std::cout << "|name| " << current_function.name
		<< " |stack space needed| " << current_function.stack_space_needed
		<< " |id| " << current_function.id << "\n";
}
