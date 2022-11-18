/* This file handles turning aarch32 asm into an object file. */

#ifndef AARCH32_ASSEMBLER_H
#define AARCH32_ASSEMBLER_H


/*
 * This function takes in aarch32 assembly and an output object file name and
 * assembles the assembly and saves it into the output file.
 */
void aarch32_asm_into_obj(char** input_asm, char* output_file_name);

#endif