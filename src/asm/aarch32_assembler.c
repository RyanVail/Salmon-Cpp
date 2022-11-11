/* This file handles turning ASM into machine code. */

//#include<asm/aarch32_assembler.h>
#include<typedefs.hpp>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

/*
 * This is just a quick way to convert four chars into an int.
 */
#define c_to_i(c0, c1, c2, c3) (c0 + (c1 << 8) + (c2 << 16) + (c3 << 24))

enum reg;
struct branch;
enum condition;
enum shift_type;
struct instruction;
struct data_processing;
struct shift_reg_12bit;
struct load_or_store_reg;
struct shift_immediate_12bit;
enum data_process_operation;

static struct instruction* instructions;
static u32 instructions_count;
static u32 asm_count;
static char** _asm;

enum reg
{
    R0, // 0x0
    R1, // 0x1
    R2, // 0x2
    R3, // 0x3
    R4, // 0x4
    R5, // 0x5
    R6, // 0x6
    R7, // 0x7
    R8, // 0x8
    R9, // 0x9
    R10, // 0xa
    R11, // 0xb
    R12, // 0xc
    R13, // 0xd // SP
    R14, // 0xe // LR
    R15, // 0xf // PC
    no_reg,
};

enum shift_type
{
    LSL, // 0x0
    LSR, // 0x1
    ASR, // 0x2
    ROR, // 0x3
    no_shift,
};

// TODO: AL is the same as "no_cond" which means if there is an "AL" after an
// instruction we read the next char as if there isn't an "AL". 
enum condition
{
    EQ, // 0x0 // Set equal
    NE, // 0x1 // Clear not equal
    CS, // 0x2 // Set unsigned higher or same
    CC, // 0x3 // Clear unsigned lower
    MI, // 0x4 // Set negative
    PL, // 0x5 // Clear positive or negative
    VS, // 0x6 // Set overflow
    VC, // 0x7 // Clear no overflow
    HI, // 0x8 // Set and Z clear unsigned higher
    LS, // 0x9 // Clear or Z set unsigned lower or same
    GE, // 0xa // >
    LT, // 0xb // <
    GT, // 0xc // >=
    LE, // 0xd // <=
    AL, // 0xe // Always
};

enum data_process_operation
{
    AND, // 0x0 // operand one & operand two
    EOR, // 0x1 // operand one ^ operand two
    SUB, // 0x2 // operand one - operand two
    RSB, // 0x3 // operand two + operandg one
    ADD, // 0x4 // operand one + operand two
    ADC, // 0x5 // operand one + operand two + carry
    SBC, // 0x6 // operand one - operand two + carry - 1
    RSC, // 0x7 // operand two - operand one + carry - 1
    TST, // 0x8 // ANDs operand one and operand two sets flags
    TEQ, // 0x9 // EORs operand one and operand two sets flags
    CMP, // 0xa // SUBs operand one and operand two sets flags
    CMN, // 0xb // ADDs operand one and operand two sets flags
    ORR, // 0xc // operand one | operand two
    MOV, // 0xd // operand two
    BIC, // 0xe // operand one AND NOT operand two
    MVN, // 0xf // NOT operand two
    no_data_processing_operation,
};

/*
 * This turns the static "instructions" into binary and returns a pointer to
 * the binary file.
 */
u8* instructions_into_binary()
{
    u8* _output_binary = (u8*)malloc(instructions_count * 4);

    free(instructions);
    return _output_binary;
}

/*
 * These functions process asm instructions based on their type specifications.
 */
static inline void process_data(u32 i, enum data_process_operation operation);
static inline void process_mem(u32 i, bool load_or_store);
static inline void process_sys_call(u32 i);
static inline void process_branch(u32 i);
static inline void process_clz(u32 i);
/* These are helper functions. */
enum reg get_reg(u32 i);
u32 get_int_immediate(u32 i);
u16 get_12bit_immediate(u32 i);
enum shift_type get_shift(u32 i);
enum condition get_condition(u32 i, u32 skip);
/*
 * This gets and returns the int form of an intermediate
 */
