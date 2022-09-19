.global _start
_start:
  ldr r2, =file
  mov r3, sp // BF stack pointer
  sub sp, #65536
  mov r5, #0 // TEMP
  b reset_m_l
  
main_loop:
  cmp r0, #0
  beq _end
  cmp r0, #62
  beq forward
  cmp r0, #60
  beq back
  cmp r0, #43
  addeq r1, #1
  beq reset_m_l
  cmp r0, #45
  subeq r1, #1
  beq reset_m_l
  cmp r0, #93
  beq closing
  	reset_m_l:
		ldrb r0, [r2], #1 // Loads in the next char to read
		b main_loop // Loops back to m_l

forward:
  // sub sp, #1 // TODO: this will cause a memory leak 
  strb r1, [r3], #-1
  ldrb r1, [r3]
  b reset_m_l
back:
  strb r1, [r3], #1
  ldrb r1, [r3]
  b reset_m_l
closing:
  lsl r1, #24
  lsr r1, #24
  cmp r1, #0
  beq reset_m_l
  sub r4, r2, #1 // r4 is the temp BF stack pointer
  b reset
  c_loop:
  	// sub r5, #1 // The counter
	cmp r0, #93
	addeq r5, #1
	cmp r0, #91
	bne reset
	opening:
		cmp r5, #0
		subne r5, #1
		moveq r5, #0 // Reset the counter
  		subeq r2, r4, #0 // Set the stack pointer to the correct location
		b reset_m_l
	reset:
		ldrb r0, [r4, #-1]! // Incrament r4 backwards and load the char into r6
		b c_loop

_end:
  //mov r7, #1
  //swi 0
  b _end

.data
// > 62
// < 60
// [ 91
// ] 93
	// If we hit a ']' go back and each time we go over a '[' add one and ']' sub one if we hit a '[' and we are at zero set the pointer there
// + 43
// - 45
file: 
	.string "[+>]++++"
	.byte 0
