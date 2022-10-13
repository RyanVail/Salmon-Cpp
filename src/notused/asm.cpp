/*
	This file turns commands into their assembly repersentation - Ryan Vail Sep 21, 2022
	- I'm rewriting this because my code is a mess and I plan on just making a compiler that works then rewrite the language in that and actually plan next time
	- While turning lines into assembly this also adds global variables and global functions
*/

// These holds single statments
struct statment
{
	char type; // 0 -> constant; 1 -> variable; 2 -> function call; 3 -> add; 4 -> sub; 5 -> mul; 6 -> lsl; 7 -> lsr;
	int value; // If this is a const its value; if variable the variable ID
	std::vector<std::string> end_asm; // This is the asm that is added to the end when this is done
};

// This takes the tokens of the file and turns them into assembly
void file_to_asm(std::vector<std::string> &file, &globals global)
{
	std::vector<statment> statment_vec; // This vector holds the current statment we are inside of ex. "function" or "if" or "while"
	int location = 0; // The current location in the file

	// If we are begging a statment
	if (file[location] == "{") { }

	// If we are ending a statment
	if (file[location] == "}") { }
}


/*
	+ 43
	- 45
	= 61
	< 60
	> 62
	! 33
	* 42
*/

// This takes stuff off the stack before doing an operation
void off_load_stack(globals &global, std::vector<statment> &statments)
{
	// This makes sure we have enough things in the stack
	if (!func && statments.size() < 2) { throw std::runtime_error("Erorr no more values in RPN stack"); }
	
	// This does this for both inputs
	for (bool i = 0; i < 1; i++)
	{
		// If it's a const just move it into the register
		if (statments[i].type == 0) { global.file_asm.push_back("mov r" + std::to_string(i) + " #" + std::to_string(statments[0].value)); }
		if ()
	}
}

// This turns a single expression into asm
void evaluate_to_asm(std::vector<std::string> &file, globals &global, int location)
{
	std::vector<std::string> new_tokens;

	// EX: 
	//	3 5 + 3 , , test_func()
	//	~~~~~~~~~~~~~~~~~~~~~
	//	test_func(3 + 5, 3)
	
	std::vector<statment> token_stack;
	
	while (true) {  }
	
	// This goes through the tokens and outputs the assembly
	for (std::vector<statment>::iterator statment_itr = token_stack.start(); statment != token_stack.end(); stack++)
	{
		statment current_statment = *statment_itr;
		if (current_statment == 3 ) { global.file_asm.push_back("add r0, r1"); }
		if (current_statment == 4 ) { global.file_asm.push_back("sub r0, r1"); }
		if (current_statment == 5 ) { global.file_asm.push_back("mul r0, r1"); }
		if (current_statment == 6 ) { global.file_asm.push_back("lsl r0, r1"); }
		if (current_statment == 7 ) { global.file_asm.push_back("lsr r0, r1"); }
	}
}
