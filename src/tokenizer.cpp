/*
This file handles tokenizing a file making further processing a simplier job - Ryan Vail 2022 Sep. 8th
*/

// TODO: Some special characters shouldn't add spaces based on the new RPN system

#pragma once

#define blank_characters_len 4
const char blank_characters[] = { 9, 10, 13, 32 };
#define special_characters_len 15
// TODO: The length's don't need to be constants
// Old special chars
// const char special_characters[] = { '+', '-', '*', '/', '(', ')', '{', '}', ';', '^', '!', '&', '=', '|', '<', '>', '[', ']' };
const char special_characters[] = { '*', '/', '{', '}', ';', '^', '!', '&', '|', ',', '(', ')', '$', '%', '@' };

// Returns true if a character is a blank otherwise false
bool is_blank(char &character)
{
	for (int current_blank=0; current_blank < blank_characters_len; current_blank++)
	{
		if (blank_characters[current_blank] == character)
		{
			return true;
		}
	}
	return false;
}

// Returns true if a character is special otherwise false
bool is_special(char &character)
{
	for (int current_special=0; current_special < special_characters_len; current_special++)
	{
		if (special_characters[current_special] == character)
		{
			return true;
		}
	}
	return false;
}

// This replaces multiple blank characters with 0 in ASCII and should be the only public function
std::vector<std::string> blanker(std::vector<std::string> &file_contents)
{
	std::vector<std::string> formated_file; // TODO: Change this from a vector of chars to a vector of strings
	bool last_blank = true; // This is set to true so if the first character is a blank it doesn't get added
	for (std::vector<std::string>::iterator file_itr = file_contents.begin(); file_itr != file_contents.end(); file_itr++)
	{
		std::string current_string = *file_itr;
		std::string formated_string;
		for (std::string::iterator str_itr = current_string.begin(); str_itr != current_string.end(); str_itr++)
		{
			// The above code just loops through every character in every string
			if (is_blank(*str_itr))
			{
				// If this is the first blank add an ASCII 0
				if (!last_blank)
				{
					formated_file.push_back(formated_string);
					formated_string = "";
					// formated_file.push_back(0);
					last_blank = true;
				}
				continue;
			}
			if (is_special(*str_itr))
			{
				if (!last_blank)
				{
					formated_file.push_back(formated_string);
					formated_string = "";
				}
				formated_string.push_back(*str_itr);
				formated_file.push_back(formated_string);
				formated_string = "";
				//formated_file.push_back(0);
				last_blank = true;
				continue;
			}
			formated_string.push_back(*str_itr);
			last_blank = false;
		}
	}
	return formated_file;
}
