/*
This file has a bunch of global definitions such as a list of variables and functions - Ryan Vail 2022 Sep. 8th
*/

struct variable
{
	bool used = false; // If the variable is used
	bool constant = false; // If the variable is a constant
	int type = 0; // The variable type
};

struct function
{
	int uuid = 0; // UUID one is the main function
	bool used = false; // If the function is used
	std::vector<variable> inputs; // These are the inputs to the function
};

struct if_statment
{
	int uuid;
	bool used = false; // If there are lines inside the if statment or not
};

struct while_statment
{
	int uuid;
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
};