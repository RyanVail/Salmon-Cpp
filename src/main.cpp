/*
	This is the main file of the C++ side of the salmon programming languague - Ryan Vail 2022 Sep. 7th
*/

// C++ includes
#include<iostream>
#include<fstream>
#include<vector>
#include<math.h> // TODO: math.h is just used for pow in var_types

// Definitions
// TODO: Make these all caps
#define Version 0.0 // The version of the Salmon compiler
#define Debug 1 // If we are in debug mode

// Local includes
#include"tokenizer.cpp"
//#include"global.cpp"
#include"var_types.cpp"
#include"symbol_table.cpp"
#include"intermediate.cpp"
//#include"token_reader.cpp"

#if Debug
	#include"debug.cpp"
#endif

// This loads in a file as a string and returns it
std::vector<std::string> load_file(char file_name[])
{
	std::fstream file_handle;
	file_handle.open(file_name);
	std::vector<std::string> file_contents;
	std::string file_line;
	// TODO: The break clause should check at the # of bytes read rather than the line equaling "" because there's cases that doesn't happen
	// This loop reads the file
	while (true)
	{
		std::getline(file_handle, file_line);
		if (file_line == "") { return file_contents; }
		file_contents.push_back(file_line);
	}
}

// This takes arguments
int main(int argc, char *argv[])
{
	std::cout << "Compiling file " << argv[1] << " with V" << Version << "...\n\n";
	std::vector<std::string> file_contents = load_file(argv[1]);
	file_contents = blanker(file_contents);

	// --- Test --- \\
	global globals;
	std::vector<std::string> file = { "u32", "a", "253", "=", "u32", "b", "23", "14", "+", "=", "fn", "main(u32 a, u32 b)" };
	//std::vector<std::string> file = { "u32", "a", "3", "=", ";"};
	////read_var(globals, file_contents, 0);
	//void read_var(global &globals, std::vector<std::string> &file, int location)
	std::vector<inter> inter_file = file_into_inter(file);
	#if Debug
		std::cout << "\n\n";
		print_symbol_table();
	#endif
	// --- TEST --- \\ 

	return 0;
}
