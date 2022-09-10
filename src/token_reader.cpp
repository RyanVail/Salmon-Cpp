/* 
	This file reads through the tokenized lines of the file and defines variables aswell as functions - Ryan Vail 2022 Sep. 9th 
*/

// Consts & Definitions
const std::vector<std::string> invalid_names = {"fn", "void", "null", "if", "else", "while", "end", "const", "return", "print", "struct"}; // Invalid function and variable names
#define invalid_names_len 11

// This function reads through all the tokens defining functions, variables, if statments, and while statments
void read_tokens(global &globals, std::vector<std::string> &file)
{
	return;
}

// This function reads a single token
void read_token(std::string &token)
{
	// If we are defining a function
	if (token == "fn")
	{

	}
	return;
}