#ifndef ELF_H
#define ELF_H

#include<typedefs.hpp>

/*
 * This adds an executable section to the elf file.
 */
void add_exe_section_elf();

/*
 * This takes in the desired output file name and writes the static variables
 * into it. Then it frees all of the dynamically allocated variables.
 */
void construct_elf();

/*
 * This function initializes all of the static varibles to the inputed values.
 */
void init_elf(bool _is_64_bit, bool _little_endianness, u8 _target_os,
u8 _target_cpu);

#endif