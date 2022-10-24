/* This is the main file of the C++ side of the salmon programming languague */

#include<main.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<stack>

#include"tokenizer.cpp"
#include<var_types.hpp>
#include<symbol_table.hpp>
#include<intermediate/intermediate.hpp>
#include<asm/aarch32_asm.hpp>

const std::string help_message = "Usage: salmonc [options] file...\n\nOptions:\n\t-h\t--help\t\tDisplays help\n";

#if DEBUG
	#include<debug.hpp>
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
			if (file_contents.size() <= 0) { std::cout << "Something went wrong while reading the file.\n"; exit(-1); } // TODO: Replace this, it's just terrible
			return file_contents;
		}
		file_contents.push_back(file_line);
	}
}

// This takes arguments
int main(int argc, char *argv[])
{
	std::string file_name;
	for (int i=1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (argv[i] == std::string("--help"))
			{
				std::cout << help_message; exit(1);
			}
			std::cout << "Unknown option: " << argv[i] << "\n"; exit(-1);
		}
		else if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'h':
					std::cout << help_message; exit(1);
					break;
				default:
					std::cout << "Unknown option: " << argv[i] << "\n"; exit(-1);
			}
		}
		else 
		{
			if (file_name != "") { std::cout << "Expected one file name but got multiple.\n"; exit(-1); }
			file_name = argv[i];
		}
	}
	if (file_name == "") { std::cout << "Please enter a file name to compile or use -h to display help.\n"; exit(-1); };
	std::cout << "Compiling file " << file_name << " with " << VERSION << "...\n\n";
	
	std::vector<inter> inter_file = file_into_inter(blanker(load_file(argv[1])));

	#if DEBUG
		std::cout << "\n\n";
		print_symbol_table();
	#endif
	
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
