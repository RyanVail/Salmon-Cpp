/* This file handles turning ASM into machine code. */

struct instruction;
enum register;
struct shift_immediate;
struct shift_register;
struct shift_type;
struct data_processing;

enum condition
{
    EQ // 0x0 // Set equal
    NE // 0x1 // Clear not equal
    CS // 0x2 // Set unsigned higher or same
    CC // 0x3 // Clear unsigned lower
    MI // 0x4 // Set negative
    PL // 0x5 // Clear positive or negative
    VS // 0x6 // Set overflow
    VC // 0x7 // Clear no overflow
    HI // 0x8 // Set and Z clear unsigned higher
    LS // 0x9 // Clear or Z set unsigned lower or same
    GE // 0xa // >
    LT // 0xb // <
    GT // 0xc // >=
    LE // 0xd // <=
    AL // 0xe // Always
};

struct instruction
{
    /*
    * 0000 0000000000000000000000000000
    * ^^^^
    * Condition
    */
    enum condition condition;
};

enum register
{
    //
};

enum shift_type
{
    LSL, // 0x0
    LSR, // 0x1
    ASR, // 0x2
    ROR, // 0x3
};
struct shift_immediate_8bit
{
    u8 shift; // u5
    enum shift_type shift_type;
    // 0
};
struct shift_register_8bit
{
    enum register register;
    // 0
    enum shift_type shift_type;
    // 1
};
struct register_shift_12bit
{
    bool is_register_shift;
    struct shift_register_8bit shift_register_8bit;
    struct shift_immediate_8bit shift_immediate_8bit;

    enum register register;
};
enum register_shift_12bit
{
    enum register register;
    // 0
    enum shift_type shift_type;
    // 1
};

struct data_processing
{
    cond condition;
    // 00
    bool immediate; 
    enum data_processing_operations
    {
        AND, // 0x0 // operand one & operand two
        EOR, // 0x1 // operand one ^ operand two
        SUB, // 0x2 // operand one - operand two
        RSB, // 0x3 // operand two + operand one
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
    };
    bool set_conditions;
    enum register operand_register;
    enum register destination_register;
    enum shift_type immediate_shift;
    struct immediate_12bit immediate;
};

#undef COMMENT