#ifndef INTERMEDIATE_INTERMEDIATE_HPP
#define INTERMEDIATE_INTERMEDIATE_HPP

#include<vector>
#include<string>
#include<symboltable.hpp>
#include<typedefs.hpp>

// This struct is a value definition

// This struct defines a single intermediate instruction
struct inter
{
	i32 id; // This is the id/type of the intermediate token
	i32 value; // This is a value or variable type id
	std::string refrenced_name; // This is the name of the function or variable that is refrenced
	variable_token* refrenced_variable_token; // The refrenced variable token if there is one
	inter(u8 id_value, i32 value_value, std::string refrenced_name_value) : id(id_value), value(value_value), refrenced_name(refrenced_name_value) {};
	inter(u8 id_value, i32 value_value, std::string refrenced_name_value, variable_token* refrenced_variable_token_value) : id(id_value), value(value_value), refrenced_name(refrenced_name_value), refrenced_variable_token(refrenced_variable_token_value) {};
};

void postprocessor_add_inter(inter _inter);

// TODO: "VARIABLE_TYPE" should be called "TYPE"
// I know that using #defines may be clunky and result in all of unintential 
// bugs, but I think it's preferable to enums because the code is clearner.
// Excluding the #undefs.
//
// The below comments were made with a tab size of four
// These are the intermediate instructions types/ids
#define VARIABLE_TYPE 0
#define VARIABLE_DECLERATION 1
#define VARIABLE_REASIGNMENT 2 // TODO: This is currently not used since it is done during the transfer to ASM but once opt. is added it will be needed
#define VARIABLE_ACCESS 3   // TODO: Variable accesses should have their own struct so we can also define []s
#define NOT 4				// '!'
#define GET 5				// '@'
#define MEM_ADDRS 6			// '%'
#define INCRAMENT 7			// "++"
#define DECRAMENT 8			// "--"
#define AND 9				// '&'
#define OR 10				// '|'
#define XOR 11				// '^'
#define LSL 12				// "<<"
#define LSR 13				// ">>"
#define EQUAL 14			// '='
#define IS_EQUAL 15			// "=="
#define LESS 16				// '<'
#define LESS_EQUAL 17		// "<="
#define GREATER 18			// '>'
#define GREATER_EQUAL 19	// ">="
#define ADD 20				// '+'
#define	SUB 21				// '-'
#define DIV 22				// '/'
#define MUL 23		
#define FUNC_BEGIN 24
#define FUNC_END 25
#define FUNC_CALL 26		// '$'
#define IF_BEGIN 27
#define IF_END 28
#define VARIABLE_NAME 29
#define RESET_RPN 30		// ';'
#define CONST 31
#define WHILE_BEGIN 32
#define WHILE_END 33
#define ELSE_BEGIN 34
#define ELSE_END 35
#define CONTINUE 36
#define BREAK 37
#define RETURN 38
#define ASM 39

// This takes in the tokenized file and returns the intermediate representation of the file
std::vector<inter> file_into_inter(std::vector<std::string> file);

#endif