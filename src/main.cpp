/* This is the main file of the C++ side of the salmon programming languague */

#include<main.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<stack>

#include"tokenizer.cpp" // TODO: Add "tokenizer.hpp" and put more functions inside namespaces
#include<vartypes.hpp>
#include<symboltable.hpp>
#include<intermediate/intermediate.hpp>
#include<asm/aarch32_asm.hpp>
#include<error.hpp>

const std::string help_message = "Usage: salmon [options] file...\n\nOptions:\n\t-h\t--help\t\tDisplays help\n";

#if DEBUG
	#include<debug.hpp>
#endif

// This loads in a file as a string and returns it
std::vector<std::string> load_file(std::string file_name)
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
			if (file_contents.size() <= 0) 
				error::send_error("Something went wrong while reading the file.\n");

			return file_contents;
		}
		file_contents.push_back(file_line);
	}
}

// This function processes command line options
std::vector<string> process_options(i32 argc, char *argv[])
{
	std::vector<std::string> files_name;
	for (i32 i=1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (argv[i] == std::string("--help"))
			{
				std::cout << help_message; 
				exit(1);
			}

			error::send_error("Unknown option: " + std::string(argv[i]) + "\n");
		}
		else if (argv[i][0] == '-')
		{
			switch(argv[i][1]) 
			{
			case 'h':
				std::cout << help_message; exit(1);
				break;
			default:
				error::send_error("Unknown option: " + std::string(argv[i]) + "\n");	
			}
		}
		else 
		{
			files.push_back(std::to_string(argv[i]));
		}
	}
	return file_name;
}

// This takes arguments
i32 main(i32 argc, char *argv[])
{
	std::vector<std::string> file_names	 = process_options(std::move(argc), std::move(argv));

	// TODO: This should check for duplicates in the file names.

	// Files should be compiled sepeartly then be linked together we should
	// only be able to include a file once

	std::string file_name = file_names[0];

	if (file_name == "")
		error::send_error("Please enter a file name to compile or use -h to display help.\n");

	std::cout << "Compiling file " << file_name << " with " << VERSION << "...\n\n";

	std::vector<inter> inter_file = file_into_inter(file_into_tokens(load_file(file_name)));

	#if DEBUG
		std::cout << "\n\n";
		print_symbol_table();
	#endif
	
	std::vector<std::string> output_asm;

	if (compilation_target == AARCH32_COMPILATION_TARGET) 
		output_asm = aarch32_asm::intermediates_into_asm(inter_file);
	else
		error::send_error("Error while choosing compilation target.\n");

	for (std::string current_asm : output_asm)
		std::cout << current_asm << "\n";

	return 0;
}
