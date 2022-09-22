/*
	This file turns commands into their assembly repersentation - Ryan Vail Sep 21, 2022
	- I'm rewriting this because my code is a mess and I plan on just making a compiler that works then rewrite the language in that and actually plan next time
	- While turning lines into assembly this also adds global variables and global functions
*/

// These holds single statments
struct statment
{
	int type;
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

// This turns a single expression into asm
void evaluate_to_asm()
{
	// This goes through and finds parentheses reordering the tokens and removing paretheses until it finds a ';', '{', or a '}'
	std::vector<std::string> new_tokens;

	// This doesn't check for anything being correct it just doesnt things if it's a + just add r0 and r1 but vars do have to be checked
}
