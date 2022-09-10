/*
	This file has a bunch of global definitions such as a list of variables and functions - Ryan Vail 2022 Sep. 8th
*/

struct variable
{
	std::string name; // The name of the variable
	bool used = false; // If the variable is used
	bool constant = false; // If the variable is a constant or not
	bool in_scope = true; // If the variable is in scope
	int const_value; // The constant value of the variable
	int type; // The variable type
	int list_length; // The length of the list if there is one
};

// These are variables that are inputs to functions
struct input_variable
{
	std::string name;
	int type;
	bool used = false;
};

struct function
{
	std::string name; // The name of the function
	int id = 0; // ID one is the main function
	bool used = false; // If the function is used
	int scope[2]; // The starting and ending lines of the function
	std::vector<variable> inputs; // These are the inputs to the function
};

struct if_statment
{
	int id;
	bool used = false; // If there are lines inside the if statment or not
	std::vector<char> condition; // This is the condition to preform the if statment
};

struct while_statment
{
	int id;
	bool used = false; // If there are lines inside the while loop or not
	std::vector<char> condition; // This is the condition to repeat the loop in code
};

// This is the global list of all other types of global structs 
struct global
{
	std::vector<function> functions;
	std::vector<variable> variables;
	std::vector<if_statment> if_statments;
	std::vector<while_statment> while_statments;

	// This returns true if the string is a name of a function otherwise false
	bool is_a_function(std::string name)
	{
		for (std::vector<function>::iterator f = functions.begin(); f != functions.end(); f++)
		{
			function current_f = *f;
			if (current_f.name == name)
			{
				return true;
			}
		}
		return false;
	}

	// This returns true if the string is a name of a var otherwise false
	bool is_a_var(std::string name)
	{
		for (std::vector<variable>::iterator v = variables.begin(); v != variables.end(); v++)
		{
			variable current_v = *v;
			if (current_v.name == name)
			{
				return true;
			}
		}
		return false;
	}
};