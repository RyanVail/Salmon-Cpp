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
std::string into_string(int &var_id)
{
	if (var_id >= types.size() || var_id <= 0) { return NULL; }
	return types[var_id];
}