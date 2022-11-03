/* This file handles turning string based variable types into their corrisponding id */

#include<vartypes.hpp>
#include<string>
#include<vector>
#include<math.h> // This is just used for pow it might be better to rewrite it

// 0 is null type
const std::vector<std::string> types = { "null", "bool", "ibool", "u8", "i8", "u16", "i16", "u32", "i32", "u64", "i64", "u128", "i128" };
#define TYPES_SIZE 13
const std::vector<std::string> invalid_names = { "if", "while", "else", "continue", "break", "return" };
#define INVALID_NAMES_SIZE 6

// To be
// const i32 types[] = { " ", "u32", "u16", "u8", "i32", "i16", "i8" };
// const i32 types_size[] = { 0, 4, 2, 1, 4, 2, 1 }; // The size in bytes each type takes up
// #define TYPES_SIZE 7


// This checks if a type is normal IE. 32 bit
bool is_normal(u8 type)
{
    if (type)
		return true;

    return false;
};

// This checks if a type is a char IE. 8 bit
bool is_char(u8 type)
{
	if (type)
		return true;

	return false;
}

// This takes in the two types and says how they can be transformed
// 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
u8 can_be_transformed_into(u8 type0, u8 type1)
{
	// TODO: Add some logic
	return 0;
}

// This checks if the given string is a valid name
bool is_valid_name(std::string name)
{
	for (i32 i=0; i < INVALID_NAMES_SIZE; i++)
		if (name == invalid_names[i])
			return false;

	for (i32 i=0; i < TYPES_SIZE; i++)
		if (name == types[i])
			return false;

	return true;
}

// This returns the type id of the inputed string
i32 get_type(std::string possible_type)
{
	for (i32 i=0; i < TYPES_SIZE; i++)
		if (types[i] == possible_type)
			return i;

	return -1; // TODO: This should return 0 like the function bellow and all calls of this function need to be fixed after that
}

// This either returns the variable id or 0
i32 into_id(std::string &string_type)
{
	for (i32 i=1; i < TYPES_SIZE; i++)
		if (types[i] == string_type)
			return i;

	return 0;
}

// This turns a variable id into it's string counterpart or NULL if it isn't valid
std::string id_into_string(i32 var_id)
{
	if (var_id >= TYPES_SIZE || var_id <= 0)
		return NULL;

	return types[var_id];
}

// TODO: This should check if it's negative
// Checks if a string is numbers
bool is_str_num(std::string &possible_number)
{
	for (std::string::iterator itr = possible_number.begin(); itr != possible_number.end(); itr++)
		if (*itr < 48 || *itr > 57)
			return false;

	return true;
}

// This returns the i32 of a string of numbers
// TODO: Check if "stoi" can do this
i32 get_str_num(std::string &possible_number)
{
	i32 value = 0;
	for (i32 i=0; i < possible_number.size(); i++)
	{
		if (i == 0 && possible_number[i] == '-')
			continue;
		value += (possible_number[i] - 48) * pow(10, -i + possible_number.size() - 1);
	}
	
	if (possible_number[0] == '-')
		value = -value;

	return value;
}

// Checks if a string is just letters or '_'s
bool is_str_letters(std::string &string_to_test)
{
	for (i8 current_char : string_to_test)
		if (((current_char > 122 || current_char < 97) && (current_char > 90 || current_char < 65)) && current_char != '_')
			return false;

	return true;
}

#undef INVALID_NAMES_SIZE
#undef TYPES_SIZE