u32 get_int_immediate(u32 i)
{
    bool _negative = _asm[i][0] == '-';
    u64 _value = 0;
    u16 _first_two = *( (u16*)(_asm[i] + sizeof(char) * _negative) );
    // TODO: These don't handle overflow
    // TODO: Instead of having another if statment we can just | the current
    // char to insure it is lowercase.
    if (_first_two == c_to_i('0','x','\0','\0'))
    {
        char* current_char = &_asm[i][2+_negative];
        while (*current_char != '\0')
        {
            _value <<= 4;
            if (*current_char > 47 && *current_char < 58)
                _value += *current_char - 48;
            else if (*current_char > 96 && *current_char < 123)
                _value += *current_char - 87;
            else if (*current_char > 64 && *current_char < 91)
                _value += *current_char - 55;
            else if (*current_char == '_')
                continue;
            else
            {
                printf("Expected a number.\n");
                exit(-1);
            }
            current_char += sizeof(char);
        }
    }
    else if (_first_two == c_to_i('0','b','\0','\0'))
    {
        char* current_char = &_asm[i][2+_negative];
        while (*current_char != '\0')
        {
            _value <<= 1;
            if (*current_char > 47 && *current_char < 50)
                _value += *current_char - 48;
            else if (*current_char == '_')
                continue;
            else
            {
                printf("Expected a number.\n");
                exit(-1);
            }
            current_char += sizeof(char);
        }
    }
    else
    {
        char* current_char = &_asm[i][_negative];
        while (*current_char != '\0')
        {
            _value *= 10;
            if (*current_char > 47 && *current_char < 58)
                _value += *current_char - 48;
            else if (*current_char == '_')
                continue;
            else
            {
                printf("Expected a number.\n");
                exit(-1);
            }
            current_char += sizeof(char);
        }       
    }
    return _value;
}

/*
 * This returns the 12bit immediate shift repersentation of an immediate.
 */
u16 get_12bit_immediate(u32 i)
{
    i++;
    u32 _value = get_int_immediate(i);
    /*
     * This gets the range of the immediate value and makes sure it can fit
     * into an eight bit number. Then it gets the shift amount needed.
     */
    // TODO: If we are compiling this can be optimized with "CLZ".
    // TODO: This needs to account for rotating not just shifting.
    // TODO: Negatives need to be accounted for.
    u32 _last_bit = -1;
    for (u32 i = 1 << 31; i != 0; i >>= 1)
    {
        _last_bit++;
        if (_value & i)
            break;
    }
    u32 _first_bit = 32;
    for (u32 i = 1; _first_bit != _last_bit; i <<= 1)
    {
        _first_bit--;
        if (_value & i || i == 0)
            break;
    }
    _first_bit = 31 - _first_bit;
    _last_bit = 31 - _last_bit;
    if (_first_bit & 1)
        _first_bit--;
    if (_last_bit - _first_bit > 8)
    {
        printf("Invalid immediate value: %i.\n", _value);
        exit(-1);
    }
    u8 _rotate = 16 - (_first_bit >> 1);
    _value >>= 16 - (_rotate << 1);
    return _value + (_rotate << 8);
}

/*
 * This takes in a char** and fills the static "instructions" array then calls
 * instructions_into_binary and returns a pointer to the instructions.
 */
