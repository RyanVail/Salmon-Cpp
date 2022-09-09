/*
This is the main file of the C++ side of the salmon programming languague - Ryan Vail 2022 Sep. 7th
*/

// C++ includes
#include<iostream>
#include<fstream>
#include<vector>

// Local includes
#include"tokenizer.cpp"
#include"global.cpp"

// Definitions
#define Version 0.0

// This loads in a file as a string and returns it
std::vector<std::string> load_file(char file_name[])
{
	std::fstream file_handle;
	file_handle.open(file_name);
	std::vector<std::string> file_contents;
	std::string file_line;
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
	std::cout << "Compiling file " << argv[1] << " with V" << Version << "...\n";
	std::vector<std::string> file_contents = load_file(argv[1]);
	blanker(file_contents);
	return 0;
}