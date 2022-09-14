/* 
	This file reads through the tokenized lines of the file and defines variables aswell as functions - Ryan Vail 2022 Sep. 9th 
*/

// Consts & Definitions
const std::vector<std::string> invalid_names = {"fn", "void", "null", "if", "else", "while", "end", "const", "return", "print", "struct"}; // Invalid function and variable names
const std::vector<std::string> expressions = { ">", "<", "=", "+", "-", "*" };
#define invalid_names_len 11

// Functions
int evaluate_consts(global &globals, std::vector<std::string> &file, int location);
int find_string_in_file(std::vector<std::string> &file, std::string string_to_find, int location);

// This function reads through all the tokens defining functions, variables, if statments, and while statments
void read_tokens(global &globals, std::vector<std::string> &file)
{
	for (int i=0; i > file.size(); i++)
	{
		//
	}
}

// This adds a variable to the global variable array and exports the assembly needed to create the variable
// This is called if a token is a variable type or the "const" token
void read_var(global &globals, std::vector<std::string> &file, int location)
{
	// Variables that will be usefull
	int array_len = -1; // The length of the array if this is an array if it isn't an array this is zero
	bool constant = false; // If the variable is a constant or not
	int type; // The variable type

	if (file[location] == "const") { constant = true; } // Checks if the variable is a constant
	// TODO: This should make sure everything on the right side of the equals sign is a constant too

	// This checks if the variable type is valid
	type = into_id(file[location + constant]);
	if (!type) { throw std::runtime_error("Error variable type not found."); }

	// This checks to see if the variable is an array
	if (file[location+constant+1] == "[")
	{
		array_len = evaluate_consts(globals, file, location + constant + 2);
		location = find_string_in_file(file, "]", location + constant + 2) + 1;
	}
	else { location += constant + 1; }
		// This checks to see if the variable is an array
		/* array_len = is_var_array(file, location + 2 + constant);
		if (array_len == -1) { location -= 3; }
		location += 4; */

	// This makes sure the variable name is just letters
	if (!is_str_letters(file[location])) { throw std::runtime_error("Invalid variable name."); }

	std::cout << "Defining variable: " << file[location] << ", type: " <<  id_into_string(type) << ", size: " << array_len << "\n";

	// This makes sure we have an '=' or a ';' on the next token
	// if (file[location + constant + 2] != "=" && file[location + constant + 2] != ";") { throw std::runtime_error("Error expected variable definiton."); }
}

// This evaluates the thing on the right side of the equal sign and turns it into assembly

/* Rules:
	1. Every value/variable has to be seperated by an operation
	2. Variable then () with no operation means we are calling a function
	3. >=, ==, !=, <= evaluate to 1_u32 or 0_u32 and are considered expresisions
	4. **, *, +, - are also considered expressions
*/

/* Example:
	4 + 2 * (4 / 2)
	4 + 2 * (2)
	6 * 2
	12
*/

// Finds the next certain string in the file starting at a specified location and returns that location
int find_string_in_file(std::vector<std::string> &file, std::string string_to_find, int location)
{
	for (int i=location; i < file.size(); i++)
	{
		if (file[i] == string_to_find)
		{
			return i;
		}
	}
	return -1;
}

// This checks a string to see if it's an expression
char check_expression(std::string possible_expression, std::string possible_next_expression)
{
	for (char i=0; i != expressions.size(); i++)
	{
		if (expressions[i] == possible_expression)
		{
			if (possible_next_expression == "=")
			{
				if (i > 2)
				{
					throw std::runtime_error("Unknown expression.");
				}
				return i + 8;
			}
			return i;
		}
	}
	return -1; // Not an expression
}

// TODO: This doesn't support bitwise operations or powers
// This computes an expression { ">", "<", "=", "+", "-", "*" };
int compute_expression(int input0, int expression_id, int input1)
{
	if (expression_id > 6)
	{
		if (expression_id == 0 + 8) { return input0 >= input1; }
		if (expression_id == 1 + 8) { return input0 <= input1; }
		if (expression_id == 2 + 8) { return input0 == input1; }
	}
	if (expression_id == 0) { return input0 > input1; }
	if (expression_id == 1) { return input0 < input1; }
	if (expression_id == 2) { return input0 = input1; }
	if (expression_id == 3) { return input0 + input1; }
	if (expression_id == 4) { return input0 - input1; }
	if (expression_id == 5) { return input0 * input1; }
	return 0;
}

// This evaluates mathmatical expressions only containing consts if there are non-consts it calls an error
int evaluate_consts(global &globals, std::vector<std::string> &file, int location)
{
	bool had_last_value = false;
	int last_value = 0;
	for (int i=location; i < file.size(); i++)
	{
		if (!is_str_num(file[i]))
		{
			if (file[i] == "]") { return last_value; }
			if (file[i] == "(" || file[i] == ")") { throw std::runtime_error("parentheses not yet supported sorry. TODO: add parenthese support. There, now it's your problem."); }
			else if (check_expression(file[i], file[i+1]) == -1) { throw std::runtime_error("Unknow expression."); }
			else
			{
				if (!had_last_value) { throw std::runtime_error("Expected const before expression."); }
				last_value = compute_expression(last_value, check_expression(file[i], file[i+1]), get_str_num(file[i+1+(check_expression(file[i], file[i+1]) > 6)]));
				i += 1 + (check_expression(file[i], file[i+1]) > 6);
			}
		}
		else
		{
			if (had_last_value) { throw std::runtime_error("Expected an expression."); }
			had_last_value = true;
			last_value = get_str_num(file[i]);
		}
	}
	return last_value;
}

void evaluate(global &globals, std::vector<std::string> &file, int bounds[])
{
	// This first goes through and finds all the opening and closing parentheses and does the inner ones first
}

// TODO:
	// Create a helper function called read_var which takes in the next two tokens and adds the variable to the global list of variables
	// Create a helper function called read_fn ^^^