/* 
	This file handles turning string based variable types into their corrisponding id - Ryan Vail 2022 Sep. 9th 
*/

const std::vector<std::string> types = { "", "u32", "i32", "char" };

// This either returns the variable id or 0
int into_id(std::string &string_type)
{
	for (int i=1; i < types.size(); i++)
	{
		if (types[i] == string_type)
		{
			return i;
		}
	}
	return 0;
}

// This turns a variable id into it's string counterpart or NULL if it isn't valid
std::string id_into_string(int &var_id)
{
	if (var_id >= types.size() || var_id <= 0) { return NULL; }
	return types[var_id];
}

// Checks if a string is numbers
bool is_str_num(std::string &possible_number)
{
	for (std::string::iterator itr = possible_number.begin(); itr != possible_number.end(); itr++)
	{
		if (*itr < 48 || *itr > 57)
		{
			return false;
		}
	}
	return true;
}

// This returns the int of a string of numbers
int get_str_num(std::string &possible_number)
{
	int value = 0;
	for (int i=0; i < possible_number.size(); i++)
	{
		value += (possible_number[i] - 48) * pow(10, -i + possible_number.size() - 1); // This just shifts the ASCII over
	}
	return value;
}

// Checks if a string is just letters
bool is_str_letters(std::string &test_string)
{
	for (std::string::iterator itr = test_string.begin(); itr != test_string.end(); itr++)
	{
		// 65-90 Uppercase
		// 97-122 Lowercase
		if ((*itr > 122 || *itr < 97) && (*itr > 90 || *itr < 65))
		{
			return false;
		}
	}
	return true;
}

// TODO: This should be able to take in constant values for array sizes too
// This checks if the variable being defined is an array and if it is this returns the length of the var array and always returns the correct token to jump to
int is_var_array(std::vector<std::string> &file, int location)
{
	// If this isn't a list
	if (file[location-1] != "[") { return -1; }

	// This makes sure we have a number for the length of the array
	if (!is_str_num(file[location])) 
	{
		if (file[location] == "?") { throw std::runtime_error("Vectors are currently not supported."); }
		else { throw std::runtime_error("Error expected a number in array size."); }
	}
	if (file[location+1] != "]") { throw std::runtime_error("Expected ]."); }
	return get_str_num(file[location]);
}

// Checks if the variable can be read as a u32
bool can_be_u32(variable var)
{
	if (var.type == 2) { throw std::runtime_error("Error cannot explicitly change type i32 to type u32."); }
	if ((var.type == 1 || var.type == 3) && !var.list_length && var.in_scope && !var.pointer) { return true; }
	return false;
}
