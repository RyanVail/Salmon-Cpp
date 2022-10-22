/*
	This is the main file of the C++ side of the salmon programming languague - Ryan Vail 2022 Sep. 7th
*/

#pragma once

#include<iostream>
#include<fstream>
#include<vector>
#include<math.h> // TODO: math.h is just used for pow in var_types. Just rewrite it!
#include<stack>

// Definitions and stuff
#define VERSION 0.0 // The version of the Salmon compiler
#define DEBUG 1 // If we are in debug mode
#define AARCH32_COMPILATION_TARGET 0
unsigned char optimization_level = 0;
unsigned char compilation_target = 0;

#include"tokenizer.cpp"
#include"var_types.cpp"
#include"symbol_table.cpp"
#include"intermediate/intermediate.cpp"
#include"asm/aarch32_asm.cpp"

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
		if (file_line == "") 
		{ 
			return file_contents;
			if (!(file_contents.size())) { std::cout << "Something went wrong while reading the file.\n"; exit(-1); } // TODO: Replace this, it's just terrible
		}
		file_contents.push_back(file_line);
	}
}

// This takes arguments
int main(int argc, char *argv[])
{
	// TODO: Add compile arguments
	if (!argv[1]) { std::cout << "Please enter a file name to compile.\n"; exit(-1); };
	std::cout << "Compiling file " << argv[1] << " with V" << VERSION << "...\n\n";
	std::vector<std::string> file_contents = load_file(argv[1]);
	file_contents = blanker(file_contents);

	std::vector<inter> inter_file = file_into_inter(file_contents);
	#if DEBUG
		std::cout << "\n\n";
		print_symbol_table();
	#endif
	
	// Computes function stack sizes
	compute_function_stack_size();
	std::vector<std::string> output_asm;
	if (compilation_target == AARCH32_COMPILATION_TARGET) 
	{ 
		output_asm = aarch32_asm::intermediates_into_asm(inter_file);
	}
	else { std::cout << "Error while choosing compilation target.\n"; exit(-1); }
	for (std::string current_asm : output_asm)
	{
		std::cout << current_asm << "\n";
	}
	return 0;
}