struct instruction* asm_into_instructions(u32 _asm_count, char** _asm_)
{
    asm_count = _asm_count;
    _asm = _asm_;
    for (int i = 0; i < asm_count; i++)
    {
        /*
         * This takes the first three chars and uncapitalizes them.
         */
        u32 _asm_current = *(int*)_asm_[i];
        _asm_current |= 32 + (32 << 8) + (32 << 16);
        _asm_current &= 0b00000000111111111111111111111111;
        switch(_asm_current)
        {
        case c_to_i('a','n','d','\0'):
            process_data(i, AND);
            break;
        case c_to_i('e','o','r','\0'):
            process_data(i, EOR);
            break;
        case c_to_i('s','u','b','\0'):
            process_data(i, SUB);
            break;
        case c_to_i('r','s','b','\0'):
            process_data(i, RSB);
            break;
        case c_to_i('a','d','d','\0'):
            process_data(i, ADD);
            break;
        case c_to_i('a','d','c','\0'):
            process_data(i, ADC);
            break;
        case c_to_i('s','b','c','\0'):
            process_data(i, SBC);
            break;
        case c_to_i('r','s','c','\0'):
            process_data(i, RSC);
            break;
        case c_to_i('t','s','t','\0'):
            process_data(i, TST);
            break;
        case c_to_i('t','e','q','\0'):
            process_data(i, TEQ);
            break;
        case c_to_i('c','m','p','\0'):
            process_data(i, SUB);
            break;
        case c_to_i('c','m','n','\0'):
            process_data(i, CMN);
            break;
        case c_to_i('o','r','r','\0'):
            process_data(i, ORR);
            break;
        case c_to_i('m','o','v','\0'):
            process_data(i, MOV);
            break;
        case c_to_i('b','i','c','\0'):
            process_data(i, BIC);
            break;
        case c_to_i('m','v','n','\0'):
            process_data(i, MVN);
            break;
        case c_to_i('l','d','r','\0'):
            process_mem(i, true);
            break;
        case c_to_i('s','t','r','\0'):
            process_mem(i, false);
            break;
        case c_to_i('c','l','z','\0'):
            process_clz(i);
            break;
        default:
            /*
             * This checks for 'b', "bl", and "bx". This could be an if
             * statment but it fits better as a switch statment.
             */
            switch (_asm_current << 16 >> 16)
            {
            case c_to_i('b','e','\0','\0'):
            case c_to_i('b','x','\0','\0'):
                printf("Processing branch.");
                process_branch(i);
                break;
            default:
                if (_asm_current << 24 >> 24 == 'b')
                    process_branch(i);
                else
                {
                    printf("Unknown instruction: %s.\n", _asm_[i]);
                    exit(-1);
                }
            }
        }
        //exit(0);
        if (_asm[i][0] == ';')
            continue;
    }
    return instructions;
}

/*
 * This takes in the current location and the number of bytes to skip before
 * reading the condition.
 */
enum condition get_condition(u32 i, u32 skip)
{
    u16 _condition = *(u16*)(_asm[i] + skip);
    _condition |= 32 + (32 << 8);
    switch(_condition)
    {
    case c_to_i('e','q','\0','\0'):
        return EQ;
    case c_to_i('n','e','\0','\0'):
        return NE;
    case c_to_i('c','s','\0','\0'):
        return CS;
    case c_to_i('c','c','\0','\0'):
        return CC;
    case c_to_i('m','i','\0','\0'):
        return MI;
    case c_to_i('p','l','\0','\0'):
        return PL;
    case c_to_i('v','s','\0','\0'):
        return VS;
    case c_to_i('v','c','\0','\0'):
        return VC;
    case c_to_i('h','i','\0','\0'):
        return HI;
    case c_to_i('l','s','\0','\0'):
        return LS;
    case c_to_i('g','e','\0','\0'):
        return GE;
    case c_to_i('l','t','\0','\0'):
        return LT;
    case c_to_i('g','t','\0','\0'):
        return GT;
    case c_to_i('l','e','\0','\0'):
        return LE;
    case c_to_i('a','l','\0','\0'):
        return AL;
    default:
        return AL;
    }
}

/*
 * This gets and returns the shift type.
 */
enum shift_type get_shift(u32 i)
{
    u32 _shift = *(u32*)(_asm[i]);
    _shift |= 32 + (32 << 8) + (32 << 16);
    _shift &= 0b00000000111111111111111111111111;
    switch (_shift)
    {
    case c_to_i('l','s','l','\0'):
        return LSL;
    case c_to_i('l','s','r','\0'):
        return LSR;
    case c_to_i('a','s','r','\0'):
        return ASR;
    case c_to_i('r','o','r','\0'):
        return ROR;
    default:
        return no_shift;
    }
}

