/* This is responsible for creating the ELF files. */

#include<elf.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<typedefs.hpp>

#define MAGIC_NUMBER 0x7F454c46 // 0x7F ELF
static bool IS_64_BIT = false;
static bool LITTLE_ENDIANNESS = true;
static u8 TARGET_OS = 0x03; // LINUX
static u16 TARGET_CPU = 0x28; // ARM
static u64 ENTRY_POINT = 0;
static u64 PROGRAM_HEADER_START = 0;
static u64 SECTOR_START = 0;
static u16 ELF_HEADER_SIZE = 0;
static u16 PROGRAM_HEADER_ENTRY_SIZE = 0;
static u16 PROGRAM_HEADER_ENTRIES = 0;
static u16 SECTION_HEADER_ENTRY_SIZE = 0;
static u16 SECTION_HEADER_ENTRIES = 0;
static u16 SECTION_NAME_INDEX = 0;

static char** SECTIONS;

/*
 * This function initializes all of the static varibles to the inputed values.
 */
void init_elf(bool _is_64_bit, bool _little_endianness, u8 _target_os,
u8 _target_cpu)
{
    IS_64_BIT = _is_64_bit;
    LITTLE_ENDIANNESS = _little_endianness;
    TARGET_OS = _target_os;
    TARGET_CPU = _target_cpu;
}

/*
 * This adds an executable section to the elf file.
 */
void add_exe_section_elf()
{

}

/*
 * This takes in the desired output file name and writes the static variables
 * into it. Then it frees all of the dynamically allocated variables.
 */
void construct_elf()
{
    //

    // Logic goes here.

    // ==== Clean up ====
    for (int i=0; i < SECTION_HEADER_ENTRIES; i++)
    {
        free(SECTIONS[i]);
    }
    free(SECTIONS);
}

#undef MAGIC_NUMBER
