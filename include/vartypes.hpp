/* This file handles turning string based variable types into their corrisponding id */

#ifndef VARTYPES_HPP
#define VARTYPES_HPP

#include<string>
#include<vector>
#include<typedefs.hpp>

// 0 is null type
const i32 types_size[] = { 0, 4, 4 };

// To be
// const i32 types[] = { " ", "u32", "u16", "u8", "i32", "i16", "i8" };
// const i32 types_size[] = { 0, 4, 2, 1, 4, 2, 1 }; // The size in bytes each type takes up
// #define TYPES_SIZE 7

// This checks if a type is normal IE. 32 bit
bool is_normal(u8 type);

// This checks if a type is a char IE. 8 bit
bool is_char(u8 type);

// This takes in the two types and says how they can be transformed
// 0 -> can be transformed 1 -> cannot be transformed without explicit transform 2-> can't be at all
u8 can_be_transformed_into(u8 type0, u8 type1);

// This checks if the given string is a valid name
bool is_valid_name(std::string name);

// This checks if a string is a valid type
i32 is_type(std::string possible_type);

// This either returns the variable id or 0
i32 into_id(std::string &string_type);

// This turns a variable id into it's string counterpart or NULL if it isn't valid
std::string id_into_string(i32 var_id);

// Checks if a string is numbers
bool is_str_num(std::string &possible_number);

// This returns the i32 of a string of numbers
i32 get_str_num(std::string &possible_number);

// Checks if a string is just letters
bool is_str_letters(std::string &test_string);

#endif