/*
 * This gets and returns the register that is being used.
 */
enum reg get_reg(u32 i)
{
    u32 _reg = *(u32*)(_asm[i]);
    _reg |= 32 + (32 >> 8) + (32 >> 16);
    _reg &= 0b00000000111111111111111111111111;
    switch (_reg)
    {
    case c_to_i('r','0','\0','\0'):
        return R0;
    case c_to_i('r','1','\0','\0'):
        return R1;
    case c_to_i('r','2','\0','\0'):
        return R2;
    case c_to_i('r','3','\0','\0'):
        return R3;
    case c_to_i('r','4','\0','\0'):
        return R4;
    case c_to_i('r','5','\0','\0'):
        return R5;
    case c_to_i('r','6','\0','\0'):
        return R6;
    case c_to_i('r','7','\0','\0'):
        return R7;
    case c_to_i('r','8','\0','\0'):
        return R8;
    case c_to_i('r','9','\0','\0'):
        return R9;
    case c_to_i('r','1','0','\0'):
        return R10;
    case c_to_i('r','1','1','\0'):
        return R11;
    case c_to_i('r','1','2','\0'):
        return R12;
    case c_to_i('r','1','3','\0'):
    case c_to_i('s','p','\0','\0'):
        return R13;
    case c_to_i('r','1','4','\0'):
    case c_to_i('l','r','\0','\0'):
        return R14;
    case c_to_i('p','c','\0','\0'):
    case c_to_i('r','1','5','\0'):
        return R15;
    default:
        return no_reg;
    }
}

/*
 * These functions process individual asm instructions. LOTS OF GOTOS AHEAD!
 */
