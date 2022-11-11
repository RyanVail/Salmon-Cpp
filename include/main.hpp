#ifndef MAIN_HPP
#define MAIN_HPP

#include<typedefs.hpp>
#include<string>
#include<vector>

#define VERSION "V0.0.0"
#define DEBUG 1
#define AARCH32_COMPILATION_TARGET 0

static u8 optimization_level = 0;
static u8 compilation_target = AARCH32_COMPILATION_TARGET;
static std::vector<std::string> *files_to_compile;

#endif