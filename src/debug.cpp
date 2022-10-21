/* This file has all of the debug things */

#pragma once

const std::string INTERS[] = { "VARIABLE_TYPE", "VARIABLE_DECLERATION", "VARIABLE_REASIGNMENT", "VARIABLE_ACCESS", "NOT", "GET", "MEM_ADDRS", "INCRAMENT", "DECRAMENT", "AND", "OR", "XOR", "LSL", "LSR", "EQUAL", "IS_EQUAL", "LESS", "LESS_EQUAL", "GREATER", "GREATER_EQUAL", "ADD", "SUB", "DIV", "MUL", "FUNC_BEGIN", "FUNC_END", "FUNC_CALL", "IF_BEGIN", "IF_END", "VARIABLE_NAME", "RESET_RPN", "CONST", "WHILE_BEGIN", "WHILE_END", "ASM" };

// TODO: This should just print the string in a vector at the index
// This takes in a intermediate id and prints the id
void print_inter(inter to_print)
{
	std::cout << INTERS[to_print.id];
	if (to_print.value) { std::cout << " : " << to_print.value; } 
	if (to_print.refrenced_name != "") { std::cout << " : " << to_print.refrenced_name; }
	std::cout << "\n";
}

// This prints the variables and function in the symbol table
void print_symbol_table()
{
	std::cout << "VARIABLE SYMBOL TABLE CONTENTS:\n";
	for (std::vector<variable_token>::iterator itr = symbol_table.variables.begin(); itr != symbol_table.variables.end(); itr++)
	{
		std::cout << "|name| " << itr->name << " |type| " << id_into_string(itr->type) << " |stack location| " << itr->stack_location << " |owner| " << itr->owner << "\n";
	}
	std::cout << "FUNCTION SYMBOL TABLE CONTENTS:\n";
	for (std::vector<function_token>::iterator itr = symbol_table.functions.begin(); itr != symbol_table.functions.end(); itr++)
	{
		std::cout << "|name| " << itr->name << " |stack space needed| " << itr->stack_space_needed << " |id| " << itr->id << "\n";
	}
}