static inline void process_sys_call(u32 i)
{
    printf("%i\n", i);
}
static inline void process_data(u32 i, enum data_process_operation operation)
{
    enum condition _condition = get_condition(i, sizeof(char) * 3);
    bool _set_flags = (_asm[i][3 + (_condition != AL) * 2] | 32) == 's';
    // TODO: i++ and checking if we are at a ';' can be somekind of macro.
    /*
     * This initializes everything to nothing so we don't have problems with
     * uninitialized values and the compiler yelling at me.
     */
    enum reg _destination_reg = no_reg;
    enum reg _first_reg = no_reg;
    enum reg _second_reg = no_reg;
    enum reg _shift_reg = no_reg;
    enum shift_type _shift = no_shift;
    u16 _second_immediate = 0;
    u16 _shift_value = 0;

    i++;
    if (_asm[i][0] == ';')
    {
        printf("Expected register after instruction.\n");
        exit(-1);
    }

    _destination_reg = get_reg(i);

    i++;
    if (_asm[i][0] == ';' || _destination_reg == no_reg)
    {
        printf("Expected a second register or immediate value.\n");
        exit(-1);
    }

    _first_reg = get_reg(i);
    if (_first_reg == no_reg)
    {
        /*
         * If it is an immediate value it can only be in operand two so we just
         * skip ahead.
         */
        if (_asm[i][0] == '#')
        {
            _second_immediate = get_12bit_immediate(i);
            goto process_data_return_no_operand_two;
        }
        else
        {
            printf("Expected a register or immediate.\n");
            exit(-1);
        }
    }

    i++;
    if (_asm[i][0] == ';')
        goto process_data_return_no_operand_two;

    /* This checks for a shift or another register. */
    _second_reg = get_reg(i);
    if (_second_reg == no_reg)
    {
        if (_asm[i][0] == '#')
        {
            _second_immediate = get_12bit_immediate(i);
            goto process_data_return_no_operand_two;
        }
        else
        {
            goto process_data_get_shift;
        }
    }

    i++;
    if (_asm[i][0] == ';')
        goto process_data_return_no_operand_two;

    /* This place can be a shift, register, or an immediate value. */
    _second_reg = get_reg(i);
    if (_second_reg == no_reg)
    {
        if (_asm[i][0] == '#')
        {
            _second_immediate = get_12bit_immediate(i);
            goto process_data_return_no_operand_two;
        }
        else
        {
            goto process_data_get_shift;
        }
    }

    /* This place can only be a shift type. */
    process_data_get_shift:
    _shift = get_shift(i);
    if (_shift == no_shift)
    {
        printf("Expected to find a shift type but found: %s.\n", _asm[i]);
        exit(-1);
    }

    i++;
    if (_asm[i][0] == ';')
    {
        printf("Expected a value after shift.\n");
        exit(-1);
    }
    
    /* This place can be a register or an immediate. */
    _shift_reg = get_reg(i);
    if (_shift_reg == no_reg)
    {
        if (_asm[i][0] == '#')
            i++;
        else
        {
            printf("Expected to find a value after a shift.\n");
            exit(-1);
        }
        _shift_value = get_int_immediate(i);
        if (_shift_value >= 1 << 5)
        {
            printf("Immediate shifts have to be under 32.\n");
            exit(-1);
        }
    }

    /*
     * This label handles both situation one and two, along side having a
     * terribly misleading name.
     */
    process_data_return_no_operand_two:
        _second_reg = _first_reg;
        _first_reg = _destination_reg;

    /*
     * This puts the final instruction together.
     */
    u32 result = _condition << 28;
    result += (bool)(_shift_reg == no_reg) << 25;
    result += operation << 21;
    result += _set_flags << 20;
    result += _first_reg << 16;
    result += _destination_reg << 12;
    /* This adds operand two. */
    if (_shift == no_shift)
        goto process_data_return_no_shift;
    if (_shift_reg == no_reg)
        result += _shift_value << 7;
    else
    {
        result += _shift_reg << 8;
        result |= 1 << 4;
    }
    result += _shift << 5;
    process_data_return_no_shift:
        result += _second_immediate;
    printf("%u\n", result);
    /*
     * This handles four situations
     * 1: One register and one other operand
     * 2: One reigster, one operand, and one shift
     * 3: One register and two operands
     * 4: One register, two operands, and one shift
     * 
     * 1 - We place the destination register into operand one and the first
     * operand goes into the second operand.
     * 2 - Same as one.
     * 3 - Set the shift to nothing.
     * 4 - No processing.
     */
    //_first_reg = get_reg();
    /* 
     * ADD R0, #24
     * ADD R0, R0, R1
     * ADD R0, R0, LSL R0
     * ADD R0, R0, R0, LSL R0
     * 
     * MVN R0, R1
     * MOV R0, R3
     * ADD R2, #142
     * 
     * ADD AA, BB, CC, DD, EE
     * 
     * A - Destination register
     * B - Source register
     * C - Second souce register
     * D - Shift
     * E - Register to shift from or immediate shift
     * 
     * ADD R0, R1, R1, LSL R0
     */
}
static inline void process_mem(u32 i, bool load_or_store)
{
    /*
     * ldr r0, [sp, #-4]!
     * ldr r0, [sp, r0]
     */
    enum reg _destination_reg = no_reg;
    enum reg _source_reg = no_reg;
    enum reg _offset_reg = no_reg;
    enum reg _shift_reg = no_reg;
    enum shift_type _shift = no_shift;
    u32 _shift_immediate = 0;
    u32 _offset_immediate = 0;
    bool _preincrement = false;

    u32 result = (1 << 26);
    u32 _condition = get_condition(i, sizeof(char) * 3);
    bool _byte = (_asm[i][3 + (_condition != AL) * 2] | 32) == 'b';
    _destination_reg = get_reg(i+1);
    if (_destination_reg == no_reg)
    {
        i++;
        goto process_branch_no_register;
    }

    if (_asm[i+2][0] != '[')
    {
        i += 2;
        goto process_branch_no_register;
    }

    _source_reg = get_reg(i+3);

    if (_source_reg == no_reg)
    {
        i += 2;
        goto process_branch_no_register;
    }

    if (_asm[i+4][0] == ']')
    {
        i += 4;
        goto process_branch_return_post;
    }

    /* This place can either be a register or an immediate. */
    _offset_reg = get_reg(i+4);
    if (_offset_reg == no_reg)
    {
        if (_asm[i+4][0] == '#')
        {
            _offset_immediate = get_int_immediate(i);
            if (_offset_immediate >= 1 << 12)
            {
                printf("Immediate offsets must be under 4096.");
                exit(-1);
            }
        }
        else
        {
            i += 4;
            goto process_branch_no_register;
        }
    }

    if (_asm[i+5][0] == ']')
    {
        i += 6;
        goto process_branch_return_post;
    }

    /* This place can only be a shift type. */
    _shift = get_shift(i+5);
    if (_shift == no_shift)
    {
        printf("Expected to find shift but found: %s.\n", _asm[i+5]);
        exit(-1);
    }

    /* This place can only be a shift. */
    _shift_reg = get_reg(i+6);
    if (_shift_reg == no_reg)
    {
        _shift_immediate = get_int_immediate(i);
        // This should make sure the shift is valid
    }
    i += 5;

    /*
     * This checks for pre incrementing and for write backs.
     */
    process_branch_return_post:
        printf("%s\n", _asm[i]);
        _preincrement = _asm[i][0] == '!';
        if (_offset_reg == no_reg && _asm[i+1][0] != ';' && !_preincrement)
            _offset_reg = get_reg(i+1);

    result |= _condition << 28;
    result |= load_or_store << 20;
    result |= 1 << 21;
    result |= _byte << 22;
    result |= 1 << 23;
    result |= _preincrement << 24;
    result |= !(_offset_reg == no_reg) << 25;
    // 20 - Loading or storing
    // 21 - Write back
    // 22 - Byte / Word
    // 23 - Positive or negative
    // 24 - Pre / Post indexing
    // 25 - If offset is immediate

    result |= _destination_reg << 12;
    result |= _source_reg << 16;
    if (_offset_reg == no_reg)
        result |= _offset_immediate;
    else
    {
        result |= _offset_reg;
        if (_shift_reg == no_reg)
            result |= _shift_immediate << 7;
        else
            result |= _shift_reg << 8;
        if (_shift != no_shift)
            result |= _shift << 5;
    }

    printf("%u\n", result);
    return;

    process_branch_no_register:
        printf("Expected register but found: %s.\n", _asm[i]);
        exit(-1);
}
static inline void process_branch(u32 i)
{
    //
}
static inline void process_clz(u32 i)
{
    // TODO: See if "clz" can set flags
    u32 result = get_condition(i, sizeof(char) * 3) << 28;
    result += get_reg(i+1) << 12;
    result += get_reg(i+2);
    //return result + 0x006f0f10;
    result += 0x016f0f10;
    printf("%u\n", result);
}

/*int main()
{
    // TODO "_asm" should have padding of '\0's on the end
    char** _asm_ = malloc(sizeof(char) * 64);
    if (_asm_ == 0)
    {
        printf("Failed to allocate memory.\n");
        exit(-1);
    }
    // ldr r0, [r0, r1]!
    char* temp = "ldrb";
    _asm_[0] = temp;
    temp = "r0";
    _asm_[1] = temp;
    temp = "[";
    _asm_[2] = temp;
    temp = "r0";
    _asm_[3] = temp;
    temp = "r1";
    _asm_[4] = temp;
    temp = "]";
    _asm_[5] = temp;
    temp = "!";
    _asm_[6] = temp;
    //temp = "r2";
    //_asm_[4] = temp;

    // _asm = _asm_;
    // printf("%i\n", get_12bit_immediate(0));
    asm_into_instructions(1, _asm_);
    free(_asm_);
    return 1;
}*/