
- LEXER:
	- TOKENIZER

- INTERMEDIATE REPERSENTATION:
	- FIRST PASS:
		- DEFINTION SUBSITUTION
		- COMMENT REMOVAL
		- VARIABLE SYMBOL TABLE
		- VARIALBE DECLERATIONS & TYPES <Variable types should be handled in intermediate to asm>
		- TOKENS TO INTERMEDIATE
		- VARIABLE REDECLERATIONS
		- VARIABLE ACCESS
		- FUNCTION SYMBOL TABLE
		- FUNCTION CALLS
	- SECOND PASS / OPTIMIZATION PASS:
		- CONST COMPUTATIONS
		- COMPILE TIME OPTIMIZATIONS
		- DEAD CODE REMOVAL
	- THIRD PASS:
		- FUNCTION STACK SIZE CACULATOR

- ASM:
	- INTERMEDIATE TO ASM