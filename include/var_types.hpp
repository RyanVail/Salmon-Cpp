/* This file handles turning string based variable types into their corrisponding id */

#ifndef VAR_TYPES_HPP
#define VAR_TYPES_HPP

#include<string>
#include<vector>

// 0 is null type
const int types_size[] = { 0, 4, 4 };

// To be
// const int types[] = { " ", "u32", "u16", "u8", "i32", "i16", "i8" };
// const int types_size[] = { 0, 4, 2, 1, 4, 2, 1 }; // The size in bytes each type takes up
// #define TYPES_SIZE 7


// This takes in the two types and says how they can be transformed
// 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
unsigned char can_be_transformed_into(unsigned char type0, unsigned char type1);

// This checks if the given string is a valid name
bool is_valid_name(std::string name);

// This checks if a string is a valid type
int is_type(std::string possible_type);

// This either returns the variable id or 0
int into_id(std::string &string_type);

// This turns a variable id into it's string counterpart or NULL if it isn't valid
std::string id_into_string(int var_id);

// Checks if a string is numbers
bool is_str_num(std::string &possible_number);

// This returns the int of a string of numbers
int get_str_num(std::string &possible_number);

// Checks if a string is just letters
bool is_str_letters(std::string &test_string);

#endif