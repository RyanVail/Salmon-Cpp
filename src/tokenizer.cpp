/* This file handles tokenizing a file making further processing a simplier job */
/* TODO: This shouldn't tokenize things in quotes. */
#include<vector>
#include<string>
#include<iostream>

#define blank_characters_len 4
const i8 blank_characters[] = { '\n', '\r', '\t', ' ' };
#define special_characters_len 16
// TODO: The lengths don't need to be constants
const i8 special_characters[] = { '*', '/', '{', '}', ';', '^', '!', '&', 
                                  '|', ',', '(', ')', '$', '%', '@', '#' };

// Returns true if a character is a blank otherwise false
bool is_blank(char &character)
{
	// TODO: Find out if GCC unravels this at O3
	for (i32 _b=0; _b < blank_characters_len; _b++)
		if (blank_characters[_b] == character)
			return true;

	return false;
}

// Returns true if a character is special otherwise false
bool is_special(char &character)
{
	for (i32 _s=0; _s < special_characters_len; _s++)
		if (special_characters[_s] == character)
			return true;

	return false;
}

// This seperates the file into tokens 
std::vector<std::string> file_into_tokens(std::vector<std::string> file_contents)
{
	std::vector<std::string> formated_file; // Each string is another token
	i32 comments = 0; // The # of comments we are insde if any
	// This goes over every string in the file
	for (std::string current_string : file_contents)
	{
		std::string formated_string = "";
		// This goes over every char in the string
		for (std::string::iterator itr = current_string.begin(); itr < current_string.end(); itr++)
		{
			// We set the currrent char equal to the current char
			char current_char = *itr;

			// If we are starting a comment we incrament "comments" and skip the next '*'
			if (*(itr+1) == '*' && current_char == '/') 
			{
				comments++; 
				itr++; 
				continue; 
			}

			// If we are ending a comment we decrament "comments" and skip the next '/'
			if (*(itr+1) == '/' && current_char == '*') 
			{ 
				comments--;
				itr++; 
				continue; 
			}

			// If we are in a comment we just repeat the loop while setting "last_char" to the current char
			if (comments)
				continue;

			// If the current char is blank
			if (is_blank(current_char))
			{
				// If there is something in the formated string add it to the formated file
				if (formated_string.size()) 
				{ 
					formated_file.push_back(formated_string); 
					formated_string = "";
				}
				continue;
			}

			// If the current char is special
			if (is_special(current_char))
			{
				// If there are things in the formated string we add it to the formated file
				if (formated_string.size()) 
				{ 
					formated_file.push_back(formated_string); 
					formated_string = "";
				}
				// We add the special char to the formated file in its own token
				formated_string = ""; 
				formated_file.push_back(std::string(1, current_char));
				continue;
			}
			// We add the current char which by now is just a normal char
			formated_string.push_back(current_char);
		}
		// If we have left overs in formated string
		if (formated_string.size())
			formated_file.push_back(formated_string);
	}
	return formated_file;
}
